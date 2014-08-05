using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.IO;
using System.Windows.Forms;
using LIB = PacketLibrary;

namespace PacketGenerator
{
    public static class Helpers
    {
        public static Template GetTemplate( Assembly asm )
        {
            var template = new Template();
            var libNS = "PacketLibrary";            // 重要：这个字串需要保持与模板 Lib 名一致
            var libNSdot = libNS + ".";
            var projEnum = "__projects";            // 重要：生成过程中通过这个枚举来识别项目分类

            //// 扫项目列表
            //var r_enums = from t in asm.GetTypes() where ( t.IsEnum ) && t.Namespace != libNS && t.Name == projEnum select t;
            //if( r_enums.Count() > 0 )
            //{
            //    var e = r_enums.First();
            //    var r_fields = e.GetFields( BindingFlags.Static | BindingFlags.Public );
            //    foreach( var r_field in r_fields )
            //    {
            //        var p = new Project { Name = r_field.Name };
            //        foreach( var a in r_field.GetCustomAttributes( false ) )
            //        {
            //            if( a is LIB.Desc ) p.Desc = ( (LIB.Desc)a ).Value;
            //        }
            //        template.Projects.Add( p );
            //    }
            //}

            // 扫枚举
            var r_enums = from t in asm.GetTypes() where ( t.IsEnum ) && t.Namespace != libNS && t.Name != projEnum select t;
            foreach( var r_enum in r_enums )
            {
                var e = new Enum();
                e.IsEnum = true;
                e.Name = r_enum.Name.ToString();
                e.Namespace = r_enum.Namespace ?? "";
                foreach( var r_attribute in r_enum.GetCustomAttributes( false ) )
                {
                    if( r_attribute is LIB.Desc ) e.Desc = ( (LIB.Desc)r_attribute ).Value;
                    // more class attributes
                }

                var ut = r_enum.GetEnumUnderlyingType();
                e.Type = ut.Name;
                switch( ut.Name )
                {
                case "Byte": e.Size = 1; e.Unsigned = true; break;
                case "SByte": e.Size = 1; e.Unsigned = false; break;
                case "UInt16": e.Size = 2; e.Unsigned = true; break;
                case "Int16": e.Size = 2; e.Unsigned = false; break;
                case "UInt32": e.Size = 4; e.Unsigned = true; break;
                case "Int32": e.Size = 4; e.Unsigned = false; break;
                case "UInt64": e.Size = 8; e.Unsigned = true; break;
                case "Int64": e.Size = 8; e.Unsigned = false; break;
                }
                var r_fields = r_enum.GetFields( BindingFlags.Static | BindingFlags.Public );
                foreach( var r_field in r_fields )
                {
                    var ef = new EnumField();
                    ef.Enum = e;
                    e.Fields.Add( ef );
                    foreach( var a in r_field.GetCustomAttributes( false ) )
                    {
                        if( a is LIB.Desc ) ef.Desc = ( (LIB.Desc)a ).Value;
                        // more class attributes
                    }
                    ef.Name = r_field.Name;
                    ef.Value = r_field.GetValue( null ).ToIntegerString( ut );
                }
                template.Enums.Add( e );
            }

            // 扫类
            var r_classes = from t in asm.GetTypes() where ( t.IsClass || t.IsValueType && !t.IsEnum ) && t.Namespace != libNS select t;
            foreach( var r_class in r_classes )
            {
                var c = new Class();
                c.IsEnum = false;
                c.Name = r_class.Name.ToString();
                c.Namespace = r_class.Namespace ?? "";
                var cis = r_class.GetInterfaces();
                foreach( var r_attribute in r_class.GetCustomAttributes( false ) )
                {
                    if( r_attribute is LIB.Desc ) c.Desc = ( (LIB.Desc)r_attribute ).Value;
                    //else if( r_attribute is LIB.Decode ) c.Decode.AddRange( ( (LIB.Decode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                    //else if( r_attribute is LIB.Encode ) c.Encode.AddRange( ( (LIB.Encode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                    //// more class attributes
                }
                template.Classes.Add( c );

                var r_fields = r_class.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ).ToList();
                if( r_class.BaseType != typeof( object ) )
                {
                    r_fields.InsertRange( 0, r_class.BaseType.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ) );
                }

