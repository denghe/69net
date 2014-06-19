#ifndef DATETIME_H
#define DATETIME_H
namespace Lib3 {

struct SString;
struct String;

// 简单压缩存储的 UTC 时间格式 (和成都本地时间差8小时，得到实际时间，小时要加8）
// 从高到低 2字节年, 1字节月日时分秒, 空1字节. 为使用方便，属性就不封装了
//
union DateTime {
    DateTime();                                                         // 不初始化
    DateTime(SString const &s);                                          // 根据传入的 19 字节长的字串 来初始化
    template<UInt32 len>
    DateTime(SByte const (&s)[len]);                                     // 根据写死的资源串来转
    DateTime(String const &s);                                         // 根据传入的 19 字节长的字串 来初始化
    template<UInt32 len>
    DateTime(Char const (&s)[len]);                                    // 根据写死的资源串来转
    DateTime(Int32 const &year, Int32 const &month, Int32 const &day, Int32 const &hour = 0, Int32 const &minute = 0, Int32 const &second = 0);
    void                    Update();                                   // 更新数据到当前系统时间
    DateTime &              Set(SString const &s);
    template<UInt32 len>
    DateTime &              Set(SByte const (&s)[len]);
    DateTime &              Set(String const &s);
    template<UInt32 len>
    DateTime &              Set(Char const (&s)[len]);
	DateTime &              Set(Int32 const &year, Int32 const &month, Int32 const &day, Int32 const &hour = 0, Int32 const &minute = 0, Int32 const &second = 0); // 同构造
	Boolean                 operator==(DateTime const &dateTime) const;	
	Boolean                 operator!=(DateTime const &dateTime) const;	
	Boolean                 operator< (DateTime const &dateTime) const;	
	Boolean                 operator<=(DateTime const &dateTime) const;	
	Boolean                 operator> (DateTime const &dateTime) const;	
	Boolean                 operator>=(DateTime const &dateTime) const;	
    static DateTime         Now();                                      // 返回已初始化为当前时间的 DateTime

	UInt64                  Value;                                      // 简单压缩存储: 从高到低 2字节年, 1字节月日时分秒, 空1字节
    struct {
#ifdef __LE
        UInt16              Year;
        Byte                Month;
        Byte                Day;
        Byte                Hour;
        Byte                Minute;
        UInt16              Second;
#else
        UInt16              Second;
        Byte                Minute;
        Byte                Hour;
        Byte                Day;
        Byte                Month;
        UInt16              Year;
#endif
    };
};


struct DateTimeHelper {
	enum Months {                                                       // 1 ~ 12 月
		JANUARY = 1,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER
	};
	enum DaysOfWeek {                                                   // 周日是 0
		SUNDAY = 0,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};
	static const Int64      MILLISECONDS = 1000;
	static const Int64      SECONDS      = 1000 * MILLISECONDS;
	static const Int64      MINUTES      =   60 * SECONDS;
	static const Int64      HOURS        =   60 * MINUTES;
	static const Int64      DAYS         =   24 * HOURS;
    DateTimeHelper();                                                   // 啥都不干
    void                    Update();                                   // 更新到当前系统时间
	static Boolean          IsLeapYear(Int32 const &year);              // 判断是否为闰年
	static Int32            DaysInMonth(Int32 const &year, Int32 const &month);       // 取某年某有应该有多少天
	void                    ComputeGregorian(Double const &julianDay);  // 计算 julian 日的 Gregorian 日期并填充日期时间属性
	void                    ComputeDaytime();                           // 用当前存储的 UTC 时间来计算填充日期时间属性
	static void             CheckLimit(Int16 &lower, Int16 &higher, Int16 const &limit);   // 溢出进位检查处理
	void                    Normalize();                                // 处理当前数据的低阶时间单位溢出进位
    Int64                   _value;                                     // TimeSpan 时间
	Int64                   _utcTime;                                   // UTC 时间
	Int16                   _year;                                      // 年
	Int16                   _month;                                     // 月（这几个都用 Int16 是因为即便换 Byte 整个结构体也是占 24 字节）
	Int16                   _day;                                       // 日
	Int16                   _hour;                                      // 时
	Int16                   _minute;                                    // 分
	Int16                   _second;                                    // 秒
	Int16                   _millisecond;                               // 毫
	Int16                   _microsecond;                               // 微
};



}
#endif
