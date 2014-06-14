#ifndef _CONVERTS_H_
#define _CONVERTS_H_
namespace Converts
{
// 将基础类型转为字串。该系列函数均为不安全操作　不判断长度　不判断越界

// 将值类型转换为字符串 返回转换后的串长 (主要为 Append, Write 等函数服务)
int toString( char * dstBuf, uint8   v );
int toString( char * dstBuf, uint16  v );
int toString( char * dstBuf, uint    v );
int toString( char * dstBuf, uint64  v );
int toString( char * dstBuf, int8    v );
int toString( char * dstBuf, int16   v );
int toString( char * dstBuf, int     v );
int toString( char * dstBuf, int64   v );
int toString( char * dstBuf, double  v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
int toString( char * dstBuf, float   v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
int toString( char * dstBuf, bool    v );

// 字串转值（值后面需要以 0 为结尾且不能有乱七八糟的 空格, TAB 等排版符）
void fromString( uint8   & dstVar, char const * s );
void fromString( uint16  & dstVar, char const * s );
void fromString( uint32  & dstVar, char const * s );
void fromString( uint64  & dstVar, char const * s );
void fromString( int8    & dstVar, char const * s );
void fromString( int16   & dstVar, char const * s );
void fromString( int     & dstVar, char const * s );
void fromString( int64   & dstVar, char const * s );
void fromString( double  & dstVar, char const * s );
void fromString( float   & dstVar, char const * s );
void fromString( bool    & dstVar, char const * s );

// 16 进制版
int toStringHex( char * dstBuf, uint8   v );
int toStringHex( char * dstBuf, uint16  v );
int toStringHex( char * dstBuf, uint    v );
int toStringHex( char * dstBuf, uint64  v );
int toStringHex( char * dstBuf, int8    v );
int toStringHex( char * dstBuf, int16   v );
int toStringHex( char * dstBuf, int     v );
int toStringHex( char * dstBuf, int64   v );
int toStringHex( char * dstBuf, double  v );
int toStringHex( char * dstBuf, float   v );
int toStringHex( char * dstBuf, bool    v );

void fromStringHex( uint8   & dstVar, char const * s );
void fromStringHex( uint16  & dstVar, char const * s );
void fromStringHex( uint32  & dstVar, char const * s );
void fromStringHex( uint64  & dstVar, char const * s );
void fromStringHex( int8    & dstVar, char const * s );
void fromStringHex( int16   & dstVar, char const * s );
void fromStringHex( int     & dstVar, char const * s );
void fromStringHex( int64   & dstVar, char const * s );
void fromStringHex( double  & dstVar, char const * s );
void fromStringHex( float   & dstVar, char const * s );
void fromStringHex( bool    & dstVar, char const * s );


// 转换用表
// 将 0 ~ 15 映射成相应的 ASCII
char const Int2HexTable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

// 将 0 ~ 9, a ~ f, A ~ F 的 ASCII 映射成相应的 int 值
uint8 const Hex2IntTable[] =
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
    2,  3,  4,  5,  6,  7,  8,  9,  0,  0,
    0,  0,  0,  0,  0,  10, 11, 12, 13, 14,
    15, 0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  10, 11, 12,
    13, 14, 15
};


// utf8 相关


int toStringUtf8( char * dstBuf, wchar_t const * s );

int toStringUtf8( char * dstBuf, wchar_t const * s, int len );

int toStringUtf8( char * dstBuf, wchar_t const & s );

int toStringUnicode( wchar_t * dstBuf, char const * s, int len );




}


#endif
