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
        public static string libNS = "PacketLibrary";            // 重要：这个字串需要保持与模板 Lib 名一致
        public static string libNSdot = libNS + ".";
        public static string projEnum = "__projects";            // 重要：生成过程中通过这个枚举来识别项目分类

        public static Template GetTemplate( Assembly asm )
        {
            var template = new Template();

            // 扫项目列表
            var r_enums = from t in asm.GetTypes() where ( t.IsEnum ) && t.Namespace != libNS && t.Name == projEnum select t;
            if( r_enums.Count() > 0 )
            {
                var e = r_enums.First();
                var r_fields = e.GetFields( BindingFlags.Static | BindingFlags.Public );
                foreach( var r_field in r_fields )
                {
                    var p = new Project { Name = r_field.Name };
                    foreach( var a in r_field.GetCustomAttributes( false ) )
                    {
                        if( a is LIB.Desc ) p.Desc = ( (LIB.Desc)a ).Value;
                    }
                    template.Projects.Add( p );
                }
            }

            // 扫枚举
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
                c.Name = r_class.Name;
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
            }

            // 自增 type id
            ushort tid = 0;
            foreach( var c in template.Classes )
            {
                // 填充自增 TypeID
                c.TypeID = tid++;
            }

            // 继续扫类
            foreach( var r_class in r_classes )
            {
                var c = template.Classes.Find( a => a.Name == r_class.Name && a.Namespace == ( r_class.Namespace ?? "" ) );

                // 扫继承字段
                var r_fields = r_class.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ).ToList();
                if( r_class.BaseType != typeof( object ) )
                {
                    r_fields.InsertRange( 0, r_class.BaseType.GetFields( BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance ) );
                }

                // 扫字段
                foreach( var r_field in r_fields )
                {
                    var f = new ClassField { Class = c };
                    f.Name = r_field.Name;
                    c.Fields.Add( f );
                    fillDeclare( template.Classes, f.Declare, r_field.FieldType );
                }

                // 继续扫字段
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
                        else if( r_attribute is LIB.Limits ) { fillDeclareLimits( f.Declare, (LIB.Limits)r_attribute ); }
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
                }
            }

            // todo: 所有容器都应该做长度限制, 不限就应该警告

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


            // 整理命名空间
            template.Namespaces = template.Classes.Select( a => a.Namespace ).Concat( template.Enums.Select( a => a.Namespace ) ).Distinct().ToList();

            return template;
        }

        public static void fillDeclareLimits( Declare d, LIB.Limits ls, int i = 0 )
        {
            if( d.DataType == DataTypes.Array )
            {
                if( i >= ls.Value.Length )
                {
                    throw new Exception( "the Limits is not enough length." );
                }
                d.MinLen = ls.Value[ i++ ];
                fillDeclareLimits( d.Childs[ 0 ], ls, i );
            }
            else if( d.DataType == DataTypes.Generic )
            {
                if( i >= ls.Value.Length )
                {
                    throw new Exception( "the Limits is not enough length." );
                }
                d.MinLen = ls.Value[ i++ ];
                if( i >= ls.Value.Length )
                {
                    throw new Exception( "the Limits is not enough length." );
                }
                d.MaxLen = ls.Value[ i++ ];
                foreach( var cd in d.Childs )
                {
                    fillDeclareLimits( cd, ls, i );
                }
            }
        }

        public static void fillDeclare( List<Class> cs, Declare d, Type t )
        {
            var tn = t.Name;
            if( t.IsArray )
            {
                d.DataType = DataTypes.Array;
                d.Namespace = "System";
                d.Name = "[]";
                var cd = new Declare();
                d.Childs.Add( cd );
                fillDeclare( cs, cd, t.GetElementType() );
            }
            else if( t.IsGenericType )
            {
                if( t.Namespace != libNS )
                    throw new Exception( "unknown data type." );
                d.DataType = DataTypes.Generic;
                d.Name = tn.Substring( 0, tn.LastIndexOf('`') );
                d.Namespace = "";
                foreach( var ct in t.GenericTypeArguments )
                {
                    var cd = new Declare();
                    d.Childs.Add( cd );
                    fillDeclare( cs, cd, ct );
                }
            }
            else if( t.Namespace == "System" )
            {
                switch( t.Name )
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
                    d.DataType = DataTypes.BuiltIn;
                    d.Name = t.Name;
                    d.Namespace = "";
                    break;
                default:
                    throw new Exception( "unknown data type." );
                }
            }
            else
            {
                if( t.Namespace != null )   //  && t.Namespace != libNS
                {
                    throw new Exception( "unknown data type." );
                }
                d.DataType = DataTypes.Custom;
                d.Name = t.Name;
                d.Namespace = "";
                d.Class = cs.Find( a => a.Name == t.Name && a.Namespace == "" );
            }
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
