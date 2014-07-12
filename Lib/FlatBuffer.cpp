#include "All.h"


FlatBuffer::FlatBuffer( int capacity )
{
    if( capacity < 1024 ) _bufLen = 1024;
    else _bufLen = (int)Utils::round2n( capacity );
    _disposer = &FlatBuffer::disposeNewBuffer;
    _buf = new char[ _bufLen ];
    _dataLen = _offset = 0;
}

FlatBuffer::FlatBuffer( char* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
{
    _dataLen = dataLen;
    _offset = 0;
    if( isRef )
    {
        _buf = buf;
        _bufLen = bufLen;
        _disposer = nullptr;
    }
    else
    {
        _bufLen = (int)Utils::round2n( dataLen );
        _buf = new char[ _bufLen ];
        memcpy( _buf, buf, dataLen );
        _disposer = &FlatBuffer::disposeNewBuffer;
    }
}

FlatBuffer::FlatBuffer( FlatBuffer const & other )
    : FlatBuffer( other._dataLen + 1 )
{
    memcpy( _buf, other._buf, other._dataLen + 1 );
    _dataLen = other._dataLen;
}

FlatBuffer::FlatBuffer( FlatBuffer && other )
    : _buf( other._buf )
    , _bufLen( other._bufLen )
    , _dataLen( other._dataLen )
    , _offset( other._offset )
    , _disposer( other._disposer )
{
    other._buf = nullptr;
    other._bufLen = 0;
    other._dataLen = 0;
    other._offset = 0;
    other._disposer = nullptr;
}

FlatBuffer::~FlatBuffer()
{
    if( _disposer ) ( this->*_disposer )( );
}

void FlatBuffer::clear()
{
    _dataLen = _offset = 0;
}

bool FlatBuffer::empty() const
{
    return _dataLen == _offset;
}

int FlatBuffer::size() const
{
    return _dataLen - _offset;
}

char const* FlatBuffer::data() const
{
    return _buf;
}

char* FlatBuffer::data()
{
    return _buf;
}

void FlatBuffer::assign( char const* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
{
    assert( _disposer && _buf != buf );
    if( isRef )
    {
        if( _disposer ) ( this->*_disposer )( );
        _buf = (char*)buf;
        _bufLen = bufLen;
        _dataLen = dataLen;
        _offset = 0;
        _disposer = nullptr;
        return;
    }
    if( _bufLen >= dataLen )
    {
        _dataLen = dataLen;
        _offset = 0;
        memcpy( _buf, buf, dataLen );
    }
    else
    {
        if( _disposer ) ( this->*_disposer )( );
        _bufLen = (int)Utils::round2n( dataLen );
        _buf = new char[ _bufLen ];
        _dataLen = dataLen;
        _offset = 0;
        _disposer = &FlatBuffer::disposeNewBuffer;
        memcpy( _buf, buf, dataLen );
    }
}

void FlatBuffer::reserve( int capacity )
{
    if( capacity <= _bufLen ) return;
    _bufLen = (int)Utils::round2n( capacity );
    auto newBuf = new char[ _bufLen ];
    memcpy( newBuf, _buf, _dataLen );
    if( _disposer ) ( this->*_disposer )( );
    _disposer = &FlatBuffer::disposeNewBuffer;
    _buf = newBuf;
}

void FlatBuffer::resize( int widx )
{
    if( widx == _dataLen ) return;
    else if( widx < _dataLen )
    {
        _dataLen = widx;
        if( _dataLen > _offset ) _offset = _dataLen;
    }
    else
    {
        reserve( widx );
        _dataLen = widx;
    }
}

FlatBuffer& FlatBuffer::operator=( FlatBuffer const& other )
{
    if( this == &other ) return *this;
    if( _bufLen >= other._dataLen )
    {
        _dataLen = other._dataLen;
        _offset = 0;
        memcpy( _buf, other._buf, other._dataLen );
    }
    else
    {
        if( _disposer ) ( this->*_disposer )( );
        _bufLen = (int)Utils::round2n( other._dataLen );
        _disposer = &FlatBuffer::disposeNewBuffer;
        _buf = new char[ _bufLen ];
        _dataLen = other._dataLen;
        memcpy( _buf, other._buf, other._dataLen );
        _offset = 0;
    }
    return *this;
}

FlatBuffer& FlatBuffer::operator=( FlatBuffer&& other )
{
    if( _disposer ) ( this->*_disposer )( );
    _buf = other._buf;
    _bufLen = other._bufLen;
    _dataLen = other._dataLen;
    _offset = 0;
    _disposer = other._disposer;
    other._buf = nullptr;
    other._bufLen = 0;
    other._dataLen = 0;
    other._offset = 0;
    other._disposer = nullptr;
    return *this;
}

char FlatBuffer::operator[]( int idx ) const
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

char& FlatBuffer::operator[]( int idx )
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

char FlatBuffer::at( int idx ) const
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

char& FlatBuffer::at( int idx )
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

int& FlatBuffer::offset()
{
    return _offset;
}

int FlatBuffer::offset() const
{
    return _offset;
}

bool FlatBuffer::read( char* buf, int dataLen )
{
    if( _offset + dataLen > _dataLen ) return false;
    memcpy( buf, _buf + _offset, dataLen );
    _offset += dataLen;
    return true;
}

void FlatBuffer::write( char const* buf, int dataLen )
{
    reserve( _dataLen + dataLen );
    memcpy( _buf + _dataLen, buf, dataLen );
    _dataLen += dataLen;
}

void FlatBuffer::disposeNewBuffer()
{
    delete[] _buf;
}

String FlatBuffer::dump()
{
    String rtv;
    Utils::binaryDump( rtv, _buf, _dataLen );
    return rtv;
}
