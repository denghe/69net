using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    public class GenLua
    {
//        public static string GetNetPacketReadTypeName( string tn ) {
//            switch( tn ) {
//                case "Byte": return "i1";
//                case "UInt16": return "u2";
//                case "UInt32": return "u4";
//                case "UInt64": return "u8";
//                case "SByte": return "u1";
//                case "Int16": return "i2";
//                case "Int32": return "i4";
//                case "Int64": return "i8";
//                case "Double": return "d";
//                case "Float": return "f";
//                case "Boolean": return "b";
//                case "Bytes": return "bs";
//                case "String": return "s";
//                case "UInt8": return "u1";
//                case "Int8": return "i1";
//                case "Single": return "f";
//                case "Bool": return "b";
//            }
//            throw new Exception( "unknown type" );
//        }

//        public static string GetDesc( string desc, int spaces ) {
//            return desc == "" ? "" : ( new string( ' ', spaces ) + "-- " + desc + "\r\n" );
//        }


//        public static string GetDecodeCondationExpression( PacketLibrary.DecodeCondation dc ) {
//            string op = "";
//            switch( dc.Operate ) {
//                case PacketLibrary.Operators.Equal: op = "=="; break;
//                case PacketLibrary.Operators.NotEqual: op = "~="; break;
//                case PacketLibrary.Operators.LargerEqual: op = ">="; break;
//                case PacketLibrary.Operators.LargerThan: op = ">"; break;
//                case PacketLibrary.Operators.SmallerEqual: op = "<="; break;
//                case PacketLibrary.Operators.SmallerThan: op = "<"; break;
//            }
//            return dc.Key + " " + op + " " + dc.Value;
//        }

//        public static string Gen( Template sd, string outputDirPath, string fn ) {

//            var sb = new StringBuilder();

//            // todo: 当前未区分 sd.Projects
//            // todo: 当前未校验数组类对象长度
//            // todo: 当前未支持数组类数据类型
//            // todo: 当前未实现 SeeAlso, Returns, CallBy


//            // 创建 opcode 查表

//            sb.Append( @"
//
//-- opcode 表
//opcodes = {}
//
//" );

//            foreach( var c in sd.Classes.Where( a => a.Enabled && ( a.Encode || a.Decode ) ) ) {
//                var desc = GetDesc( c.Desc, 0 );
//                sb.WriteFormat( desc + @"opcodes." + c.Name + @" = " + c.Opcode + "\r\n" );
//            }





//            sb.Append( @"
//-- 解码函数表，其 key 为 opcode
//decoders = {}
//
//" );

//            // 创建解码函数

//            foreach( var c in sd.Classes.Where( a => a.Enabled && a.Decode ) ) {
//                var desc = GetDesc( c.Desc, 0 );
//                sb.WriteFormat( @"" + desc + @"decoders[ opcodes." + c.Name + @" ] = function( p )
//    local t = {}" );
//                foreach( var f in c.Fields ) {
//                    desc = GetDesc( f.Desc, 4 );
//                    if( f.DecodeCondation.Key != "" )
//                        sb.WriteFormat( @"
//%2    if t.%3 then
//        t.%1 = p:dr%0()
//    else
//        t.%1 = nil
//    end"
//                    , GetNetPacketReadTypeName( f.Type )
//                    , f.Name
//                    , desc
//                    , GetDecodeCondationExpression( f.DecodeCondation )
//     );
//                    else
//                        sb.WriteFormat( @"
//%2    t.%1 = p:dr%0()"
//                    , GetNetPacketReadTypeName( f.Type )
//                    , f.Name
//                    , desc
//     );
//                }
//                sb.Append( @"
//    return t
//end
//
//" );
//            }



            
//            // 创建编码函数( 传参版 )

//            sb.Append( @"
//
//
//
//
//
//-- 编码函数表( 传参版 )
//encoders = {}
//
//" );

//            foreach( var c in sd.Classes.Where( a => a.Enabled && a.Encode ) ) {
//                var desc = GetDesc( c.Desc, 0 );
//                sb.WriteFormat( desc + @"encoders." + c.Name + @" = function( p" );
//                foreach( var f in c.Fields ) {
//                    desc = GetDesc( f.Desc, 4 );
//                    sb.WriteFormat( @"
//" + desc + "    , p_" + f.Name );
//                }

//                sb.WriteFormat( @" )
//
//    p:clear()
//    p.opcode = opcodes." + c.Name + @"
//" );
//                foreach( var f in c.Fields ) {
//                    desc = GetDesc( f.Desc, 4 );
//                    sb.WriteFormat( @"
//    p:dw%0( p_%1 )"
//                    , GetNetPacketReadTypeName( f.Type )
//                    , f.Name
//     );
//                }
//                sb.Append( @"
//end
//
//" );
//            }















//            try {
//                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_packets.lua" ), false );
//            }
//            catch( Exception ex ) {
//                return ex.Message;
//            }

//            return "";
//        }

    }
}
