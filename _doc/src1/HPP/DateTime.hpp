#ifndef DATETIME_HPP
#define DATETIME_HPP
namespace Lib3 {




INLINE DateTime::DateTime() {
}
INLINE DateTime::DateTime(SString const &s) {
    Set(s);
}
template<UInt32 len>
INLINE DateTime::DateTime(SByte const (&s)[len]) {
    Set(s);
}
INLINE DateTime::DateTime(String const &s) {
    Set(s);
}
template<UInt32 len>
INLINE DateTime::DateTime(Char const (&s)[len]) {
    Set(s);
}
INLINE DateTime::DateTime(Int32 const &year, Int32 const &month, Int32 const &day, Int32 const &hour, Int32 const &minute, Int32 const &second) {
    Set(year, month, day, hour, minute, second);
}
INLINE void DateTime::Update() {
    DateTimeHelper dth;
    dth.Update();
}
INLINE DateTime & DateTime::Set(SString const &s) {
    assert (s.Length() >= 19);
    Converts::Convert(s.Buffer(), *this);
    return *this;
}
template<UInt32 len>
INLINE DateTime & DateTime::Set(SByte const (&s)[len]) {
    assert (len >= 19);
    Converts::Convert(s, *this);
    return *this;
}
INLINE DateTime & DateTime::Set(Int32 const &year, Int32 const &month, Int32 const &day, Int32 const &hour, Int32 const &minute, Int32 const &second) {
    Year   = year;
    Month  = month;
    Day    = day;
    Hour   = hour;
    Minute = minute;
    Second = second;
    return *this;
}
INLINE Boolean DateTime::operator==(DateTime const &dateTime) const {
    return Value == dateTime.Value;
}
INLINE Boolean DateTime::operator!=(DateTime const &dateTime) const {
    return Value != dateTime.Value;
}	
INLINE Boolean DateTime::operator< (DateTime const &dateTime) const {
    return Value < dateTime.Value;
}	
INLINE Boolean DateTime::operator<=(DateTime const &dateTime) const {
    return Value <= dateTime.Value;
}	
INLINE Boolean DateTime::operator> (DateTime const &dateTime) const {
    return Value > dateTime.Value;
}	
INLINE Boolean DateTime::operator>=(DateTime const &dateTime) const {
    return Value >= dateTime.Value;
}	
INLINE DateTime DateTime::Now() {
    DateTime t;
    t.Update();
    return t;
}


















INLINE DateTimeHelper::DateTimeHelper() { }
INLINE void DateTimeHelper::Update() {
#ifdef __linux
	struct timeval tv;
	if (gettimeofday(&tv, Null))
		assert ( false );
	_value = Int64(tv.tv_sec) * 1000000 + tv.tv_usec;
#else
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;
	ULARGE_INTEGER ts;
	ts.LowPart  = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	_value = ts.QuadPart / 10;
#endif
    _utcTime = _value * 10 + ((Int64)0x01b21dd2 << 32) + 0x13814000;
    var utcDays = Double(_utcTime) / 864000000000.0;
    var julianDay = utcDays + 2299160.5;
    ComputeGregorian(julianDay);
	ComputeDaytime();
}
INLINE void DateTimeHelper::ComputeGregorian(Double const &julianDay) {
	Double z    = std::floor(julianDay - 1721118.5);
	Double r    = julianDay - 1721118.5 - z;
	Double g    = z - 0.25;
	Double a    = std::floor(g / 36524.25);
	Double b    = a - std::floor(a / 4);
	_year       = (Int16)std::floor((b + g) / 365.25);
	Double c    = b + z - std::floor(365.25 * _year);
	_month      = (Int16)std::floor((5 * c + 456) / 153);
	Double dday = c - std::floor((153.0 * _month - 457) / 5) + r;
	_day        = (Int16)dday;
	if (_month > 12) {
		++_year;
		_month -= 12;
	}
	r      *= 24;
	_hour   = (Int16)std::floor(r);
	r      -= std::floor(r);
	r      *= 60;
	_minute = (Int16)std::floor(r);
	r      -= std::floor(r);
	r      *= 60;
	_second = (Int16)std::floor(r);
	r      -= std::floor(r);
	r      *= 1000;
	_millisecond = (Int16)std::floor(r);
	r      -= std::floor(r);
	r      *= 1000;
	_microsecond = (Int16)(r + 0.5);
	Normalize();
}


INLINE void DateTimeHelper::ComputeDaytime() {
    // var ms      = _utcTime / 10;
	_hour       = Int32((_value / HOURS) % 24);
	_minute     = Int32((_value / MINUTES) % 60);
	_second     = Int32((_value / SECONDS) % 60);
}
INLINE void DateTimeHelper::CheckLimit(Int16 &lower, Int16 &higher, Int16 const &limit) {
	if (lower >= limit) {
		higher += Int16(lower / limit);
		lower   = Int16(lower % limit);
	}
}
INLINE Int32 DateTimeHelper::DaysInMonth(Int32 const &year, Int32 const &month) {
	assert( month >= 1 && month <= 12 );
	static Int32 daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && IsLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}
INLINE Boolean DateTimeHelper::IsLeapYear(Int32 const &year) {
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}
INLINE void DateTimeHelper::Normalize() {
	CheckLimit(_microsecond, _millisecond, 1000);
	CheckLimit(_millisecond, _second, 1000);
	CheckLimit(_second, _minute, 60);
	CheckLimit(_minute, _hour, 60);
	CheckLimit(_hour, _day, 24);
	if (_day > DaysInMonth(_year, _month)) {
		_day -= DaysInMonth(_year, _month);
		if (++_month > 12) {
			++_year;
			_month -= 12;
		}
	}
}










}
#endif
