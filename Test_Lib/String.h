#ifndef _STRING_H__
#define _STRING_H__

// 支持传入定长 buffer 作为容器
// todo:
// 各种复制构造啥的
// 支持各种常用 字串 处理
// 支持 2进制 拼接 行为
// Utils 要补 2进制 拼接相关函数
// 移到 Lib 之后，Utils 要补 String 对象本身的函数
class String
{
public:
    String( char* buffer, int bufLen, int dataLen = 0 );
    explicit String( int capacity = 64 );

    template<size_t bufLen>
    String( char( &buffer )[ bufLen ], int dataLen = 0 );

    template<size_t len>
    String( char const( &buffer )[ len ] );

    String( String const & other );
    String( String && other );

    String& operator=( String const & other );
    String& operator=( String && other );

    ~String();
    void reserve( int len );
    void clear();
    char* c_str();

    // todo: more operator like ==, > < ...

    template<typename ...TS>
    void append( TS const & ...vs );

    // todo: more util funcs
    void toLower();
    void toUpper();
    void toLowerUnsafe();
    void toUpperUnsafe();

private:
    typedef void ( String::*Disposer )( );
    void disposeIncommingBuffer();
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buffer;
    int         _bufferLength;
    int         _dataLength;
    Disposer    _disposer;
};

#include "String.hpp"


#endif
