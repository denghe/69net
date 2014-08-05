/********************************************************/
// 关键字

// enum
// class

/********************************************************/
// 标记

// Desc( "注释" )
// Limit( [定长] | [ 最小长, 最大长 ] )   todo: 考虑未来做成变长以方便限定多级容器  或再实现个 Limits 容器
// Condation( 属性名, 值 [ , 属性名, 值 ][, ...] )

/********************************************************/
// 数据类型

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

/********************************************************/
// 集合( 通常需要用 Limit 来限定元素个数的合法范围 )

// []                   // 必须用 Limit 标记限长
// string
// List<V>
// Dict<K, V>

/********************************************************/
// 泛型( todo )

// Pair<K, V>
// Tuple<...>

/********************************************************/
// 输出

// c++ 对应   |  c# 对应      |  lua 类型对应
//------------+---------------+------------------------
// byte       |  byte         | double
// ushort     |  ushort       | double
// uint       |  uint         | double
// uint64     |  ulong        | double
// sbyte      |  sbyte        | double
// short      |  short        | double
// int        |  int          | double
// int64      |  long         | double
// double     |  double       | double
// float      |  float        | double
// bool       |  bool         | bool
// []         |  []           | table
// String     |  string       | string
// List       |  List         | table
// Dict       |  Dictionary   | table

namespace PacketLibrary
{

    public class List<V> { }
    public class Dict<K, V> { }

    /// <summary>
    /// 备注。可用于类/枚举 及其 成员
    /// </summary>
    public class Desc : System.Attribute
    {
        public Desc( string v ) { Value = v; }
        public string Value { get; set; }
    }

    /// <summary>
    /// 针对最外层级的 数组, byte[], string 做长度限制。单个长度值为定长
    /// </summary>
    public class Limit : System.Attribute
    {
        public Limit( int fix )
        {
            Min = fix;
            Max = fix;
        }
        public Limit( int min, int max )
        {
            if( max < min ) throw new System.Exception( "error: max < min" );
            Min = min;
            Max = max;
        }
        public int Min { get; set; }
        public int Max { get; set; }
    }

    public class Limits : System.Attribute
    {
        public Limits( params int[] v )
        {
            Value = v;
        }
        public int[] Value { get; set; }
    }

    /// <summary>
    /// 设置序列化条件: 须满足 某 field  等于 value ( field name, value  成对交替填写 )
    /// </summary>
    public class Condation : System.Attribute
    {
        public Condation( params object[] ps ) { Value = ps; }
        public object[] Value { get; set; }
    }

    /// <summary>
    /// 设置 field 初始值
    /// </summary>
    public class Default : System.Attribute
    {
        public Default( object v ) { Value = v; }
        public object Value { get; set; }
    }

    /// <summary>
    /// 是否创建 Get 函数
    /// </summary>
    public class Get : System.Attribute
    {
        public Get() { Value = true; }
        public Get( bool v ) { Value = v; }
        public bool Value { get; set; }
    }

    /// <summary>
    /// 是否创建 Set 函数
    /// </summary>
    public class Set : System.Attribute
    {
        public Set() { Value = true; }
        public Set( bool v ) { Value = v; }
        public bool Value { get; set; }
    }

}



/***********************************************************************************/
// 下面的东西当前未启用, 也不完整. 先放这，待整理
/***********************************************************************************/


///// <summary>
///// 是否启用。默认为启用。可用于类和属性。
///// </summary>
//public class Enabled : System.Attribute
//{
//    public Enabled( bool v ) { Value = v; }
//    public bool Value { get; set; }
//}


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
///// 简单设置相关性（可用于生成链接文档）
///// </summary>
//public class SeeAlso : System.Attribute
//{
//    public SeeAlso( params  string[] v ) { Value = v; }
//    public string[] Value { get; set; }
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
//    public DecodeCondation( string k, Operators o, System.Object v ) { Key = k; Operate = o; Value = v; }
//    public string Key { get; set; }
//    public Operators Operate { get; set; }
//    public System.Object Value { get; set; }
//}


///// <summary>
///// 编码条件限制（生成类似 if ( 某属性名key == 值v ) 编码当前属性;
///// </summary>
//public class EncodeCondation : System.Attribute
//{
//    public EncodeCondation( string k, Operators o, System.Object v ) { Key = k; Operate = o; Value = v; }
//    public string Key { get; set; }
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
