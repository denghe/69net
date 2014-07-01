#include "Lib/All.h"
#include "List.h"

List<bool>::List( int capacity )
{
    assert( capacity > 0 );
    auto len = int( capacity / 8 );
    if( len < 64 ) len = 64;
    else len = (int)Utils::round2n( len );
    _size = 0;
    _buf = new char[ len ];
    _maxSize = len * 8;
}

List<bool>::~List()
{
    if( _buf )
    {
        clear();
        delete[] _buf;
    }
}

List<bool>::List( List<bool> && other )
    : _buf( other._buf )
    , _size( other._size )
    , _maxSize( other._maxSize )
{
    other._buf = nullptr;
}

List<bool>::List( List<bool> const & other )
    : List<bool>( other._size )
{
    memcpy( _buf, other._buf, other.byteSize() );
}

List<bool>& List<bool>::operator=( List<bool> && other )
{
    delete[] _buf;
    _buf = other._buf;
    _size = other._size;
    _maxSize = other._maxSize;
    other._buf = nullptr;
    return *this;
}

List<bool>& List<bool>::operator=( List<bool> const & other )
{
    if( this == &other ) return *this;
    _size = other._size;
    if( _maxSize < other._size )
    {
        auto len = int( other._size / 8 );
        if( len < 64 ) len = 64;
        else len = (int)Utils::round2n( len );
        _maxSize = len * 8;
        delete[] _buf;
        _buf = new char[ len ];
    }
    memcpy( _buf, other._buf, other.byteSize() );
    return *this;
}

void List<bool>::push( bool v )
{
    if( _size == _maxSize ) reserve( _size + 1 );
    set( _size++, v );
}

void List<bool>::pop()
{
    assert( _size > 0 );
    --_size;
}

bool List<bool>::top() const
{
    assert( _size > 0 );
    return operator[]( _size - 1 );
}

void List<bool>::clear()
{
    _size = 0;
}

void List<bool>::reserve( int capacity )
{
    assert( capacity > 0 );
    if( capacity <= _maxSize ) return;
    auto len = (int)Utils::round2n( ( capacity - 1 ) / 8 + 1 );
    _maxSize = len * 8;
    auto newBuffer = new char[ len ];
    memcpy( newBuffer, _buf, byteSize() );
    delete[] _buf;
    _buf = newBuffer;
}

char* List<bool>::data() const
{
    return _buf;
}

int List<bool>::size() const
{
    return _size;
}

int List<bool>::byteSize() const
{
    if( _size ) return ( _size - 1 ) / 8 + 1;
    return 0;
}

bool List<bool>::operator[]( int idx ) const
{
    return at( idx );
}

bool List<bool>::at( int idx ) const
{
    return ( ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] &
             ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) ) ) > 0;
}

void List<bool>::set( int idx, bool v )
{
    assert( idx >= 0 && idx < _size );
    if( v )
        ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] |=
        ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    else
        ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] &=
        ~( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
}

