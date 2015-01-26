#include "All.h"

namespace xxx
{
    List<bool>::List( int capacity )
    {
        assert( capacity > 0 );
        auto len = int( capacity / 8 );
        if( len < 64 ) len = 64;
        else len = (int)Round2n( len );
        size = 0;
        buf = new char[ len ];
        maxSize = len * 8;
    }

    List<bool>::~List()
    {
        if( buf )
        {
            Clear();
            delete[] buf;
        }
    }

    List<bool>::List( List<bool> && o )
        : buf( o.buf )
        , size( o.size )
        , maxSize( o.maxSize )
    {
        o.buf = nullptr;
    }

    List<bool>::List( List<bool> const & o )
        : List<bool>( o.size )
    {
        memcpy( buf, o.buf, o.ByteSize() );
    }

    List<bool>& List<bool>::operator=( List<bool> && o )
    {
        delete[] buf;
        buf = o.buf;
        size = o.size;
        maxSize = o.maxSize;
        o.buf = nullptr;
        return *this;
    }

    List<bool>& List<bool>::operator=( List<bool> const & o )
    {
        if( this == &o ) return *this;
        size = o.size;
        if( maxSize < o.size )
        {
            auto len = int( o.size / 8 );
            if( len < 64 ) len = 64;
            else len = (int)Round2n( len );
            maxSize = len * 8;
            delete[] buf;
            buf = new char[ len ];
        }
        memcpy( buf, o.buf, o.ByteSize() );
        return *this;
    }

    void List<bool>::Push( bool v )
    {
        if( size == maxSize ) Reserve( size + 1 );
        Set( size++, v );
    }

    void List<bool>::Pop()
    {
        assert( size > 0 );
        --size;
    }

    bool List<bool>::Top() const
    {
        assert( size > 0 );
        return operator[]( size - 1 );
    }

    void List<bool>::Clear()
    {
        size = 0;
    }

    void List<bool>::Reserve( int capacity )
    {
        assert( capacity > 0 );
        if( capacity <= maxSize ) return;
        auto len = (int)Round2n( ( capacity - 1 ) / 8 + 1 );
        maxSize = len * 8;
        auto newBuf = new char[ len ];
        memcpy( newBuf, buf, ByteSize() );
        delete[] buf;
        buf = newBuf;
    }

    char* List<bool>::Data() const
    {
        return buf;
    }

    int List<bool>::Size() const
    {
        return size;
    }

    int List<bool>::ByteSize() const
    {
        if( size ) return ( size - 1 ) / 8 + 1;
        return 0;
    }

    bool List<bool>::operator[]( int idx ) const
    {
        return At( idx );
    }

    bool List<bool>::At( int idx ) const
    {
        return ( ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] &
                 ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) ) ) > 0;
    }

    void List<bool>::Set( int idx, bool v )
    {
        assert( idx >= 0 && idx < size );
        if( v ) SetTrue( idx );
        else SetFalse( idx );
    }

    void List<bool>::SetTrue( int idx )
    {
        assert( idx >= 0 && idx < size );
        ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] |=
            ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    }

    void List<bool>::SetFalse( int idx )
    {
        assert( idx >= 0 && idx < size );
        ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] &=
            ~( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    }

    void List<bool>::FillTrue()
    {
        memset( buf, 0xFFFFFFFFu, ( size - 1 ) / 8 + 1 );
    }

    void List<bool>::FillFalse()
    {
        memset( buf, 0, ( size - 1 ) / 8 + 1 );
    }

    void List<bool>::Fill( bool v, int idxFrom, int idxTo )
    {
        assert( size > 0 && idxFrom >= 0 && idxTo >= 0 && idxFrom < size && idxTo < size );
        if( idxFrom == idxTo )
        {
            Set( idxFrom, v );
            return;
        }
        if( idxFrom > idxTo )
        {
            std::swap( idxFrom, idxTo );
        }
        auto byteIdxFrom = idxFrom >> 3;
        auto byteIdxTo = idxTo >> 3;

        if( byteIdxFrom == byteIdxTo )
        {
            // ��һ�� �м�һ���� v �� byte ����
            if( v )
            {
                buf[ byteIdxFrom ] |= (byte)0xFFu >> ( 7 - ( idxTo - idxFrom ) ) << ( idxFrom & 7 );
            }
            else
            {
                buf[ byteIdxFrom ] &= ~( (byte)0xFFu >> ( 7 - ( idxTo - idxFrom ) ) << ( idxFrom & 7 ) );
            }
        }
        else
        {
            // �ֱ��һͷһβ, �� memset �м�
            auto idxFrom7 = idxFrom & 7;
            auto idxTo7 = idxTo & 7;
            if( v )
            {
                buf[ byteIdxFrom ] |= (byte)0xFFu << idxFrom7;
                buf[ byteIdxTo ] |= (byte)0xFFu >> ( 7 - idxTo7 );
            }
            else
            {
                buf[ byteIdxFrom ] &= ~( (byte)0xFFu << idxFrom7 );
                buf[ byteIdxTo ] &= ~( (byte)0xFFu >> ( 7 - idxTo7 ) );
            }
            if( idxFrom7 ) ++byteIdxFrom;
            if( idxTo7 ) --byteIdxTo;
            if( byteIdxFrom <= byteIdxTo )
            {
                memset( buf + byteIdxFrom, v ? 0xFFFFFFFFu : 0, byteIdxTo - byteIdxFrom );
            }
        }
    }

    void List<bool>::Resize( int capacity, bool init )
    {
        if( capacity == size ) return;
        if( capacity < size )
        {
            size = capacity;
            return;
        }
        Reserve( capacity );
        auto oldSize = size;
        size = capacity;
        if( init )
        {
            Fill( false, oldSize, capacity - 1 );
        }
    }
}
