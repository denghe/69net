#ifndef _FLATBUFFER_H__
#define _FLATBUFFER_H__


class FlatBuffer
{
public:
    explicit FlatBuffer( int capacity = 1024 );                                 // prepare
    FlatBuffer( char* buf, int bufLen, int dataLen = 0, bool isRef = false );   // copy or ref
    FlatBuffer( FlatBuffer const & other );                                     // copy
    FlatBuffer( FlatBuffer && other );                                          // move
    FlatBuffer& operator=( FlatBuffer const & other );                          // copy
    FlatBuffer& operator=( FlatBuffer && other );                               // move
    ~FlatBuffer();
    void reserve( int capacity );
    void clear();                                                               // ridx = widx = 0
    int size() const;                                                           // widx
    bool empty() const;
    char* data() const;
    char& operator[] ( int idx ) const;                                         // return _buf[ idx ]
    char& operator[] ( int idx );                                               // return _buf[ idx ]
    char at( int idx ) const;
    char& at( int idx );
    void set( int idx, char v );
    int& widx();
    int& ridx();

    template<typename T>
    void write( T const& v );
    void write( char const* buf, int len );
    template<typename ...TS>
    void writes( TS const & ...vs );

    // return 0: success    -1: overflow    1+: out of range
    template<typename T>
    int read( T& v );
    int read( char* buf, int len );

private:
    typedef void ( FlatBuffer::*Disposer )( );
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buf;
    int         _bufLen;
    int         _widx;
    int         _ridx;
    Disposer    _disposer;
};

#include "FlatBuffer.hpp"


#endif
