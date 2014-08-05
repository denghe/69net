using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PacketGenerator
{
    public static class GenCPP
    {
        public static string Gen( Template t, string outDir, string pn )
        {
            _pn = pn;
            var ns = pn + "Packets";
            ConvertNamespace( t, ns );

            var sb = new StringBuilder();

            sb.Append( @"#ifndef __" + pn.ToUpper() + @"_H__
#define __" + pn.ToUpper() + @"_H__


namespace " + pn + @"Packets
{" );
            foreach( var c in t.Classes )
            {
                sb.Append( @"
    class " + c.Name + @";" );
            }

            sb.Append( @"
}
namespace " + pn + @"Packets
{" );

            foreach( var c in t.Classes )
            {
                sb.Append( ( c.Desc == "" ? "" : GetComment( c.Desc, 4 ) ) + @"
    class " + c.Name + @" : Memmoveable
    {
        PACKET_CLASS_HEADER" + ( IsPod( c ) ? "_POD" : "" ) + @"( " + c.Name + @" );
public:
" );
                foreach( var f in c.Fields )
                {
                    var tn = SplitTypeKeyword( GetTypeKeyword( f.Declare ), f.Declare );
                    sb.Append( ( f.Desc == "" ? "" : GetComment( f.Desc, 8 ) ) + @"
        " + tn.Key + " _" + f.Name.ToFirstLower() + tn.Value + ( ( GetDefaultValueByType( f ) != "" ) ? ( " = " + GetDefaultValueByType( f ) ) : "" ) + ";" );
                }
                sb.Append( @"

        static void fill( FlatBuffer& fb" );
                foreach( var f in c.Fields )
                {
                    var tn = GetTypeKeyword( f.Declare );
                    var atn = SplitTypeKeyword( GetTypeKeyword( f.Declare ), f.Declare );
                    if( f.Declare.DataType == DataTypes.Array )
                    {
                        sb.Append( @"
            , " + atn.Key + "( &" + f.Name + " )" + atn.Value );
                    }
                    else
                    {
                        sb.Append( @"
            , " + tn + " " + f.Name );
                    }
                }
                sb.Append( @" );
" );
                foreach( var f in c.Fields )
                {
                    var tn = GetTypeKeyword( f.Declare );
                    var atn = SplitTypeKeyword( GetTypeKeyword( f.Declare ), f.Declare );
                    if( f.Get )
                    {
                        sb.Append( @"
        " + tn + " const& get" + f.Name.ToFirstUpper() + @"();" );
                    }
                    if( f.Set )
                    {
                        if( IsSimpleType( f ) )
                        {
                            if( f.Declare.DataType == DataTypes.Array )
                            {
                                sb.Append( @"
        inline void set" + f.Name.ToFirstUpper() + @"( " + atn.Key + @"( &v )" + atn.Value + @" )
        {
            _" + f.Name.ToFirstLower() + @" = v;
        }" );
                            }
                            else
                            {
                                sb.Append( @"
        inline void set" + f.Name.ToFirstUpper() + @"( " + tn + @" v )
        {
            _" + f.Name.ToFirstLower() + @" = v;
        }" );
                            }
                        }
                        else
                        {
                            sb.Append( @"
        template<typename VT> void set" + f.Name.ToFirstUpper() + @"( VT&& v )
        {
            _" + f.Name.ToFirstLower() + @" = std::forward<VT>( v );
        }" );
                        }
                    }
                }
                sb.Append( @"
    };" );
            }

            sb.Append( @"
}
#endif
" );


            try
            {
                sb.WriteToFile( Path.Combine( outDir, ns + ".h" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }

            sb.Clear();



            sb.Append( @"#include ""Lib/All.h""
#include """ + ns + @".h""

namespace " + pn + @"Packets
{" );
            foreach( var c in t.Classes )
            {
                sb.Append( @"
    ushort " + c.Name + @"::getTypeID()
    {
        return " + c.TypeID + @";
    }" );

                if( !IsPod( c ) )
                {
                    sb.Append( @"
    " + c.Name + @"::" + c.Name + @"( " + c.Name + @" const& other )" );
                    bool isFirst = true;
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
        " + ( isFirst ? ":" : "," ) + " _" + f.Name.ToFirstLower() + @"( other._" + f.Name.ToFirstLower() + @" )" );
                        isFirst = false;
                    }
                    sb.Append( @"
    {
    }

    " + c.Name + @"::" + c.Name + @"( " + c.Name + @"&& other )" );
                    isFirst = true;
                    foreach( var f in c.Fields )
                    {
                        if( ( f.Declare.Class as Class != null ) && IsPod( (Class)f.Declare.Class ) )
                        {
                            sb.Append( @"
        " + ( isFirst ? ":" : "," ) + " _" + f.Name.ToFirstLower() + @"( other._" + f.Name.ToFirstLower() + @" )" );
                        }
                        else
                        {
                            sb.Append( @"
        " + ( isFirst ? ":" : "," ) + " _" + f.Name.ToFirstLower() + @"( std::move( other._" + f.Name.ToFirstLower() + @" ) )" );
                        }
                        isFirst = false;
                    }
                    sb.Append( @"
    {
    }

    " + c.Name + @"& " + c.Name + @"::operator=( " + c.Name + @" const& other )
    {" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
        _" + f.Name.ToFirstLower() + @" = other._" + f.Name.ToFirstLower() + @";" );
                    }
                    sb.Append( @"
        return *this;
    }

    " + c.Name + @"& " + c.Name + @"::operator=( " + c.Name + @"&& other )
    {" );
                    foreach( var f in c.Fields )
                    {
                        if( ( f.Declare.Class as Class != null ) && IsPod( (Class)f.Declare.Class ) )
                        {
                            sb.Append( @"
        _" + f.Name.ToFirstLower() + @" = other._" + f.Name.ToFirstLower() + @";" );
                        }
                        else
                        {
                            sb.Append( @"
        _" + f.Name.ToFirstLower() + @" = std::move( other._" + f.Name.ToFirstLower() + @" );" );
                        }
                    }
                    sb.Append( @"
        return *this;
    }

" );
                }


                foreach( var f in c.Fields )
                {
                    if( f.Get )
                    {
                        var tn = GetTypeKeyword( f.Declare );
                        sb.Append( @"
    " + tn + " const& " + c.Name + @"::get" + f.Name.ToFirstUpper() + @"()
    {
        return _" + f.Name.ToFirstLower() + @";
    }" );
                    }
                }

                sb.Append( @"
    void " + c.Name + @"::fill( FlatBuffer& fb" );
                foreach( var f in c.Fields )
                {
                    var tn = GetTypeKeyword( f.Declare );
                    var atn = SplitTypeKeyword( GetTypeKeyword( f.Declare ), f.Declare );
                    if( f.Declare.DataType == DataTypes.Array )
                    {
                        sb.Append( @"
        , " + atn.Key + @"( &p" + f.Name.ToFirstUpper() + " )" + atn.Value );
                    }
                    else
                    {
                        sb.Append( @"
        , " + tn + @" p" + f.Name.ToFirstUpper() );
                    }
                }
                sb.Append( @" )
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        fb.writes(" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
            p" + f.Name.ToFirstUpper() + ( f == c.Fields[ c.Fields.Count - 1 ] ? "" : @", " ) );
                    }
                    sb.Append( @" );" );
                }
                sb.Append( @"
    }" );

                sb.Append( @"

    void " + c.Name + @"::writeBuffer( FlatBuffer& fb ) const
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        fb.writes(" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
            _" + f.Name.ToFirstLower() + ( f == c.Fields[ c.Fields.Count - 1 ] ? "" : @", " ) );
                    }
                    sb.Append( @" );" );
                }

                sb.Append( @"
    }

    bool " + c.Name + @"::readBuffer( FlatBuffer& fb )
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        return fb.reads(" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
            _" + f.Name.ToFirstLower() + ( f == c.Fields[ c.Fields.Count - 1 ] ? "" : @", " ) );
                    }
                    sb.Append( @" );" );
                }
                else
                {
                    sb.Append( @"
        return true;" );
                }
                sb.Append( @"
    }

    void " + c.Name + @"::writeBufferDirect( FlatBuffer& fb ) const
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        fb.writesDirect(" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
            _" + f.Name.ToFirstLower() + ( f == c.Fields[ c.Fields.Count - 1 ] ? "" : @", " ) );
                    }
                    sb.Append( @" );" );
                }

                sb.Append( @"
    }

    int " + c.Name + @"::getWriteBufferSize() const
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        return " );
                    foreach( var f in c.Fields )
                    {
                        if( IsSimpleType( f ) )
                        {
                            sb.Append( @"
            sizeof( _" + f.Name.ToFirstLower() + " )" );
                        }
                        else
                        {
                            sb.Append( @"
            BufferUtils::getSizes( _" + f.Name.ToFirstLower() + " )" );
                        }
                        sb.Append( f == c.Fields[ c.Fields.Count - 1 ] ? "" : @" + " );
                    }
                    sb.Append( @";" );
                }

                sb.Append( @"
    }
" );

            }

            sb.Append( @"
}
" );


            try
            {
                sb.WriteToFile( Path.Combine( outDir, ns + ".cpp" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }

            System.Diagnostics.Process.Start( "explorer.exe", outDir );
            return "";
        }


        public static string ToFieldName( string s )
        {
            if( s.Length <= 1 ) return "_" + s.ToLower();
            return "_" + s.Substring( 0, 1 ).ToLower() + s.Substring( 1 );
        }



        // 项目名上下文，省掉下面的传参了
        public static string _pn;


        // 把 Namespace 啥中间的 . 转为 ::
        public static void ConvertNamespace( Template t, string baseNS = "" )
        {
            foreach( var c in t.Classes )
            {
                /*if (c.Namespace == "") c.Namespace = "::" + baseNS;
                else */
                c.Namespace = c.Namespace.Replace( ".", "::" );
                foreach( var f in c.Fields )
                {
                    if( f.Declare.Namespace == "" ) continue;
                    if( f.Declare.Namespace == "System" )
                    {
                        f.Declare.Namespace = "";
                    }
                    else
                    {
                        f.Declare.Namespace = ( baseNS == "" ? "" : ( "::" + baseNS + "::" ) ) + f.Declare.Namespace.Replace( ".", "::" );
                    }
                }
            }
        }


        /// <summary>
        /// 成员数据类型 全是简单数据类型, 或由 简单数据类型组成的类( 这个和 c++ 的 std::is_pod 不一样，没那么严格 ) 
        /// </summary>
        public static bool IsPod( Class c )
        {
            foreach( var f in c.Fields )
            {
                if( f.Declare.Name == "String" )
                    return false;
                if( f.Declare.DataType == DataTypes.Generic ) return false;
                if( f.Declare.DataType == DataTypes.BuiltIn ) continue;
                if( f.Declare.DataType == DataTypes.Custom )
                {
                    if( f.Declare.Class == null )
                    {
                        throw new Exception( "should not be null" );
                    }
                    if( f.Declare.Class != null && f.Declare.Class.IsEnum ) continue;
                    if( !IsPod( (Class)f.Declare.Class ) ) return false;
                }
            }
            return true;
        }


        public static bool IsSimpleType( ClassField f )
        {
            if( f.Declare.DataType != DataTypes.BuiltIn ) return false;
            if( f.Declare.Name == "String" )
            {
                return false;
            }
            if( f.Declare.Class != null && f.Declare.Class.IsEnum ) return true;
            switch( f.Declare.Name )
            {
            case "Byte":
            case "UInt16":
            case "UInt32":
            case "UInt64":
            case "SByte":
            case "Int16":
            case "Int32":
            case "Int64":
            case "Double":
            case "Single":
            case "Boolean":
                return true;
            }
            return false;
        }

        public static string GetDefaultValueByType( ClassField f )
        {
            if( f.Declare.DataType != DataTypes.BuiltIn )
            {
                return "";
            }
            switch( f.Declare.Name )
            {
            case "Byte":
            case "UInt16":
            case "UInt32":
            case "UInt64":
            case "SByte":
            case "Int16":
            case "Int32":
            case "Int64":
            case "Double":
            case "Single": return "0";
            case "Boolean": return "false";
            case "String": return "";
            default:
                if( f.Declare.Class != null && f.Declare.Class.IsEnum )
                {
                    return ( f.Declare.Namespace != "" ? f.Declare.Namespace : _pn ) + "." + f.Declare.Name + "." + ( (Enum)f.Declare.Class ).Fields.First().Name;
                }
                return "";
            }
        }

        public static string GetComment( string s, int space )
        {
            if( s.Trim() == "" ) return "";
            var sps = new string( ' ', space );
            return "\r\n" +
sps + @"/*
" + sps + @" * " + s + @"
" + sps + @"*/";
        }

        public static string GetTypeKeyword( Declare d )
        {
            string rtv = "";
            if( d.DataType == DataTypes.Array )
            {
                rtv = GetTypeKeyword( d.Childs[ 0 ] ) + "[" + d.MinLen + "]";
            }
            else if( d.DataType == DataTypes.BuiltIn )
            {
                switch( d.Name )
                {
                case "Byte": rtv = "byte"; break;
                case "SByte": rtv = "sbyte"; break;
                case "UInt16": rtv = "ushort"; break;
                case "Int16": rtv = "short"; break;
                case "UInt32": rtv = "uint"; break;
                case "Int32": rtv = "int"; break;
                case "UInt64": rtv = "uint64"; break;
                case "Int64": rtv = "int64"; break;
                case "Double": rtv = "double"; break;
                case "Single": rtv = "float"; break;
                case "Boolean": rtv = "bool"; break;
                default:
                    rtv = ( d.Namespace != "" ? ( d.Namespace + "::" ) : "" ) + d.Name;
                    break;
                }
            }
            else if( d.DataType == DataTypes.Custom )
            {
                rtv = ( d.Namespace != "" ? d.Namespace : ( "::" + _pn + "Packets" ) ) + "::" + d.Name;
            }
            else
            {
                rtv = d.Name + "<";
                for( int i = 0; i < d.Childs.Count; ++i )
                {
                    if( i > 0 ) rtv += ",";
                    rtv += GetTypeKeyword( d.Childs[ i ] );
                }
                rtv += ">";
            }
            return rtv;
        }

        // todo: 将类型声明尾部的 1组或多组 [???] 分离并返回
        public static KeyValuePair<string, string> SplitTypeKeyword( string k, Declare d )
        {
            int n = 0;
            while( d.DataType == DataTypes.Array )
            {
                n += 2 + d.MinLen.ToString().Length;
                d = d.Childs[ 0 ];
            }
            return new KeyValuePair<string, string>( k.Substring( 0, k.Length - n ), k.Substring( k.Length - n, n ) );
        }
    }
}
