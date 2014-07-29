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
            var sb = new StringBuilder();




            try
            {
                sb.WriteToFile( Path.Combine( outDir, pn + ".h" ) );
            }
            catch( Exception ex )
            {
                return ex.Message;
            }

            sb.Clear();





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

        // 项目名上下文，省掉下面的传参了
        public static string _pn;


        // 把 Namespace 啥中间的 . 转为 ::
        public static void ConvertNamespace( Template sd, string baseNS = "" )
        {
            foreach( var c in sd.Classes )
            {
                /*if (c.Namespace == "") c.Namespace = "::" + baseNS;
                else */
                c.Namespace = c.Namespace.Replace( ".", "::" );
                foreach( var f in c.Fields )
                {
                    if( f.TypeNamespace == "" ) continue;
                    f.TypeNamespace = ( baseNS == "" ? "" : ( "::" + baseNS + "::" ) ) + f.TypeNamespace.Replace( ".", "::" );
                }
            }
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
                        return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type + "." + ( (Enum)f.TypeClass ).Fields.First().Name;
                    }
                    else
                        return "null"; // "new " + ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type + "()";
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

        public static string GetTypeKeyword( Enum c )
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
        public static string GetTypeKeyword( ClassField f )
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
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type;
                }
                else
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type;
            }
        }
        public static string GetKeyTypeKeyword( ClassField f )
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
                    return ( f.KeyTypeNamespace != "" ? f.KeyTypeNamespace : _pn ) + "." + f.KeyType;
                }
                else
                    return ( f.TypeNamespace != "" ? f.TypeNamespace : _pn ) + "." + f.Type;
            }
        }

    }
}
