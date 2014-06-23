#include "../Lib/All.h"
#include "String.h"


String::String( int capacity /*= 64 */ )
    : _buffer( new char[ capacity ] )
    , _bufferLength( capacity )
    , _dataLength( 0 )
    , _disposer( &String::disposeNewBuffer )
{
    // todo: 做个池？
    // todo: if capacity <= pool.item.buffer.size 则从 pool 分配, _disposer 指向 disposePoolBuffer
}

String::String( char* buffer, int len )
    : _buffer( buffer )
    , _bufferLength( len )
    , _dataLength( 0 )
    , _disposer( nullptr )
{
}

String::~String()
{
    if( _disposer ) ( this->*_disposer )( );
}

void String::reserve( int len )
{
    if( len <= _bufferLength ) return;
    auto newBuffer = new char[ len ];
    memcpy( newBuffer, _buffer, _dataLength );
    if( _disposer ) ( this->*_disposer )( );
    _disposer = &String::disposeNewBuffer;
    _buffer = newBuffer;
    _bufferLength = len;
}

char* String::c_str()
{
    return _buffer;
}

void String::disposeIncommingBuffer()
{
}

void String::disposePoolBuffer()
{
}

void String::disposeNewBuffer()
{
    delete[] _buffer;
}
