#ifndef _BASEUTILS_H_
#define _BASEUTILS_H_



// calc leader zero ( 数一个数字前面有多少个0 ）

#if __GCC
#define clz(x) __builtin_clz(x)
#elif __IA && __MSVC
INLINE int clz( size_t x )
{
#if __X64
    return 63 - (int)__lzcnt64( x );
#else
    return 31 - __lzcnt( x );
#endif
}
#else
#if __X64
#error not support
#endif
INLINE int popcnt( size_t x ) {
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return x & 0x0000003f;
}
//INLINE int ctz( size_t x ) {
//    return popcnt((x & -x) - 1);
//}
INLINE int clz( size_t x ) {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return 32 - popcnt(x);
}
#endif






// 返回刚好大于 x 的 2^n 的值用于内存分配

INLINE size_t round2n( size_t len ) {
#if __GCC
    int bits = 31 - clz(len);
#else
#if __X64
    int bits = 63 - clz(len);
#else
    int bits = 31 - clz(len);
#endif
#endif
    size_t rtv = size_t(1) << bits;
    if( rtv == len ) return len;
    return rtv << 1;
}








// 利用 SFINAE 特性来判断 T 内是否有 某 函数

#define HAS_FUNC( CN, FN, FT )   \
template<typename T>                                                                \
class CN                                                                            \
{                                                                                   \
    template<typename T, FT> struct FuncMatcher;                                    \
    template<typename T> static char hasFunc( FuncMatcher<T, &T::FN>* );            \
    template<typename T> static int hasFunc( ... );                                 \
public:                                                                             \
    static const bool value = sizeof( hasFunc<T>( nullptr ) ) == sizeof(char);      \
}


/*

示例：

// 创建检测类
HAS_FUNC( toString_checker, toString, void (T::*)() const );

// 目标函数
template<typename T> void Console::Write( T const & v )
{
    toString_switch<T, toString_checker<T>::value>::func( v );
}

// 辅助路由类
template<typename T, bool> struct toString_switch { };

// 带函数的会 call 这个
template<typename T> struct toString_switch<T, true> { static bool func( T const & v ) {
    // ...
} };

template<typename T> struct toString_switch<T, false> { static bool func( T const & v ) {
    // ...
} };

*/





// 回收辅助类, 用于确保函数退出时执行某种回收操作
struct FreeHelper
{
	std::function<void()> _f;
	FreeHelper( std::function<void()> f ) : _f( f ) {}
	~FreeHelper() { if( _f ) _f(); }
	inline void cancel() { _f = nullptr; }
};





template<typename T>
bool isValueType()    // 整数，浮点，枚举，指针
{
    return std::is_enum<T>::value || std::is_integral<T>::value || std::is_pointer<T>::value || std::is_floating_point<T>::value;
}







#endif
