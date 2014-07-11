#include "All.h"


FlatBuffer::FlatBuffer( int capacity )
{
    if( capacity < 1024 ) _bufLen = 1024;
    else _bufLen = (int)Utils::round2n( capacity );
    _disposer = &FlatBuffer::disposeNewBuffer;
    _buf = new char[ _bufLen ];
    _widx = _ridx = 0;
}

FlatBuffer::FlatBuffer( char* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
{
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
    _widx = dataLen;
    _ridx = 0;
}

FlatBuffer::FlatBuffer( FlatBuffer const & other )
{

}

FlatBuffer::FlatBuffer( FlatBuffer && other )
{

}

FlatBuffer::~FlatBuffer()
{

}

void FlatBuffer::clear()
{

}

bool FlatBuffer::empty() const
{

}

int FlatBuffer::size() const
{

}

char* FlatBuffer::data() const
{

}

void FlatBuffer::reserve( int capacity )
{

}

void FlatBuffer::write( char const* buf, int len )
{
    reserve( _widx + len );
    memcpy( _buf + _widx, buf, len );
    _widx += len;
}

FlatBuffer& FlatBuffer::operator=( FlatBuffer const & other )
{

}

FlatBuffer& FlatBuffer::operator=( FlatBuffer && other )
{

}

char& FlatBuffer::operator[]( int idx ) const
{

}

char& FlatBuffer::operator[]( int idx )
{

}

char FlatBuffer::at( int idx ) const
{

}

char& FlatBuffer::at( int idx )
{

}

void FlatBuffer::set( int idx, char v )
{

}

int& FlatBuffer::widx()
{

}

int& FlatBuffer::ridx()
{

}

int FlatBuffer::read( char* buf, int len )
{

}
