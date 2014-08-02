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
            ConvertNamespace( t, pn + "Packets" );

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
                    var tn = GetTypeKeyword( f );
                    sb.Append( ( f.Desc == "" ? "" : GetComment( f.Desc, 8 ) ) + @"
        " + tn + " _" + f.Name.ToFirstLower() + ( ( GetDefaultValueByType( f ) != "" ) ? ( " = " + GetDefaultValueByType( f ) ) : "" ) + ";" );
                }
                sb.Append( @"

        static void fill( FlatBuffer& fb" );
                foreach( var f in c.Fields )
                {
                    var tn = GetTypeKeyword( f );
                    sb.Append( @"
            , " + tn + " " + f.Name );
                }
                sb.Append( @" );
" );
                foreach( var f in c.Fields )
                {
                    var tn = GetTypeKeyword( f );
                    if( f.Get )
                    {
                        sb.Append( @"
        " + tn + " const& get" + f.Name.ToFirstUpper() + @"();" );
                    }
                    if( f.Set )
                    {
                        if( IsSimpleType( f ) )
                        {
                            sb.Append( @"
        inline void set" + f.Name.ToFirstUpper() + @"( " + tn + @" v )
        {
            _" + f.Name.ToFirstLower() + @" = v;
        }" );
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
                sb.WriteToFile( Path.Combine( outDir, pn + ".h" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }

            sb.Clear();



            sb.Append( @"#include ""Lib/All.h""
#include """ + pn + @".h""

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
                        if( f.TypeClass is Class && IsPod( (Class)f.TypeClass ) )
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
                        if( f.TypeClass is Class && IsPod( (Class)f.TypeClass ) )
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
                        var tn = GetTypeKeyword( f );
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
                    var tn = GetTypeKeyword( f );
                    sb.Append( @"
        , " + tn + @" p" + f.Name.ToFirstUpper() );
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
                        var tn = GetTypeKeyword( f );
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
                    foreach( var f in c.Fields )
                    {
                        var tn = GetTypeKeyword( f );
                        sb.Append( @"
        if( !fb.read( _" + f.Name.ToFirstLower() + @" ) ) return false;" );
                    }
                }
                sb.Append( @"
        return true;
    }

    void " + c.Name + @"::writeBufferDirect( FlatBuffer& fb ) const
    {" );
                if( c.Fields.Count > 0 )
                {
                    sb.Append( @"
        fb.writesDirect(" );
                    foreach( var f in c.Fields )
                    {
                        var tn = GetTypeKeyword( f );
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
                        var tn = GetTypeKeyword( f );
                        if( IsSimpleType( f ) )
                        {
                            sb.Append( @"
            sizeof( _" + f.Name.ToFirstLower() + " )" );
                        }
                        else
                        {
                            sb.Append( @"
            _" + f.Name.ToFirstLower() + ".getWriteBufferSize()" );
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
                sb.WriteToFile( Path.Combine( outDir, pn + ".cpp" ) );
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
                    if( f.TypeNamespace == "" ) continue;
                    if( f.TypeNamespace == "System" )
                    {
                        f.TypeNamespace = "";
                    }
                    else
                    {
                        f.TypeNamespace = ( baseNS == "" ? "" : ( "::" + baseNS + "::" ) ) + f.TypeNamespace.Replace( ".", "::" );
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
                if( f.IsList || f.IsDictionary || f.Type == "String" && f.TypeNamespace == "" )
                {
                    return false;
                }
                if( f.TypeClass != null && f.TypeClass.IsEnum ) continue;
                switch( f.Type )
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
                    continue;
                }
                if( !IsPod( (Class)f.TypeClass ) ) return false;
            }
            return true;
        }


        public static bool IsSimpleType( ClassField f )
        {
            if( f.IsList || f.IsDictionary || f.Type == "String" && f.TypeNamespace == "" )
            {
                return false;
            }
            if( f.TypeClass != null && f.TypeClass.IsEnum ) return true;
            switch( f.Type )
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
            if( f.IsArray || f.IsList || f.IsDictionary )
            {
                return "";
            }
            switch( f.Type )
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
            default:
                if( f.TypeClass != null && f.TypeClass.IsEnum )
                {
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type + "." + ( (Enum)f.TypeClass ).Fields.First().Name;
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

        public static string GetTypeKeyword( Enum c )
        {
            switch( c.Type )
            {
            case "Byte": return "byte";
            case "SByte": return "sbyte";
            case "UInt16": return "ushort";
            case "Int16": return "short";
            case "UInt32": return "uint";
            case "Int32": return "int";
            case "UInt64": return "uint64";
            case "Int64": return "int64";
            }
            throw new Exception( "it can't be" );
        }
        public static string GetTypeKeyword( ClassField f )
        {
            switch( f.Type )
            {
            case "Byte": return "byte";
            case "SByte": return "sbyte";
            case "UInt16": return "ushort";
            case "Int16": return "short";
            case "UInt32": return "uint";
            case "Int32": return "int";
            case "UInt64": return "uint64";
            case "Int64": return "int64";
            case "Double": return "double";
            case "Single": return "float";
            case "Boolean": return "bool";
            //case "String": return "String";
            default:
                //return ( f.TypeNamespace != "" ? f.TypeNamespace : ( "::" + _pn + "Packets" ) ) + "::" + f.Type;
                return ( f.TypeNamespace != "" ? ( f.TypeNamespace + "::" ) : "" ) + f.Type;
            }
        }
        //public static string GetKeyTypeKeyword( ClassField f )
        //{
        //    switch( f.KeyType )
        //    {
        //    case "Byte": return "byte";
        //    case "Byte[]": return "byte[]";
        //    case "SByte": return "sbyte";
        //    case "UInt16": return "ushort";
        //    case "Int16": return "short";
        //    case "UInt32": return "uint";
        //    case "Int32": return "int";
        //    case "UInt64": return "ulong";
        //    case "Int64": return "long";
        //    case "Double": return "double";
        //    case "Single": return "float";
        //    case "Boolean": return "bool";
        //    case "String": return "string";
        //    case "DateTime": return "DateTime";
        //    default:
        //        if( f.KeyTypeClass != null && f.KeyTypeClass.IsEnum )
        //        {
        //            return ( f.KeyTypeNamespace != "" ? f.KeyTypeNamespace : _pn ) + "." + f.KeyType;
        //        }
        //        else
        //            return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type;
        //    }
        //}

    }
}
