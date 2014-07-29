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
                // 从子类中先把父类属性移除，再插入到前面，以确保父类属性在前
                var r_fields = r_class.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ).ToList();
                if( r_class.BaseType != typeof( object ) )
                {
                    r_fields.InsertRange( 0, r_class.BaseType.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ) );

                    //var r_parent_field_names = r_class.BaseType.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ).Select( a => a.Name ).ToList();
                    //var tmp = new List<FieldInfo>();
                    //while( r_fields.Any( a => r_parent_field_names.Contains( a.Name ) ) )
                    //{
                    //    var o = r_fields.First( a => r_parent_field_names.Contains( a.Name ) );
                    //    tmp.Add( o );
                    //    r_fields.Remove( o );
                    //}
                    //for( int i = 0; i < tmp.Count; i++ )
                    //{
                    //    r_fields.Insert( i, tmp[ i ] );
                    //}
                }
                foreach( var r_field in r_fields )
                {
                    var f = new ClassField { Class = c };
                    c.Fields.Add( f );
                    foreach( var r_attribute in r_field.GetCustomAttributes( false ) )
                    {
                        if( r_attribute is LIB.Desc ) f.Desc = ( (LIB.Desc)r_attribute ).Value;
                        //else if( r_attribute is LIB.Decode ) c.Decode.AddRange( ( (LIB.Decode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                        //else if( r_attribute is LIB.Encode ) c.Encode.AddRange( ( (LIB.Encode)r_attribute ).Value.Select( o => template.Projects.FirstOrDefault( oo => oo.Name == o.ToString() ) ) );
                        //// more field attributes
                    }
                    f.Name = r_field.Name;
                    if( r_field.FieldType.Name.LastIndexOf( "[]" ) == r_field.FieldType.Name.Length - 2 && r_field.FieldType.Name!= "Byte[]" )
                    {
                        f.IsArray = true;
                        f.Type = r_field.FieldType.FullName.Substring( 0, r_field.FieldType.FullName.Length - 2 ).Replace( libNSdot, "" );
                    }
                    //else if( r_field.FieldType.Name.StartsWith( "List`1" ) )
                    //{
                    //    f.IsArray = true;
                    //    f.Type = r_field.FieldType.FullName.Replace( libNSdot, "" ).Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ].Replace( "List`1[[", "" );
                    //}
                    else if( r_field.FieldType.Name.StartsWith( "Dict`2" ) )
                    {
                        // PacketLibrary.Dict`2[[Enum1, PacketTemplate_Test, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null],[System.String, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]]
                        f.IsDictionary = true;
                        var ss = r_field.FieldType.FullName.Replace( libNSdot + "Dict`2[[", "" ).Replace( "]]", "" ).Split( new string[] { "],[" }, StringSplitOptions.None );
                        f.KeyType = ss[ 0 ].Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ];
                        f.Type = ss[ 1 ].Split( new string[] { ", " }, StringSplitOptions.None )[ 0 ];
                    }
                    else
                    {
                        f.Type = r_field.FieldType.Name;
                        f.TypeNamespace = r_field.FieldType.Namespace ?? "";
                    }

                    if( f.Type.Contains( '.' ) )
                    {
                        f.TypeNamespace = f.Type.Substring( 0, f.Type.LastIndexOf( '.' ) );
                        f.Type = f.Type.Substring( f.Type.LastIndexOf( '.' ) + 1 );
                    }
                    if( f.TypeNamespace == libNS )
                    {
                        f.TypeNamespace = "System";
                    }

                    if( f.IsDictionary )
                    {
                        if( f.KeyType.Contains( '.' ) )
                        {
                            f.KeyTypeNamespace = f.KeyType.Substring( 0, f.KeyType.LastIndexOf( '.' ) );
                            f.KeyType = f.KeyType.Substring( f.KeyType.LastIndexOf( '.' ) + 1 );
                        }
                        if( f.KeyTypeNamespace == libNS )
                        {
                            f.KeyTypeNamespace = "System";
                        }
                    }

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
                    if( f.IsDictionary )
                    {
                        if( f.KeyTypeNamespace == "System" )
                        {
                            if( !ValidateSystemDataType( f.KeyType ) )
                            {
                                throw new Exception( "unhandled type: " + f.KeyTypeNamespace + "." + f.KeyType + " " + c.Name + "." + f.Name );
                            }
                        }
                        else
                        {
                            if( template.Enums.Exists( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType ) )
                            {
                                f.KeyTypeClass = template.Enums.First( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType );
                            }
                            else if( template.Classes.Exists( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType ) )
                            {
                                f.KeyTypeClass = template.Classes.First( a => a.Namespace == f.KeyTypeNamespace && a.Name == f.KeyType );
                            }
                            else
                            {
                                throw new Exception( "unhandled key type: " + f.KeyTypeNamespace + "." + f.KeyType + " " + c.Name + "." + f.Name );
                            }
                        }
                    }

                }
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
                case "Byte[]":
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
                case "DateTime": return true;
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

    }
}

//else if( a is Enabled ) fps.Enabled = ( (Enabled)a ).Value;
//else if( a is Limit ) { fps.MinLen = ( (Limit)a ).Min; fps.MaxLen = ( (Limit)a ).Max; }
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
