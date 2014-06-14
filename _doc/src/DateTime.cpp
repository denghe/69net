#include "Precompile.h"



DateTime::DateTime()
    : _value( 0 )
{
}

DateTime::DateTime( char const * s )
{
    assign( s );
}

DateTime::DateTime( int year, int month, int day, int hour, int minute, int second )
{
    assign( year, month, day, hour, minute, second );
}

void DateTime::assign( char const * s )
{
    assign(
        // year
        ( s[ 0 ] - '0' ) * 1000 +
        ( s[ 1 ] - '0' ) * 100 +
        ( s[ 2 ] - '0' ) * 10 +
        ( s[ 3 ] - '0' ),
        // month
        ( s[ 5 ] - '0' ) * 10 +
        ( s[ 6 ] - '0' ),
        // day
        ( s[ 8 ] - '0' ) * 10 +
        ( s[ 9 ] - '0' ),
        // hour
        ( s[ 11 ] - '0' ) * 10 +
        ( s[ 12 ] - '0' ),
        // minute
        ( s[ 14 ] - '0' ) * 10 +
        ( s[ 15 ] - '0' ),
        // second
        ( s[ 17 ] - '0' ) * 10 +
        ( s[ 18 ] - '0' )
    );
}

void DateTime::assign( int year, int month, int day, int hour, int minute, int second )
{
    _year   = year;
    _month  = month;
    _day    = day;
    _hour   = hour;
    _minute = minute;
    _second = second;
}

void DateTime::update()
{
    DateTimeHelper dth;
    dth.update();
    assign( dth._year, dth._month, dth._day, dth._hour, dth._minute, dth._second );
}

bool DateTime::operator==( DateTime const & dateTime ) const
{
    return _value == dateTime._value;                 
}

bool DateTime::operator!=( DateTime const & dateTime ) const
{
    return _value != dateTime._value;                 
}

bool DateTime::operator< ( DateTime const & dateTime ) const 
{
    return _value < dateTime._value;                  
}

bool DateTime::operator<=( DateTime const & dateTime ) const
{
    return _value <= dateTime._value;                 
}

bool DateTime::operator> ( DateTime const & dateTime ) const
{
    return _value > dateTime._value;                  
}

bool DateTime::operator>=( DateTime const & dateTime ) const
{
    return _value >= dateTime._value;
}

DateTime DateTime::now()
{
    DateTime t;
    t.update();
    return t;
}

int DateTime::toString( char * buf ) const
{
    int n = this->_year;
    buf[ 3 ] = '0' + ( n - n / 10 * 10 );    n /= 10;
    buf[ 2 ] = '0' + ( n - n / 10 * 10 );    n /= 10;
    buf[ 1 ] = '0' + ( n - n / 10 * 10 );    n /= 10;
    buf[ 0 ] = '0' + n;
    buf[ 4 ] = '-';
    n = this->_month;
    buf[ 6 ] = '0' + ( n - n / 10 * 10 );
    buf[ 5 ] = '0' + ( n / 10 );
    buf[ 7 ] = '-';
    n = this->_day;
    buf[ 9 ] = '0' + ( n - n / 10 * 10 );
    buf[ 8 ] = '0' + ( n / 10 );
    buf[ 10 ] = ' ';
    n = this->_hour;
    buf[ 12 ] = '0' + ( n - n / 10 * 10 );
    buf[ 11 ] = '0' + ( n / 10 );
    buf[ 13 ] = ':';
    n = this->_minute;
    buf[ 15 ] = '0' + ( n - n / 10 * 10 );
    buf[ 14 ] = '0' + ( n / 10 );
    buf[ 16 ] = ':';
    n = this->_second;
    buf[ 18 ] = '0' + ( n - n / 10 * 10 );
    buf[ 17 ] = '0' + ( n / 10 );
    return 19;
}

void DateTime::toString( String & s ) const
{
    s.ensure( 19 );
    toString( s._buf + s._len );
    s._len += 19;
}

DateTime const DateTime::defaultValue;


















DateTimeHelper::DateTimeHelper()
{
}

void DateTimeHelper::update()
{
#if __WIN
    FILETIME ft;
    GetSystemTimeAsFileTime( &ft );
     // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
    ULARGE_INTEGER epoch;
    epoch.LowPart  = 0xD53E8000;
    epoch.HighPart = 0x019DB1DE;
    ULARGE_INTEGER ts;
    ts.LowPart  = ft.dwLowDateTime;
    ts.HighPart = ft.dwHighDateTime;
    ts.QuadPart -= epoch.QuadPart;
    _value = ts.QuadPart / 10;
#else
    struct timeval tv;
    if (gettimeofday(&tv, nullptr))
        assert ( false );
    _value = int64(tv.tv_sec) * 1000000 + tv.tv_usec;
#endif
    _utcTime = _value * 10 + ( (int64)0x01b21dd2 << 32 ) + 0x13814000;
    auto utcDays = double(_utcTime) / 864000000000.0;
    auto julianDay = utcDays + 2299160.5;
    ComputeGregorian(julianDay);
	ComputeDaytime();
}

void DateTimeHelper::ComputeGregorian( double julianDay )
{
	double z    = std::floor(julianDay - 1721118.5);
	double r    = julianDay - 1721118.5 - z;
	double g    = z - 0.25;
	double a    = std::floor(g / 36524.25);
	double b    = a - std::floor(a / 4);
	_year       = (int)std::floor((b + g) / 365.25);
	double c    = b + z - std::floor(365.25 * _year);
	_month      = (int)std::floor((5 * c + 456) / 153);
	double dday = c - std::floor((153.0 * _month - 457) / 5) + r;
	_day        = (int)dday;
	if (_month > 12) {
		++_year;
		_month -= 12;
	}
	r      *= 24;
	_hour   = (int)std::floor(r);
	r      -= std::floor(r);
	r      *= 60;
	_minute = (int)std::floor(r);
	r      -= std::floor(r);
	r      *= 60;
	_second = (int)std::floor(r);
	r      -= std::floor(r);
	r      *= 1000;
	_millisecond = (int)std::floor(r);
	r      -= std::floor(r);
	r      *= 1000;
	_microsecond = (int)(r + 0.5);
	Normalize();
}

void DateTimeHelper::ComputeDaytime()
{
    // auto ms      = _utcTime / 10;
	_hour       = int((_value / HOURS) % 24);
	_minute     = int((_value / MINUTES) % 60);
	_second     = int((_value / SECONDS) % 60);
}

void DateTimeHelper::CheckLimit( int & lower, int & higher, int limit )
{
	if (lower >= limit)
    {
		higher += lower / limit;
		lower   = lower % limit;
	}
}

int DateTimeHelper::DaysInMonth( int year, int month )
{
	assert( month >= 1 && month <= 12 );
	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && IsLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}

bool DateTimeHelper::IsLeapYear( int year )
{
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

void DateTimeHelper::Normalize()
{
	CheckLimit(_microsecond, _millisecond, 1000);
	CheckLimit(_millisecond, _second, 1000);
	CheckLimit(_second, _minute, 60);
	CheckLimit(_minute, _hour, 60);
	CheckLimit(_hour, _day, 24);
	if (_day > DaysInMonth(_year, _month))
    {
		_day -= DaysInMonth(_year, _month);
		if (++_month > 12)
        {
			++_year;
			_month -= 12;
		}
	}
}

