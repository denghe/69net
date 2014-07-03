#ifndef _STRING_H__
#define _STRING_H__

// support import fixed buffer
// support ref char const * string
// todo:
// 各种运算符重载
// 支持各种常用 字串 处理
// 内存池
class String
{
public:
    explicit String( int capacity = 64 );                       // prepare
    String( char* buf, int bufLen, int dataLen = 0 );           // ref
    String( char const* s, bool isRef = false );                // copy or ref
    String( String const & other );                             // copy
    String( String && other );                                  // move
    String& operator=( String const & other );                  // copy
    String& operator=( String && other );                       // move
    ~String();                                                  // dispose buf
    void reserve( int len );                                    // prepare memory
    void resize( int len, bool fillZero = true );               // cut( maybe damage utf8 ) or grow up
    void clear();                                               // set _dataLen to 0
    int size() const;                                           // return _dataLen;
    char* c_str() const;                                        // return _buf
    char* c_str();                                              // return _buf
    operator char*( ) const;                                    // return _buf
    operator char*( );                                          // return _buf
    char& operator[] ( int idx ) const;                         // return _buf[ idx ]
    char& operator[] ( int idx );                               // return _buf[ idx ]
    char at( int idx ) const;
    void set( int idx, char v );


    bool operator==( String const& other );
    bool operator!=( String const& other );
    bool operator<( String const& other );
    bool operator>( String const& other );
    bool operator<=( String const& other );
    bool operator>=( String const& other );


    void toLower();
    void toUpper();
    void toLowerUnsafe();                                       // 2x faster than toLower(). bad chars：   @[\]^_` 

    template<typename ...TS>
    void append( TS const & ...vs );

    int getHashCode();                                          // only support align of 4 _buf on some ARM cpu and unaligned buffer

    // todo: more util funcs

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
