#ifndef _FLATBUFFER_H__
#define _FLATBUFFER_H__


class FlatBuffer
{
public:
    explicit FlatBuffer( int capacity = 1024 );                                 // prepare
    FlatBuffer( char* buf, int bufLen, int dataLen = 0, bool isRef = false );   // copy or ref
    FlatBuffer( FlatBuffer const& other );                                      // copy
    FlatBuffer( FlatBuffer&& other );                                           // move
    FlatBuffer& operator=( FlatBuffer const& other );                           // copy
    FlatBuffer& operator=( FlatBuffer&& other );                                // move
    ~FlatBuffer();
    void assign( char const* buf, int bufLen, int dataLen = 0, bool isRef = false );  // copy or ref
    void reserve( int capacity );
    void resize( int widx );
    void clear();                                                               // _dataLen = _offset = 0
    int size() const;                                                           // _dataLen
    int offset() const;                                                         // _offset
    int& offset();                                                              // _offset
    bool empty() const;                                                         // _dataLen == 0
    bool eof() const;                                                           // _dataLen == _offset
    char const* data() const;                                                   // _buf
    char* data();                                                               // _buf
    char operator[] ( int idx ) const;                                          // _buf[ idx ]
    char& operator[] ( int idx );                                               // _buf[ idx ]
    char at( int idx ) const;                                                   // _buf[ idx ]
    char& at( int idx );                                                        // _buf[ idx ]

    template<typename T>
    void write( T const& v );
    void write( char const* buf, int dataLen );
    template<typename ...TS>
    void writes( TS const& ...vs );

    template<typename T>
    bool read( T& v );
    bool read( char* buf, int dataLen );

    // todo: List Dict support

    // todo: writeVar  readVar support

    String dump();

private:
    typedef void ( FlatBuffer::*Disposer )( );
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    int         _offset;
    Disposer    _disposer;
};

#include "FlatBuffer.hpp"


#endif
