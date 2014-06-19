#ifndef BASEDEFINES_H
#define BASEDEFINES_H
namespace Lib3 {


// 方便理解和使用
//
typedef bool                        Boolean;
typedef float                       Single;
typedef double                      Double;
typedef wchar_t                     Char;

typedef char                        SByte, SChar;
typedef unsigned char               Byte;

typedef short                       Int16;
typedef unsigned short              UInt16;

typedef int                         Int32;
typedef unsigned int                UInt32;

#ifdef __GNUC__
#ifdef __LP64__
    typedef long                    Int64;
    typedef unsigned long           UInt64;
#else
    typedef long long               Int64;
    typedef unsigned long long      UInt64;
#endif
#else
typedef __int64                     Int64;
typedef unsigned __int64            UInt64;
#endif




// 代表一个空值
// 可用 Null 来替代 NULL 宏，并且可强类型识别（比如用来赋空值）
const struct NullType {
    template<class T> operator T*() const {
        return 0; 
    }
    template<class C, class T> operator T C::*() const {
        return 0; 
    }
private:
    void operator &() const;
} Null = {};


// windows 的判断: _WIN32      linux 的判断 __linux          gcc 的判断  __GNUC__
// gcc 判断 wchar_t 长度的宏： __SIZEOF_WCHAR_T__ 


// 方便判断当前是否为 x64
//
#if defined(__LP64__) || defined(_WIN64) || defined(_M_X64)
    #define __X64
#endif

// 方便判断当前是否为 intel/amd  32/64位 cpu (可以取rdtsc, 可以不用对齐指针硬转赋值等)
//
#if defined(_M_IX86) || defined(__i386__) 
    #define __IA32
#endif

#if defined(_M_AMD64) || defined(__x86_64__)
    #define __IA64
#endif

#if defined(__IA32) || defined(__IA64)
    #define __IA
#endif

// 方便判定是否为小尾。GCC 下的宏定义为 LITTLE_ENDIAN, VC 下为 LITTLEENDIAN, 统一为 GCC 版本写法
//
#if defined(LITTLE_ENDIAN) || defined(LITTLEENDIAN)
    #define __LE
#endif

// 方便敲代码
//
#define var                         auto
#define Ref                         std::shared_ptr
#define Pair                        std::pair

// 内联, 局部变量起始地址内存对齐  ( 代码抄至 doom3 )
//
#ifdef __GNUC__
    #define INLINE                  inline
    #define ALIGN8( x )             x __attribute__ ((aligned (8)))
    #define ALIGN16( x )            x __attribute__ ((aligned (16)))
#else
    #define INLINE                  __inline
    #define ALIGN2( x )		        __declspec(align(2)) x
    #define ALIGN4( x )		        __declspec(align(4)) x
    #define ALIGN8( x )		        __declspec(align(8)) x
    #define ALIGN16( x )		    __declspec(align(16)) x
#endif

// 用于确认 DEBUG 状态
//
#if defined(_DEBUG) || defined(DEBUG)
    #define __DEBUG
#endif

// 调试模式下方便传 文件名，行 信息到函数内
//
#ifdef __DEBUG
#define __TI                ,__FILE__,__LINE__
#else
#define __TI
#endif

// Cout 输出换行方便
//
#define NewLine             '\n'


//
// 在 GC 上创建对象
//
#define GCNew(gc, T)                new (gc.Allocate(sizeof(T))) T
//
// 在 GC 上创建对象数组
//
#define GCNewA(gc, T, C)            new (gc.Allocate(sizeof(T) * C)) T[C]






// 部分容器中的Buffer的几种初始状态
enum BufferStates {
    BufferState_Original,           // 原始 (自带的 不能回收)
    BufferState_New,                // new 的 (需要回收)
    BufferState_Custom              // 传入的 (不能回收)
};



// 数据转换的几种处理结果
enum ConvertStates {
    ConvertState_Success = 0,       // 处理成功
    ConvertState_Failed,            // 数据源(用于还原的流)数据错误
    ConvertState_Overflow,          // 数组超长
    ConvertState_WrongID,           // 错误类型编号
    ConvertState_NotEnoughData,     // 数据源(用于还原的流)剩余数据不足
    ConvertState_NotEnoughMemory    // 内存分配器内存不足
};





}
#endif
