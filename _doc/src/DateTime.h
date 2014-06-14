#ifndef _DATETIME_H_
#define _DATETIME_H_

// UTC 时间( 和中国时间差 8 小时，得到实际时间，小时要加 8 )
struct DateTime
{
    DateTime();
    DateTime( char const * s );                                         // 根据传入的 19 字节长的字串 来初始化
    DateTime( int year, int month, int day, int hour = 0, int minute = 0, int second = 0 );
    void                    update();                                   // 更新数据到当前系统时间
    void                    assign( char const * s );
	void                    assign( int year, int month, int day, int hour = 0, int minute = 0, int second = 0 ); // 同构造
    int                     toString( char * buf ) const;               // 填充格式化字串到 buf yyyy-MM-dd hh:mm:ss
    void                    toString( String & s ) const;
    static DateTime         now();                                      // 返回已初始化为当前时间的 DateTime
    bool                    operator==( DateTime const & dateTime ) const;
    bool                    operator!=( DateTime const & dateTime ) const;
    bool                    operator< ( DateTime const & dateTime ) const;
    bool                    operator<=( DateTime const & dateTime ) const;
    bool                    operator> ( DateTime const & dateTime ) const;
    bool                    operator>=( DateTime const & dateTime ) const;

    static DateTime const   defaultValue;

    union
    {
	uint64                  _value;                                     // 简单压缩存储: 从高到低 2字节年, 1字节月日时分秒, 空1字节
    struct
    {
        uint16              _year;
        uint8               _month;
        uint8               _day;
        uint8               _hour;
        uint8               _minute;
        uint8               _second;
        uint8               __unused;
    };
    };
};

enum class Months
{                                                    // 1 ~ 12 月
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

enum class DaysOfWeek
{                                                   // 周日是 0
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

// 日期辅助类（获取，计算）
struct DateTimeHelper
{

	static const int64      MILLISECONDS = 1000;
	static const int64      SECONDS      = 1000 * MILLISECONDS;
	static const int64      MINUTES      =   60 * SECONDS;
	static const int64      HOURS        =   60 * MINUTES;
	static const int64      DAYS         =   24 * HOURS;
    DateTimeHelper();                                                   // 啥都不干
    void                    update();                                   // 更新到当前系统时间
	static bool             IsLeapYear( int year );                     // 判断是否为闰年
	static int              DaysInMonth( int year, int month );         // 取某年某有应该有多少天
	void                    ComputeGregorian( double julianDay );       // 计算 julian 日的 Gregorian 日期并填充日期时间属性
	void                    ComputeDaytime();                           // 用当前存储的 UTC 时间来计算填充日期时间属性
	static void             CheckLimit( int & lower, int & higher, int limit );   // 溢出进位检查处理
	void                    Normalize();                                // 处理当前数据的低阶时间单位溢出进位
    int64                   _value;                                     // TimeSpan 时间
	int64                   _utcTime;                                   // UTC 时间
    int                     _year;                                      // 年
    int                     _month;                                     // 月
    int                     _day;                                       // 日
    int                     _hour;                                      // 时
    int                     _minute;                                    // 分
    int                     _second;                                    // 秒
    int                     _millisecond;                               // 毫
    int                     _microsecond;                               // 微
};


#endif
