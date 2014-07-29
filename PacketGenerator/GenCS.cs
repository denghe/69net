using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PacketGenerator
{
    public static class GenCS
    {
        public static string _fn;
        public static string Gen( Template t, string outputDirPath, string fn )
        {
            _fn = fn = fn.Substring( "PacketTemplate_".Length );
            var nss = t.Classes.Select( a => a.Namespace ).Concat( t.Enums.Select( a => a.Namespace ) ).Distinct().ToList();

            var sb = new StringBuilder();








            sb.Clear();
            sb.Append( @"using SerialHelper;

// 解码函数数组声明
//
public Action<Bytes>[] PacketHandlers;


// 向数组存入对应的处理函数
//
PacketHandlers = new Action<Bytes, params object[]>[ (int)" + fn + @"_AllClassEnums.ZMaxValue ];

" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
PacketHandlers[ (int)" + fn + @"_AllClassEnums." + en + @" ] = " + en + @";" );
                }
            }

            sb.Append( @"

// 不获取标记位系列函数实现空壳

" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
private void " + en + @"( Bytes b, params object[] extPs )
{
    var o = new " + ns + @"." + c.Name + @"();
    var rtv = o.readFrom( b );              
    if( rtv != ReadState.Success )
    {
        return;     // 这里放置解包错误处理代码
    }

    // if( On_" + en + @" != null ) On_" + en + @"( o );
}

" );
                }
            }
            sb.Append( @"

// 获取标记位

" );

            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
private void " + en + @"( Bytes b, params object[] ext_ps )
{
    var o = new " + ns + @"." + c.Name + @"();
    var flags = new bool[ (int)" + ns + @"." + c.Name + @"_e.ZMaxValue ];
    var rtv = o.readFrom( b, flags );
    if( rtv != ReadState.Success )
    {
        // 这里放置解包错误处理代码
        return;
    }
    // read success code here
    if( On_" + en + @" != null ) On_" + en + @"( o );
" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
    if( flags[ (int)" + ns + @"." + c.Name + @"_e." + f.Name + @" ] )
    {
        // 处理代码
        if( On_" + en + @"_m_" + f.Name + @" != null ) On_" + en + @"_m_" + f.Name + @"( o );
    }" );
                    }
                    sb.Append( @"
    
}

" );
                }
            }
            sb.Append( @"




" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
public static event System.Action<" + ns + "." + c.Name + @"> On_" + en + @";" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( @"
public static event System.Action<" + ns + "." + c.Name + @"> On_" + en + @"_m_" + f.Name + @";" );
                    }
                }
            }
            sb.Append( @"




" );


            sb.Append( @"

// 可能有用的扩展函数
public static byte[] toByteArray( this ISC o )
{
    var b = new Bytes();
    b.write( o );
    b.collapse();
    return b._buf;
}






#region SyncSend

/// <summary>
/// 同步获取返回结果
/// </summary>
public T SyncSend<T>( ISC cmd, int timeoutSec ) where T : ISC
{
    string cb_ = Guid.NewGuid().ToString();
    bool got_ = false;
    T rtv_ = default( T );
    var func = new Action<string, T>( ( cb, o ) =>
    {
        if( cb_ != cb ) return;
        got_ = true;
        rtv_ = o;
    } );
    RegisterHandler<T>( func );
    Send( cmd, cb_ );
    for( int i = 0; i < timeoutSec * 10; ++i )
    {
        if( got_ ) goto LabEnd;
        System.Threading.Thread.Sleep( 100 );
    }
LabEnd:
    UnregisterHandler<T>( func );
    return rtv_;
}

private void RegisterHandler<T>( dynamic func ) where T : ISC
{");
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
    if( typeof( T ) == typeof( " + ns + "." + c.Name + @" ) ) On_" + en + @" += func;" );
                }
            }
            sb.Append(@"
}
private void UnegisterHandler<T>( dynamic func ) where T : ISC
{" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
    if( typeof( T ) == typeof( " + ns + "." + c.Name + @" ) ) On_" + en + @" -= func;" );
                }
            }
            sb.Append( @"
}

#endregion




