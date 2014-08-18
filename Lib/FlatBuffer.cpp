#include "All.h"

Pool FlatBuffer::_emptyPool;     // for do not need delete's buffer

FlatBuffer::FlatBuffer( int capacity )
{
    if( capacity < 1024 ) _bufLen = 1024;
    else _bufLen = (int)Utils::round2n( capacity );
    _disposer = nullptr;
    _buf = new char[ _bufLen ];
    _dataLen = _offset = 0;
}

FlatBuffer::FlatBuffer( Pool& p )
{
    _buf = (char*)p.alloc();
    _bufLen = p.itemBufLen();
    _dataLen = 0;
    _disposer = &p;
}

FlatBuffer::FlatBuffer( char* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
{
    _dataLen = dataLen;
    _offset = 0;
    if( isRef )
    {
        _buf = buf;
        _bufLen = bufLen;
        _disposer = &_emptyPool;
    }
    else
    {
        _bufLen = (int)Utils::round2n( dataLen );
        _buf = new char[ _bufLen ];
        memcpy( _buf, buf, dataLen );
        _disposer = nullptr;
    }
}

FlatBuffer::FlatBuffer( FlatBuffer const & other )
    : FlatBuffer( other._dataLen + 1 )
{
    memcpy( _buf, other._buf, other._dataLen + 1 );
    _dataLen = other._dataLen;
}

FlatBuffer::FlatBuffer( FlatBuffer&& other )
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
    dispose();
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
    return _dataLen;
}

int& FlatBuffer::size()
{
    return _dataLen;
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
    assert( _buf != buf );
    _offset = 0;
    if( isRef )
    {
        dispose();
        _buf = (char*)buf;
        _bufLen = bufLen;
        _dataLen = dataLen;
        _disposer = &_emptyPool;
        return;
    }
    if( _bufLen < dataLen )
    {
        dispose();
        _bufLen = (int)Utils::round2n( dataLen );
        _disposer = nullptr;
        _buf = new char[ _bufLen ];
    }
    _dataLen = dataLen;
    memcpy( _buf, buf, dataLen );
}

void FlatBuffer::reserve( int capacity )
{
    if( capacity <= _bufLen ) return;
    _bufLen = (int)Utils::round2n( capacity );
    auto newBuf = new char[ _bufLen ];
    memcpy( newBuf, _buf, _dataLen );
    dispose();
    _disposer = nullptr;
    _buf = newBuf;
}

void FlatBuffer::resize( int dataLen )
{
    if( dataLen == _dataLen ) return;
    else if( dataLen < _dataLen )
    {
        _dataLen = dataLen;
        if( _dataLen > _offset ) _offset = _dataLen;
    }
    else
    {
        reserve( dataLen );
        _dataLen = dataLen;
    }
}

FlatBuffer& FlatBuffer::operator=( FlatBuffer const& other )
{
    if( this == &other ) return *this;
    _offset = 0;
    _dataLen = other._dataLen;
    if( _bufLen >= other._dataLen )
    {
        memcpy( _buf, other._buf, other._dataLen );
    }
    else
    {
        dispose();
        _bufLen = (int)Utils::round2n( other._dataLen );
        _disposer = nullptr;
        _buf = new char[ _bufLen ];
        memcpy( _buf, other._buf, other._dataLen );
    }
    return *this;
}

FlatBuffer& FlatBuffer::operator=( FlatBuffer&& other )
{
    dispose();
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

String FlatBuffer::dump()
{
    String rtv;
    BufferUtils::dump( rtv, _buf, _dataLen );
    return rtv;
}



int FlatBuffer::getWriteBufferSize()
{
    return sizeof( int ) + size();
}
void FlatBuffer::writeBuffer( FlatBuffer& fb )
{
    fb.write( _dataLen );
    fb.write( _buf, _dataLen );
}
void FlatBuffer::writeBufferDirect( FlatBuffer& fb )
{
    fb.writeDirect( _dataLen );
    fb.writeDirect( _buf, _dataLen );
}
bool FlatBuffer::readBuffer( FlatBuffer& fb )
{
    int len;
    if( !fb.read( len ) || len < 0
        || fb.offset() + len > fb.size() ) return false;
    clear();
    reserve( len );
    _dataLen = len;
    memcpy( _buf, fb.data() + fb.offset(), len );
    fb.offset() += len;
    return true;
}



void FlatBuffer::writeDirect( char const* buf, int dataLen )
{
    memcpy( _buf + _dataLen, buf, dataLen );
    _dataLen += dataLen;
}
void FlatBuffer::write( char const* buf, int dataLen )
{
    reserve( _dataLen + dataLen );
    writeDirect( buf, dataLen );
}

bool FlatBuffer::read( char* buf, int dataLen )
{
    if( _offset + dataLen > _dataLen ) return false;
    memcpy( buf, _buf + _offset, dataLen );
    _offset += dataLen;
    return true;
}

void FlatBuffer::dispose()
{
    if( _disposer )
    {
        if( _disposer->itemBufLen() ) _disposer->free( _buf );
    }
    else
    {
        delete[] _buf;
    }
}
