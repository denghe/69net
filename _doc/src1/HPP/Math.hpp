#ifndef MATH_HPP
#define MATH_HPP
namespace Lib3 {
namespace Math {



template<typename T>
INLINE UInt32 GetHashCode(T const &in) {
    if (sizeof(T) == 1)
        return ((Byte*)&in)[0];
    if (sizeof(T) == 2)
        return ((Byte*)&in)[0] || ( ((Byte*)&in)[1] << 8 );
    if (sizeof(T) == 3)
        return ((Byte*)&in)[0] || ( ((Byte*)&in)[1] << 8 ) || ( ((Byte*)&in)[2] << 16 );
    if (sizeof(T) == 4)
        return *(UInt32*)&in;
    return GetHashCode((Byte*)&in, sizeof(T));     // ４字节以上，循环计算
}



// 判断传入值是否为素数
INLINE Boolean IsPrime(UInt64 const &n) {
    if ((n & 1) == 0) return (n == 2);                                  // 偶数, 或 < 2 的都不是素数
    var num = (UInt64)sqrt((double) n);                                 // 开方, 得到判断范围  todo: 高速不精确的开方算法   Int32 最大值: 46341
    for (UInt64 i = 3; i <= num; i += 2)
        if ((n % i) == 0)
            return false;                                               // 如果能被 >= 3 的奇数除尽, 则非素数
    return true;
}


INLINE UInt32 GetHashCode(Byte const * const &buf, UInt32 const &len) {
    UInt32 seed = 131, hash = 0;
    for (UInt32 i = 0; i < len; ++i)
        hash = hash * seed + buf[i];
    return hash;
}


INLINE UInt32 GetHashCode(SString const &in) {
    if (in.Length() == 0)
        return 0;
    return GetHashCode((Byte*)in.Buffer(), in.Length());
}
INLINE UInt32 GetHashCode(String const &in) {
    if (in.Length() == 0)
        return 0;
    return GetHashCode((Byte*)in.Buffer(), in.Length() * sizeof(Char));
}




}}
#endif