" );




            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_Samples.txt" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }







            sb.Clear();
            sb.Append( @"using SerialHelper;

public static partial class " + fn + @"_AllClassHandler
{
    public static ISC read( int opcode, byte[] buf )
    {
        var b = new Bytes( buf, buf.Length );
        switch( (" + fn + @"_AllClassEnums)opcode )
        {" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;

                    sb.Append( @"
            case " + fn + @"_AllClassEnums." + en + @":
            {
                var o = new " + ns + @"." + c.Name + @"();
                var rtv = o.readFrom( b );
                if( rtv != ReadState.Success ) return null;
                return o;
            }" );
                }
            }
            sb.Append( @"
        default:
            return null;
        }
    }
}
" );

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_AllClassHandler.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }






            sb.Clear();
            sb.Append( @"
public enum " + fn + @"_AllClassEnums
{" );
            int idx = 0;
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( GetComment( c.Desc, 4 ) + @"
    " + en + @" = " + ( idx++ ) + @"," );
                }
            }
            sb.Append( @"
    ZMaxValue = " + t.Classes.Count + @"
}
" );

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_AllClassEnums.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }




            sb.Clear();
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                sb.Append( @"
namespace " + ns + @"
{" );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    sb.Append( GetComment( c.Desc, 4 ) + @"
    public enum " + c.Name + @"_e : byte
    {" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( GetComment( f.Desc, 8 ) + @"
        " + f.Name + @"," );
                    }
                    sb.Append( @"
        ZMaxValue = " + c.Fields.Count + @"
    }
" );
                }
                sb.Append( @"
}
" );
            }

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_Enum.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }



            // todo: change tn add basenamespace




            sb.Clear();
            sb.Append( @"using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SerialHelper
{
    partial class Bytes
    {" );
            foreach( var e in t.Enums )
            {
                var ns = ( e.Namespace != "" ? e.Namespace : fn );
                var tn = ns + "." + e.Name;
                sb.Append( @"
        public void write( List<" + tn + @"> v )
        {
            if( v == null )
            {
                write( -1 );
                return;
            }
            else if( v.Count == 0 )
            {
                write( 0 );
                return;
            }
            write( v.Count );
            for( int i = 0; i < v.Count; ++i )
            {
                write( (" + e.GetTypeKeyword() + @")v[ i ] );
            }
        }
        public ReadState read( ref List<" + tn + @"> v )
        {
            int len = 0;
            var rtv = read( ref len );
            if( rtv != ReadState.Success ) return rtv;
            if( len < -1 ) return ReadState.Fail;
            else if( len == -1 )
            {
                v = null;
                return rtv;
            }
            else if( len == 0 )
            {
                if( v != null ) v.Clear();
                else v = new List<" + tn + @">();
                return rtv;
            }
            if( v == null ) v = new List<" + tn + @">();
            else v.Clear();
            for( int i = 0; i < len; ++i )
            {
                " + e.GetTypeKeyword() + @" o = 0;
                rtv = read( ref o );
                if( rtv != ReadState.Success ) return rtv;
                v.Add( (" + tn + @")o );
            }
            return ReadState.Success;
        }
" );
            }

            foreach( var g in t.Classes.SelectMany( a => a.Fields ).Where( a => a.IsDictionary ).GroupBy( a => a.KeyTypeNamespace + "." + a.KeyType + "," + a.TypeNamespace + "." + a.Type ) )
            {
                var f = g.First();
                var ktn = f.GetKeyTypeKeyword();
                var vtn = f.GetTypeKeyword();
                sb.Append( @"
        public void write( Dictionary<" + ktn + @", " + vtn + @"> v )
        {
            if( v == null )
            {
                write( -1 );
                return;
            }
            else if( v.Count == 0 )
            {
                write( 0 );
                return;
            }
            write( v.Count );
            write( v.Keys.ToList() );
            write( v.Values.ToList() );
        }
        public ReadState read( ref Dictionary<" + ktn + @", " + vtn + @"> v )
        {
            int len = 0;
            var rtv = read( ref len );
            if( rtv != ReadState.Success ) return rtv;
            if( len < -1 ) return ReadState.Fail;
            else if( len == -1 )
            {
                v = null;
                return rtv;
            }
            else if( len == 0 )
            {
                if( v != null ) v.Clear();
                else v = new Dictionary<" + ktn + @", " + vtn + @">();
                return rtv;
            }
            if( v == null ) v = new Dictionary<" + ktn + @", " + vtn + @">();
            else v.Clear();
            var keys = new List<" + ktn + @">();
            rtv = read( ref keys );
            if( rtv != ReadState.Success ) return rtv;
            if( keys.Count != len ) return ReadState.Fail;
            var vals = new List<" + vtn + @">();
            rtv = read( ref vals );
            if( rtv != ReadState.Success ) return rtv;
            if( vals.Count != len ) return ReadState.Fail;
            for( int i = 0; i < len; ++i )
            {
                v.Add( keys[ i ], vals[ i ] );
            }
            return ReadState.Success;
        }
" );
            }

            sb.Append( @"
    }
}
" );
            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_BytesPartial.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }







            sb.Clear();
            sb.Append( @"using System;
using System.Collections.Generic;
using SerialHelper;
" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                sb.Append( @"
namespace " + ns + @"
{" );
                foreach( var e in t.Enums.Where( a => a.Namespace == ns_ ) )
                {
                    sb.Append( GetComment( e.Desc, 4 ) + @"
    public enum " + e.Name + @" : " + e.GetTypeKeyword() + @"
    {" );
                    foreach( var f in e.Fields )
                    {
                        sb.Append( GetComment( f.Desc, 8 ) + @"
        " + f.Name + " = " + f.Value + @"," );
                    }
                    sb.Append( @"
    }
" );
                }

                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    sb.Append( GetComment( c.Desc, 4 ) + @"
    public partial class " + c.Name + @" : ISC
    {" );
                    foreach( var f in c.Fields )
                    {
                        sb.Append( GetComment( f.Desc, 8 ) );
                        if( f.IsArray )
                        {
                            sb.Append( @"
        public List<" + f.GetTypeKeyword() + @"> " + f.Name + ";" );// + @" = " + GetDefaultValueByType( f ) + @";" );
                        }
                        else if( f.IsDictionary )
                        {
                            sb.Append( @"
        public Dictionary<" + f.GetKeyTypeKeyword() + ", " + f.GetTypeKeyword() + @"> " + f.Name + ";" );// + @" = " + GetDefaultValueByType( f ) + @";" );
                        }
                        else
                        {
                            sb.Append( @"
        public " + f.GetTypeKeyword() + @" " + f.Name + ";" );// + @" = " + GetDefaultValueByType( f ) + @";" );
                        }
                    }
                    sb.Append( @"
    }
" );
                }

                sb.Append( @"
}
" );
            }

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_Class.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }









            sb.Clear();
            sb.Append( @"using System;
using System.Collections.Generic;
using SerialHelper;
" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                sb.Append( @"
namespace " + ns + @"
{" );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    sb.Append( @"
    partial class " + c.Name + @"
    {" );
                    foreach( var f in c.Fields )
                    {
                        if( f.IsArray )
                        {
                            sb.Append( @"
        public List<" + f.GetTypeKeyword() + @"> __prop__" + f.Name + " { get { return " + f.Name + "; } }" );
                        }
                        else if( f.IsDictionary )
                        {
                            sb.Append( @"
        public Dictionary<" + f.GetKeyTypeKeyword() + ", " + f.GetTypeKeyword() + @"> __prop__" + f.Name + " { get { return " + f.Name + "; } }" );
                        }
                        else
                        {
                            sb.Append( @"
        public " + f.GetTypeKeyword() + @" __prop__" + f.Name + " { get { return " + f.Name + "; } }" );
                        }
                    }
                    sb.Append( @"
    }
" );
                }

                sb.Append( @"
}
" );
            }

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_ClassProp.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }








            sb.Clear();
            sb.Append( @"using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using SerialHelper;
" );
            foreach( var ns_ in nss )
            {
                var ns = ( ns_ != "" ? ns_ : fn );
                sb.Append( @"
namespace " + ns + @"
{" );
                foreach( var c in t.Classes.Where( a => a.Namespace == ns_ ) )
                {
                    var en = ns.Replace( ".", "_" ) + "_" + c.Name;
                    sb.Append( @"
    partial class " + c.Name + @"
    {
        public int getOpcode()
        {
            return (int)" + fn + @"_AllClassEnums." + en + @";
        }
        public " + fn + @"_AllClassEnums getOpcodeEnum()
        {
            return " + fn + @"_AllClassEnums." + en + @";
        }

        public void writeTo( Bytes bin )
        {
            bin.write( (uint)" + c.Fields.Count + @" );" );
                    foreach( var f in c.Fields )
                    {
                        if( f.TypeClass != null && f.TypeClass.IsEnum && !f.IsArray )
                        {
                            sb.Append( @"
            bin.write( (" + ( f.TypeClass as Enum ).GetTypeKeyword() + ")this." + f.Name + @" );" );
                        }
                        else
                            sb.Append( @"
            bin.write( this." + f.Name + @" );" );
                    }
                    sb.Append( @"
        }
" );
                    if( c.Fields.Count > 0 )
                    {
                        sb.Append( @"
        public void writeTo( Bytes bin, params " + c.Name + @"_e[] ms )
        {
            if( ms.Length == " + c.Fields.Count + @" || ms.Length == 0 || ms == null )
            {
                writeTo( bin );
                return;
            }
#if DEBUG
            for( int k = 0; k < ms.Length - 1; ++k )
            {
                if( ms[ k ] > ms[ k + 1 ] ) throw new Exception( ""错误的枚举顺序: "" + ms[ k ].ToString() + ms[ k + 1 ].ToString() );
                if( ms[ k ] == ms[ k + 1 ] ) throw new Exception( ""重复的枚举项: "" + ms[ k ].ToString() );
            }
#endif
            bin.write( (uint)ms.Length );
            bin.ensure( ms.Length + 1 );
            Array.Copy( ms, 0, bin._buf, bin._len, ms.Length );
            bin._len += ms.Length;

            int i = 0;" );
                        foreach( var f in c.Fields )
                        {
                            if( f.TypeClass != null && f.TypeClass.IsEnum && !f.IsArray )
                            {
                                sb.Append( @"
            if( ms[ i ] == " + c.Name + @"_e." + f.Name + @" ) { bin.write( (" + ( f.TypeClass as Enum ).GetTypeKeyword() + ")this." + f.Name + @" ); if( ++i == ms.Length ) return; }" );
                            }
                            else
                                sb.Append( @"
            if( ms[ i ] == " + c.Name + @"_e." + f.Name + @" ) { bin.write( this." + f.Name + @" ); if( ++i == ms.Length ) return; }" );
                        }
                        sb.Append( @"
        }

        public ReadState readFrom( Bytes bin, bool[] filledMembers = null )
        {
            ReadState rtv;
            uint len = 0;
            rtv = bin.read( ref len ); if( rtv != ReadState.Success ) return rtv;
            if( len > " + c.Fields.Count + @" ) return ReadState.Fail;
            else if( len == " + c.Fields.Count + @" )
            {" );
                        foreach( var f in c.Fields )
                        {
                            if( f.TypeClass != null && f.TypeClass.IsEnum && !f.IsArray )
                            {
                                var tn = ( f.TypeNamespace != "" ? f.TypeNamespace : fn ) + "." + f.Type;
                                var e = ( f.TypeClass as Enum );
                                sb.Append( @"
                { " + e.GetTypeKeyword() + @" tmp = 0; rtv = bin.read( ref tmp ); if( rtv != ReadState.Success ) return rtv; this." + f.Name + @" = (" + tn + @")tmp; }" );
                            }
                            else
                                sb.Append( @"
                rtv = bin.read( ref this." + f.Name + @" ); if( rtv != ReadState.Success ) return rtv;" );
                        }
                        if( c.Fields.Count > 0 )
                        {
                            sb.Append( @"
                if( filledMembers != null )
                {
                    for( int j = 0; j < " + c.Fields.Count + @"; ++j ) filledMembers[ j ] = true;
                }" );
                        }
                        sb.Append( @"
            }
            else
            {
                var ms = new " + c.Name + @"_e[ len ];
                for( int j = 0; j < len; ++j )
                {
                    ms[ j ] = (" + c.Name + @"_e)bin._buf[ bin._readIdx++ ];
                }
                int i = 0;" );
                        foreach( var f in c.Fields )
                        {
                            if( f.TypeClass != null && f.TypeClass.IsEnum && !f.IsArray )
                            {
                                var e = ( f.TypeClass as Enum );
                                var tn = ( f.TypeNamespace != "" ? f.TypeNamespace : fn ) + "." + f.Type;
                                sb.Append( @"
                if( ms[ i ] == " + c.Name + @"_e." + f.Name + @" ) { " + e.GetTypeKeyword() + @" tmp = 0; rtv = bin.read( ref tmp ); if( rtv != ReadState.Success ) return rtv; else if ( filledMembers != null ) filledMembers[ (int)" + c.Name + @"_e." + f.Name + @" ] = true; this." + f.Name + @" = (" + tn + @")tmp; if( ++i == ms.Length ) return rtv; }" );
                            }
                            else
                                sb.Append( @"
                if( ms[ i ] == " + c.Name + @"_e." + f.Name + @" ) { rtv = bin.read( ref this." + f.Name + @" ); if( rtv != ReadState.Success ) return rtv; else if ( filledMembers != null ) filledMembers[ (int)" + c.Name + @"_e." + f.Name + @" ] = true; if( ++i == ms.Length ) return rtv; }" );
                        }
                        sb.Append( @"
            }
            return rtv;
        }
" );
                    }
                    else
                    {
                        sb.Append( @"
        public ReadState readFrom( Bytes bin, bool[] filledMembers = null )
        {
            ReadState rtv;
            uint len = 0;
            rtv = bin.read( ref len ); if( rtv != ReadState.Success ) return rtv;
            if( len == 0 ) return rtv;
            return ReadState.Fail;
        }
" );
                    }
                    sb.Append( @"
    }
" );
                }
                sb.Append( @"
}
" );
            }

            try
            {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_Partial.cs" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }
            System.Diagnostics.Process.Start( "explorer.exe", outputDirPath );


            return "";
        }

        public static string GetDefaultValueByType( ClassField f )
        {
            if( f.IsArray )
            {
                return "null"; // "new List<" + f.GetTypeKeyword() + ">()";
            }
            else if( f.IsDictionary )
            {
                return "null"; // "new Dictionary<" + f.GetKeyTypeKeyword() + ", " + f.GetTypeKeyword() + ">()";
            }
            else
            {
                switch( f.Type )
                {
                case "Byte[]": return "null";   // "new byte[0]";
                case "Byte":
                case "UInt8":
                case "UInt16":
                case "UInt32":
                case "UInt64":
                case "SByte":
                case "Int8":
                case "Int16":
                case "Int32":
                case "Int64":
                case "Double":
                case "Float":
                case "Single": return "0";
                case "Boolean":
                case "Bool": return "false";
                case "String": return "\"\"";
                case "DateTime": return "DateTime.MinValue";
                default:
                    if( f.TypeClass != null && f.TypeClass.IsEnum )
                    {
                        return ( f.TypeNamespace != "" ? f.TypeNamespace : _fn ) + "." + f.Type + "." + ( (Enum)f.TypeClass ).Fields.First().Name;
                    }
                    else
                        return "null"; // "new " + ( f.TypeNamespace != "" ? f.TypeNamespace : _fn ) + "." + f.Type + "()";
                }
            }
        }

        public static string GetComment( string s, int space )
        {
            if( s.Trim() == "" ) return "";
            var sps = new string( ' ', space );
            return "\r\n" +
sps + @"/// <summary>
" + sps + @"/// " + s + @"
" + sps + @"/// </summary>";
        }

        public static string GetTypeKeyword( this Enum c )
        {
            switch( c.Type )
            {
            case "Byte": return "byte";
            case "Byte[]": return "byte[]";
            case "SByte": return "sbyte";
            case "UInt16": return "ushort";
            case "Int16": return "short";
            case "UInt32": return "uint";
            case "Int32": return "int";
            case "UInt64": return "ulong";
            case "Int64": return "long";
            }
            throw new Exception( "it can't be" );
        }
        public static string GetTypeKeyword( this ClassField f )
        {
            switch( f.Type )
            {
            case "Byte": return "byte";
            case "Byte[]": return "byte[]";
            case "SByte": return "sbyte";
            case "UInt16": return "ushort";
            case "Int16": return "short";
            case "UInt32": return "uint";
            case "Int32": return "int";
            case "UInt64": return "ulong";
            case "Int64": return "long";
            case "Double": return "double";
            case "Single": return "float";
            case "Boolean": return "bool";
            case "String": return "string";
            case "DateTime": return "DateTime";
            default:
                if( f.TypeClass != null && f.TypeClass.IsEnum )
                {
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _fn ) + "." + f.Type;
                }
                else
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _fn ) + "." + f.Type;
            }
        }
        public static string GetKeyTypeKeyword( this ClassField f )
        {
            switch( f.KeyType )
            {
            case "Byte": return "byte";
            case "Byte[]": return "byte[]";
            case "SByte": return "sbyte";
            case "UInt16": return "ushort";
            case "Int16": return "short";
            case "UInt32": return "uint";
            case "Int32": return "int";
            case "UInt64": return "ulong";
            case "Int64": return "long";
            case "Double": return "double";
            case "Single": return "float";
            case "Boolean": return "bool";
            case "String": return "string";
            case "DateTime": return "DateTime";
            default:
                if( f.KeyTypeClass != null && f.KeyTypeClass.IsEnum )
                {
                    return ( f.KeyTypeNamespace != "" ? f.KeyTypeNamespace : _fn ) + "." + f.KeyType;
                }
                else
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _fn ) + "." + f.Type;
            }
        }

    }
}
