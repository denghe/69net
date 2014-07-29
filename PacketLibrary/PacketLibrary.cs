/***********************************************************************************/
// 关键字
/***********************************************************************************/
// enum
// class

/***********************************************************************************/
// 数据类型：
/***********************************************************************************/
// byte  
// ushort
// uint  
// ulong 
// sbyte 
// short 
// int   
// long  
// double
// float 
// bool

/***********************************************************************************/
// 集合( 可用 Limit 来限定元素个数的合法范围 )
/***********************************************************************************/
// []                   // 必须用 Limit 标记限长
// string
// List<V>
// Dict<K, V>

/***********************************************************************************/
// 输出
/***********************************************************************************/
// c++ 对应   |  c# 对应               |  lua 类型对应
//------------+------------------------+------------------------
// uint8      |  Byte     或  byte     | double
// uint16     |  UInt16   或  ushort   | double
// uint32     |  UInt32   或  uint     | double
// uint64     |  UInt64   或  ulong    | double
// int8       |  SByte    或  sbyte    | double
// int16      |  Int16    或  short    | double
// int32      |  Int32    或  int      | double
// int64      |  Int64    或  long     | double
// double     |  Double   或  double   | double
// single     |  Single   或  float    | double
// bool       |  Boolean  或  bool     | bool
// []         |  []                    | table
// String     |  String   或  string   | string
// List       |  List                  | table
// Dict       |  Dictionary            | table

namespace PacketLibrary
{

    public class List<V> { }
    public class Dict<K, V> { }

    /// <summary>
    /// 备注。可用于类/枚举 及其 成员
    /// </summary>
    public class Desc : System.Attribute
    {
        public Desc( System.String v ) { Value = v; }
        public System.String Value { get; set; }
    }

    /// <summary>
    /// 针对最外层级的 数组, byte[], string 做长度限制。单个长度值为定长
    /// </summary>
    public class Limit : System.Attribute
    {
        public Limit( System.UInt32 fix )
        {
            Min = fix;
            Max = fix;
        }
        public Limit( System.UInt32 min, System.UInt32 max )
        {
            if( max < min ) throw new System.Exception( "error: max < min" );
            Min = min;
            Max = max;
        }
        public System.UInt32 Min { get; set; }
        public System.UInt32 Max { get; set; }
    }

}



/***********************************************************************************/
// 下面的东西当前未启用, 也不完整. 先放这，待整理
/***********************************************************************************/


///// <summary>
///// 设置类于项目的编码需求
///// </summary>
//public class Encode : System.Attribute
//{
//    public Encode( params object[] ps ) { Value = ps; }
//    public object[] Value { get; set; }
//}


///// <summary>
///// 设置类于项目的解码需求
///// </summary>
//public class Decode : System.Attribute
//{
//    public Decode( params object[] ps ) { Value = ps; }
//    public object[] Value { get; set; }
//}


///// <summary>
///// 消息的返回结果列表。可用于类
///// </summary>
//public class Returns : System.Attribute
//{
//    public Returns( params  System.Type[] v ) { Value = v; }
//    public System.Type[] Value { get; set; }
//}


///// <summary>
///// 经由哪些消息所引发，可能返回该条消息。可用于类
///// </summary>
//public class CallBy : System.Attribute
//{
//    public CallBy( params  System.Type[] v ) { Value = v; }
//    public System.Type[] Value { get; set; }
//}


///// <summary>
///// 是否启用。默认为启用。可用于类和属性
///// </summary>
//public class Enable : System.Attribute
//{
//    public Enabled( System.Boolean v ) { Value = v; }
//    public System.Boolean Value { get; set; }
//}


///// <summary>
///// 简单设置相关性（可用于生成链接文档）
///// </summary>
//public class SeeAlso : System.Attribute
//{
//    public SeeAlso( params  System.String[] v ) { Value = v; }
//    public System.String[] Value { get; set; }
//}


///// <summary>
///// 针对 Bytes, String，整个包 做长度 check
///// </summary>
//public class Limit : System.Attribute
//{
//    public Limit( System.UInt32 fix ) {
//        Min = fix;
//        Max = fix;
//    }
//    public Limit( System.UInt32 min, System.UInt32 max ) {
//        if( max < min )
//            throw new System.Exception( "error: max < min" );
//        Min = min;
//        Max = max;
//    }
//    public System.UInt32 Min { get; set; }
//    public System.UInt32 Max { get; set; }
//}


//public enum Operators {
//    Equal,
//    NotEqual,
//    LargerEqual,
//    SmallerEqual,
//    LargerThan,
//    SmallerThan,
//}


///// <summary>
///// 解码条件限制（生成类似 if ( 某属性名key == 值v ) 解码当前属性;
///// </summary>
//public class DecodeCondation : System.Attribute
//{
//    public DecodeCondation( System.String k, Operators o, System.Object v ) { Key = k; Operate = o; Value = v; }
//    public System.String Key { get; set; }
//    public Operators Operate { get; set; }
//    public System.Object Value { get; set; }
//}


///// <summary>
///// 编码条件限制（生成类似 if ( 某属性名key == 值v ) 编码当前属性;
///// </summary>
//public class EncodeCondation : System.Attribute
//{
//    public EncodeCondation( System.String k, Operators o, System.Object v ) { Key = k; Operate = o; Value = v; }
//    public System.String Key { get; set; }
//    public Operators Operate { get; set; }
//    public System.Object Value { get; set; }
//}


///// <summary>
///// 针对 数值类型 做范围 check
///// </summary>
//public class ChkValue : System.Attribute {
//    public ChkValue(System.Decimal min, System.Decimal max) {
//        if (max < min)
//            throw new System.Exception("error: max < min");
//        Min = min;
//        Max = max;
//    }
//    public System.Decimal Min { get; set; }
//    public System.Decimal Max { get; set; }
//}


// 项目分类枚举示例
//// 模板专用: 项目分类枚举( 当前未启用 )
//enum __projects
//{
//    Server1,
//    Server2,
//    Client,
//    Manage
//}