                foreach( var r_field in r_fields )
                {
                    var f = new ClassField { Class = c };
                    f.Name = r_field.Name;
                    c.Fields.Add( f );

                    var ft = r_field.FieldType;
                    if( ft.IsArray ) f.Declare.IsArray = true;
                    if( ft.IsGenericType )
                    {
                        if( ft.Namespace == libNS )
                            throw new Exception( "unknown data type." );
                        f.Declare.DataType = DataTypes.Generic;
                        // fill Childs.....
                        
                    }
                    else if( ft.Namespace == "System" )
                    {
                        switch( ft.Name )
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
                            f.Declare.DataType = DataTypes.BuiltIn;
                            f.Declare.Name = ft.Name;
                            f.Declare.Namespace = "";
                        default:
                            throw new Exception( "unknown data type." );
                        }
                    }
                    else if( ft.Namespace == libNS )
                    {
                        // class or enum
                        f.Declare.DataType = DataTypes.Custom;
                        f.Declare.Name = ft.Name;
                        f.Declare.Namespace = libNS;

                    }
                    //if( f.Declare.Namespace == libNS )
                    //{
                    //    if( f.Declare.DataType == DataTypes.BuiltIn )
                    //    {
                    //        f.Declare.Namespace = "System";
                    //    }
                    //}

                }
                for( int fidx = 0; fidx < c.Fields.Count; ++fidx )
                {
                    var r_field = r_fields[ fidx ];
                    var f = c.Fields[ fidx ];
                    foreach( var r_attribute in r_field.GetCustomAttributes( false ) )
                    {
                        if( r_attribute is LIB.Desc ) f.Desc = ( (LIB.Desc)r_attribute ).Value;
                        else if( r_attribute is LIB.Default ) f.Default = ( (LIB.Default)r_attribute ).Value;
                        else if( r_attribute is LIB.Get ) f.Get = ( (LIB.Get)r_attribute ).Value;
                        else if( r_attribute is LIB.Get ) f.Get = ( (LIB.Get)r_attribute ).Value;
                        else if( r_attribute is LIB.Limit ) { f.Declare.MinLen = ( (LIB.Limit)r_attribute ).Min; f.Declare.MaxLen = ( (LIB.Limit)r_attribute ).Max; }
                        else if( r_attribute is LIB.Condation )
                        {
                            var ps = ( (LIB.Condation)r_attribute ).Value;
                            for( int i=0; i < ps.Length; i += 2 )
                            {
                                // todo: 检查如果被引用的 fields 位于当前 field 的后方，条件非法
                                f.Condation.Add( c.Fields.Find( a => a.Name == (string)ps[ i ] ), ps[ i + 1 ] );
                            }
                        }
                        //else if( r_attribute is LIB.Decode ) c.Decode.AddRange( ( (LIB.Decode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                        //else if( r_attribute is LIB.Encode ) c.Encode.AddRange( ( (LIB.Encode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                        //// more field attributes
                    }


                    //else if( r_field.FieldType.Name.StartsWith( "List`1" ) )
                    //{
                    //    f.IsArray = true;
                    //    f.Type = r_field.FieldType.FullName.Replace( libNSdot, "" ).Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ].Replace( "List`1[[", "" );
                    //}
                    //else if( r_field.FieldType.Name.StartsWith( "Dict`2" ) )
                    //{
                    //    // PacketLibrary.Dict`2[[Enum1, PacketTemplate_Test, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null],[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]]
                    //    //f.IsDictionary = true;
                    //    //var ss = r_field.FieldType.FullName.Replace( libNSdot + "Dict`2[[", "" ).Replace( "]]", "" ).Split( new string[] { "],[" }, StringSplitOptions.None );
                    //    //f.KeyType = ss[ 0 ].Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ];
                    //    //f.Type = ss[ 1 ].Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ];
                    //}
                    //else
                    //{
                    //    f.Type = r_field.FieldType.Name;
                    //    f.TypeNamespace = r_field.FieldType.Namespace ?? "";
                    //}

                    //if( f.Type.Contains( '.' ) )
                    //{
                    //    f.TypeNamespace = f.Type.Substring( 0, f.Type.LastIndexOf( '.' ) );
                    //    f.Type = f.Type.Substring( f.Type.LastIndexOf( '.' ) + 1 );
                    //}



                    //if( f.IsDictionary )
                    //{
                    //    if( f.KeyType.Contains( '.' ) )
                    //    {
                    //        f.KeyTypeNamespace = f.KeyType.Substring( 0, f.KeyType.LastIndexOf( '.' ) );
                    //        f.KeyType = f.KeyType.Substring( f.KeyType.LastIndexOf( '.' ) + 1 );
                    //    }
                    //    if( f.KeyTypeNamespace == libNS )
                    //    {
                    //        f.KeyTypeNamespace = "System";
                    //    }
                    //}

                    // todo: check List
                    //if( f.FieldType.Name.StartsWith( "Nullable`1" ) )
                    //{
                    //    fps.Nullable = true;
                    //    fps.Type = f.FieldType.FullName.Replace( libNSdot, "" ).Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ].Replace( "Nullable`1[[", "" );
                    //}
                    //else
                    //{
                    //    fps.Type = f.FieldType.FullName.Replace( "[]", "" ).Replace( libNSdot, "" );
                    //}
                    //if( fps.Type.Contains( '.' ) )
                    //{
                    //    fps.TypeNamespace = fps.Type.Substring( 0, fps.Type.LastIndexOf( '.' ) );
                    //    fps.Type = fps.Type.Substring( fps.Type.LastIndexOf( '.' ) + 1 );
                    //}
                }
            }

            // todo: 所有 Binary, String, WString, List<T> 都应该做长度限制, 不限就应该警告

            //// 整理引用关系, 调整 Encode, Decode
            //foreach( var c in template.Classes )
            //{
            //    // todo
            //}

            //// 整理 enable 属性( 合并 encode & decode )
            //foreach( var c in template.Classes )
            //{
            //    if( c.Encode.Count + c.Decode.Count == 0 )
            //        c.Enable = c.Encode = c.Decode = template.Projects;
            //    else
            //        c.Enable = c.Encode.Concat( c.Decode ).Distinct().ToList();
            //}

            // 自增 type id
            ushort tid = 0;

            // 整理数据类型
            foreach( var c in template.Classes )
            {
                foreach( var f in c.Fields )
                {
                    if( f.TypeNamespace == "System" )
                    {
                        if( !ValidateSystemDataType( f.Type ) )
                        {
                            throw new Exception( "unhandled type: " + f.TypeNamespace + "." + f.Type + " " + c.Name + "." + f.Name );
                        }
                    }
                    else
                    {
                        if( template.Enums.Exists( a => a.Namespace == f.TypeNamespace && a.Name == f.Type ) )
                        {
                            f.TypeClass = template.Enums.First( a => a.Namespace == f.TypeNamespace && a.Name == f.Type );
                        }
                        else if( template.Classes.Exists( a => a.Namespace == f.TypeNamespace && a.Name == f.Type ) )
                        {
                            f.TypeClass = template.Classes.First( a => a.Namespace == f.TypeNamespace && a.Name == f.Type );
                        }
                        else
                        {
                            throw new Exception( "unhandled type: " + f.TypeNamespace + "." + f.Type + " " + c.Name + "." + f.Name );
                        }
                    }
                    //if( f.IsDictionary )
                    //{
                    //    if( f.KeyTypeNamespace == "System" )
                    //    {
                    //        if( !ValidateSystemDataType( f.KeyType ) )
                    //        {
                    //            throw new Exception( "unhandled type: " + f.KeyTypeNamespace + "." + f.KeyType + " " + c.Name + "." + f.Name );
                    //        }
                    //    }
                    //    else
                    //    {
                    //        if( template.Enums.Exists( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType ) )
                    //        {
                    //            f.KeyTypeClass = template.Enums.First( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType );
                    //        }
                    //        else if( template.Classes.Exists( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType ) )
                    //        {
                    //            f.KeyTypeClass = template.Classes.First( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType );
                    //        }
                    //        else
                    //        {
                    //            throw new Exception( "unhandled key type: " + f.KeyTypeNamespace + "." + f.KeyType + " " + c.Name + "." + f.Name );
                    //        }
                    //    }
                    //}

                }

                // 填充自增 TypeID
                c.TypeID = tid++;
            }

            // 整理命名空间
            template.Namespaces = template.Classes.Select( a => a.Namespace ).Concat( template.Enums.Select( a => a.Namespace ) ).Distinct().ToList();




            return template;
        }

        public static bool ValidateSystemDataType( string t )
        {
            switch( t )
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
            case "String":
            case "Byte[]":
                return true;
            }
            return false;
        }

        // 转枚举值专用
        public static string ToIntegerString( this object enumValue, Type ut )
        {
            switch( ut.Name )
            {
            case "Byte": return Convert.ToByte( enumValue ).ToString();
            case "SByte": return Convert.ToSByte( enumValue ).ToString();
            case "UInt16": return Convert.ToUInt16( enumValue ).ToString();
            case "Int16": return Convert.ToInt16( enumValue ).ToString();
            case "UInt32": return Convert.ToUInt32( enumValue ).ToString();
            case "Int32": return Convert.ToInt32( enumValue ).ToString();
            case "UInt64": return Convert.ToUInt64( enumValue ).ToString();
            case "Int64": return Convert.ToInt64( enumValue ).ToString();
            }
            throw new Exception( "unknown Underlying Type" );
        }


        public static string ToFirstUpper( this string s )
        {
            if( s.Length <= 1 ) return s.ToUpper();
            return s.Substring( 0, 1 ).ToUpper() + s.Substring( 1 );
        }

        public static string ToFirstLower( this string s )
        {
            if( s.Length <= 1 ) return s.ToLower();
            return s.Substring( 0, 1 ).ToLower() + s.Substring( 1 );
        }



    }
}

//else if( a is Enabled ) fps.Enabled = ( (Enabled)a ).Value;
//else if( a is DecodeCondation ) fps.DecodeCondation = (DecodeCondation)a;
//else if( a is EncodeCondation ) fps.EncodeCondation = (EncodeCondation)a;
// "ChkValue": fps.MinValue = ((ChkValue)a).Min; fps.MinValue = ((ChkValue)a).Max; break;

// 扫空 interface（用于 Decode, Encode 的参数列表）
//var interfaces = from t in asm.GetTypes() where t.IsInterface && t.Namespace != libNS select t;
//foreach( var i in interfaces )
//{
//    var p = new Project { Name = i.Name };
//    foreach( var a in i.GetCustomAttributes( false ) )
//    {
//        if( a is Desc ) p.Desc = ( (Desc)a ).Value;
//        // more proj attributes
//    }
//    template.Projects.Add( p );
//}
//else if( a is Enabled ) cps.Enabled = ( (Enabled)a ).Value;
//else if( a is Opcode ) cps.Opcode = ( (Opcode)a ).Value;
//else if( a is Returns ) cps.Results = ( (Returns)a ).Value.ToList();
//else if( a is CallBy ) cps.CallBy = ( (CallBy)a ).Value.ToList();
