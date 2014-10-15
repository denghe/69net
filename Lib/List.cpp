#include "All.h"

namespace xxx
{


    List<bool>::List( int capacity )
    {
        assert( capacity > 0 );
        auto len = int( capacity / 8 );
        if( len < 64 ) len = 64;
        else len = (int)Round2n( len );
        _size = 0;
        _buf = new char[ len ];
        _maxSize = len * 8;
    }

    List<bool>::~List()
    {
        if( _buf )
        {
            Clear();
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
        memcpy( _buf, other._buf, other.ByteSize() );
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
            else len = (int)Round2n( len );
            _maxSize = len * 8;
            delete[] _buf;
            _buf = new char[ len ];
        }
        memcpy( _buf, other._buf, other.ByteSize() );
        return *this;
    }

    void List<bool>::Push( bool v )
    {
        if( _size == _maxSize ) Reserve( _size + 1 );
        Set( _size++, v );
    }

    void List<bool>::Pop()
    {
        assert( _size > 0 );
        --_size;
    }

    bool List<bool>::Top() const
    {
        assert( _size > 0 );
        return operator[]( _size - 1 );
    }

    void List<bool>::Clear()
    {
        _size = 0;
    }

    void List<bool>::Reserve( int capacity )
    {
        assert( capacity > 0 );
        if( capacity <= _maxSize ) return;
        auto len = (int)Round2n( ( capacity - 1 ) / 8 + 1 );
        _maxSize = len * 8;
        auto newBuf = new char[ len ];
        memcpy( newBuf, _buf, ByteSize() );
        delete[] _buf;
        _buf = newBuf;
    }

    char* List<bool>::Data() const
    {
        return _buf;
    }

    int List<bool>::Size() const
    {
        return _size;
    }

    int List<bool>::ByteSize() const
    {
        if( _size ) return ( _size - 1 ) / 8 + 1;
        return 0;
    }

    bool List<bool>::operator[]( int idx ) const
    {
        return At( idx );
    }

    bool List<bool>::At( int idx ) const
    {
        return ( ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] &
                 ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) ) ) > 0;
    }

    void List<bool>::Set( int idx, bool v )
    {
        assert( idx >= 0 && idx < _size );
        if( v )
            ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] |=
            ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
        else
            ( (size_t*)_buf )[ idx / ( sizeof( size_t ) * 8 ) ] &=
            ~( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    }


}