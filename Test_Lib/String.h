#ifndef _STRING_H__
#define _STRING_H__

// 支持传入定长 buffer 作为容器
// 支持引用 char const * const 字串
// todo:
// 各种运算符重载
// 支持各种常用 字串 处理
// 支持 2进制 拼接 行为
// Utils 要补 2进制 拼接相关函数
// 移到 Lib 之后，Utils 要补 String 对象本身的函数
class String
{
public:
    // prepare
    explicit String( int capacity = 64 );

    // ref
    String( char* buf, int bufLen, int dataLen = 0 );

    // copy / ref
    String( char const* s, bool isRef = false );

    // copy
    String( String const & other );

    // move
    String( String && other );

    // copy
    String& operator=( String const & other );

    // move
    String& operator=( String && other );

    // dispose buf
    ~String();

    // prepare memory
    void reserve( int len );

    // set _dataLen to 0
    void clear();

    // return _buf
    char* c_str();


    void toLower();
    void toUpper();

    // 4x faster than toLower(). will be damage these chars：   @[\]^_` 
    void toLowerUnsafe();

    template<typename ...TS>
    void append( TS const & ...vs );

    // todo: more util funcs
    // todo: more operator like ==, > < ...

private:
    typedef void ( String::*Disposer )( );
    void disposeIncommingBuffer();
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    Disposer    _disposer;
};

#include "String.hpp"


#endif
