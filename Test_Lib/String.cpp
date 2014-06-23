#include "../Lib/All.h"
#include "String.h"


String::String( int capacity /*= 64 */ )
{
    if( capacity <= 64 ) _bufferLength = 64;
    else _bufferLength = Utils::round2n( capacity );
    _disposer = &String::disposeNewBuffer;
    _dataLength = 0;
    _buffer = new char[ _bufferLength ];

    // todo: 做个池？
    // todo: if capacity <= pool.item.buffer.size 则从 pool 分配, _disposer 指向 disposePoolBuffer
}

String::String( char* buffer, int len )    // todo: 加个 bufLen 参数, 相关调用要改
    : _buffer( buffer )
    , _bufferLength( len )
    , _dataLength( 0 )
    , _disposer( nullptr )
{
}

String::String( String const & other )
    : String( other._buffer, Utils::round2n( other._dataLength ) )
{
    _dataLength = other._dataLength;
}

String::String( String && other )
    : _buffer( other._buffer )
    , _bufferLength( other._bufferLength )
    , _dataLength( other._dataLength )
    , _disposer( other._disposer )
{
    other._buffer = nullptr;
    other._bufferLength = 0;
    other._dataLength = 0;
    other._disposer = nullptr;
}

//String::String( char const * s )
//{
//    auto len = strlen( s );
//    if( len <= 64 ) _bufferLength = 64;
//    else _bufferLength = Utils::round2n( len );
//    _disposer = &String::disposeNewBuffer;
//    _buffer = new char[ _bufferLength ];
//    memcpy( _buffer, s, len + 1 );
//    _dataLength = len;
//}

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

String& String::operator=( String const & other )
{
    // todo
    return *this;
}

String& String::operator=( String && other )
{
    // todo
    return *this;
}
