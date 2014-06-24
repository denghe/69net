#include "../Lib/All.h"
#include "String.h"


String::String( char* buffer, int bufLen, int dataLen )
    : _buffer( buffer )
    , _bufferLength( bufLen )
    , _dataLength( dataLen )
    , _disposer( nullptr )
{
}

// todo: 做个池？ 注意替换每个第 1 次 new 的位置
// todo: if capacity <= pool.item.buffer.size 则从 pool 分配, _disposer 指向 disposePoolBuffer
String::String( int capacity /*= 64 */ )
{
    if( capacity <= 64 ) _bufferLength = 64;
    else _bufferLength = (int)Utils::round2n( capacity );
    _disposer = &String::disposeNewBuffer;
    _dataLength = 0;
    std::cout << "String::String( int capacity ) new char[ " << _bufferLength << " ]\n";
    _buffer = new char[ _bufferLength ];
}

String::String( String const & other )
    : String( other._buffer, (int)Utils::round2n( other._dataLength ), other._dataLength )
{
}

String::String( String && other )
    : _buffer( other._buffer )
    , _bufferLength( other._bufferLength )
    , _dataLength( other._dataLength )
    , _disposer( other._disposer )
{
    //other._buffer = nullptr;  // 这三行不必要
    //other._bufferLength = 0;
    //other._dataLength = 0;
    other._disposer = nullptr;
}

String::~String()
{
    if( _disposer ) ( this->*_disposer )( );
}

void String::reserve( int len )
{
    if( len < _bufferLength ) return;
    len = (int)Utils::round2n( len + 1 );
    _bufferLength = len;
    std::cout << "void String::reserve( int len ) new char[ " << len << " ]\n";
    auto newBuffer = new char[ len ];
    memcpy( newBuffer, _buffer, _dataLength + 1 );
    if( _disposer ) ( this->*_disposer )( );
    _disposer = &String::disposeNewBuffer;
    _buffer = newBuffer;
}

char* String::c_str()
{
    return _buffer;
}

void String::disposeIncommingBuffer() {}

void String::disposePoolBuffer()
{
    // todo
}

void String::disposeNewBuffer()
{
    std::cout << "delete[] _buffer\n";
    delete[] _buffer;
}

String& String::operator=( String const & other )
{
    if( _bufferLength > other._dataLength )
    {
        _dataLength = other._dataLength;
        memcpy( _buffer, other._buffer, other._dataLength + 1 );
    }
    else
    {
        if( _disposer ) ( this->*_disposer )( );
        _bufferLength = (int)Utils::round2n( other._dataLength + 1 );
        _disposer = &String::disposeNewBuffer;
        std::cout << "String& String::operator=( String const & other ) new char[ " << _bufferLength << " ]\n";
        _buffer = new char[ _bufferLength ];
        _dataLength = other._dataLength;
        memcpy( _buffer, other._buffer, other._dataLength + 1 );
    }
    return *this;
}

String& String::operator=( String && other )
{
    if( _disposer ) ( this->*_disposer )( );
    _buffer = other._buffer;
    _bufferLength = other._bufferLength;
    _dataLength = other._dataLength;
    _disposer = other._disposer;
    //other._buffer = nullptr;  // 这三行不必要
    //other._bufferLength = 0;
    //other._dataLength = 0;
    other._disposer = nullptr;
    return *this;
}
