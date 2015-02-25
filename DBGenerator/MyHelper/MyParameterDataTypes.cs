namespace MyHelper
{
    // Qyery 参数类型
    public enum MyParameterDataTypes
    {
        Boolean,    // 1, 0, true, false
        Numeric,    // 各种数值类型，不需要转义（时间戳也算数值类型）
        DateTime,   // 需要转为 "年-月-日 时:分:秒" 的字串格式
        String,     // 要转义单引号和反斜杠，两头自动加 ''
        StringPart, // 不会在两头加 ''
        Binary,     // 会转为 0xAABBCC 这样的格式
        Numerics,   // 这三个为上面的单参的复制形式，将生成逗号分隔，主用于 in 之类的表达式
        DateTimes,  // 这三个为上面的单参的复制形式，将生成逗号分隔，主用于 in 之类的表达式
        Strings,    // 这三个为上面的单参的复制形式，将生成逗号分隔，主用于 in 之类的表达式
    };

}
