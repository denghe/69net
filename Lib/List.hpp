#ifndef _LIST_HPP__
#define _LIST_HPP__

namespace xxx
{


    template<typename T>
    List<T>::List( int capacity )
    {
        assert( capacity > 0 );
        auto byteLen = int( capacity * sizeof( T ) );
        if( byteLen < 64 ) byteLen = 64;
        else byteLen = (int)Round2n( byteLen );
        size = 0;
        buf = ( T* )new char[ byteLen ];
        maxSize = byteLen / sizeof( T );
    }

    template<typename T>
    List<T>::~List()
    {
        if( buf )
        {
            Clear();
            delete[]( char* )buf;
        }
    }

    template<typename T>
    List<T>::List( List&& o )
        : buf( o.buf )
        , size( o.size )
        , maxSize( o.maxSize )
    {
        o.buf = nullptr;
        o.size = 0;
        o.maxSize = 0;
    }

    template<typename T>
    List<T>::List( List const& o )
        : List( o.size )
    {
        if( std::is_pod<T>::value )
            memcpy( buf, o.buf, o.ByteSize() );
        else
            for( int i = 0; i < o.size; ++i )
                new ( buf + i ) T( o.buf[ i ] );
        size = o.size;
    }

    template<typename T>
    List<T>& List<T>::operator=( List&& o )
    {
        Clear();
        delete[]( char* )buf;
        buf = o.buf;
        size = o.size;
        maxSize = o.maxSize;
        o.buf = nullptr;
        o.size = 0;
        o.maxSize = 0;
        return *this;
    }

    template<typename T>
    List<T>& List<T>::operator=( List const& o )
    {
        if( this == &o ) return *this;
        Clear();
        size = o.size;
        if( maxSize < o.size )
        {
            auto byteLen = int( o.size * sizeof( T ) );
            if( byteLen < 64 ) byteLen = 64;
            else byteLen = (int)Round2n( byteLen );
            maxSize = byteLen / sizeof( T );
            delete[]( char* )buf;
            buf = ( T* )new char[ byteLen ];
        }
        if( std::is_pod<T>::value )
            memcpy( buf, o.buf, o.ByteSize() );
        else
            for( int i = 0; i < o.size; ++i )
                new ( buf + i ) T( o.buf[ i ] );
        return *this;
    }

    template<typename T>
    template<typename VT>
    void List<T>::Push( VT&& v )
    {
        Emplace( std::forward<VT>( v ) );
    }

    template<typename T>
    template<typename VT>
    void List<T>::PushFast( VT&& v )
    {
        new ( buf + size++ ) T( std::forward<VT>( v ) );
    }

    template<typename T>
    template<typename ...PTS>
    T& List<T>::Emplace( PTS&& ...ps )
    {
        if( size == maxSize ) Reserve( size + 1 );
        return *new ( buf + size++ ) T( std::forward<PTS>( ps )... );
    }


    template<typename T>
    void List<T>::Pop()
    {
        assert( size > 0 );
        buf[ --size ].~T();
    }

    template<typename T>
    T& List<T>::Top()
    {
        assert( size > 0 );
        return buf[ size - 1 ];
    }
    template<typename T>
    T const& List<T>::Top() const
    {
        assert( size > 0 );
        return buf[ size - 1 ];
    }

    template<typename T>
    T& List<T>::TopPop()
    {
        assert( size > 0 );
        return buf[ --size ];
    }
    template<typename T>
    T const& List<T>::TopPop() const
    {
        assert( size > 0 );
        return buf[ --size ];
    }

    template<typename T>
    void List<T>::Clear()
    {
        for( int i = 0; i < size; ++i )
            buf[ i ].~T();
        size = 0;
    }

    template<typename T>
    void List<T>::Reserve( int capacity )
    {
        assert( capacity > 0 );
        if( capacity <= maxSize ) return;
        auto byteLen = (int)Round2n( capacity * sizeof( T ) );
        maxSize = byteLen / sizeof( T );
        auto newBuf = ( T* )new char[ byteLen ];
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
            memcpy( newBuf, buf, ByteSize() );
        else
            for( int i = 0; i < size; ++i )
                new ( newBuf + i ) T( std::move( buf[ i ] ) );
        delete[]( char* )buf;
        buf = newBuf;
    }

    template<typename T>
    void List<T>::Resize( int capacity, bool Init /*= true */ )
    {
        if( capacity == size ) return;
        else if( capacity < size )
        {
            if( !std::is_pod<T>::value )
                for( int i = capacity; i < size; ++i )
                    buf[ i ].~T();
        }
        else
        {
            Reserve( capacity );
            if( Init )
            {

                if( std::is_pod<T>::value )
                    memset( buf + size, 0, ( capacity - size ) * sizeof( T ) );
                else
                    for( int i = size; i < capacity; ++i )
                        new ( buf + i ) T();
            }
        }
        size = capacity;
    }

    template<typename T>
    T* List<T>::Data() const
    {
        return buf;
    }

    template<typename T>
    int List<T>::Size() const
    {
        return size;
    }

    template<typename T>
    int& List<T>::Size()
    {
        return size;
    }

    template<typename T>
    int List<T>::ByteSize() const
    {
        return size * sizeof( T );
    }

    template<typename T>
    int List<T>::ByteMaxSize() const
    {
        return maxSize * sizeof( T );
    }

    template<typename T>
    T const & List<T>::operator[]( int idx ) const
    {
        assert( idx < size && idx >= 0 );
        return buf[ idx ];
    }

    template<typename T>
    T& List<T>::operator[]( int idx )
    {
        assert( idx < size && idx >= 0 );
        return buf[ idx ];
    }

    template<typename T>
    T const & List<T>::At( int idx ) const
    {
        assert( idx < size && idx >= 0 );
        return buf[ idx ];
    }

    template<typename T>
    T& List<T>::At( int idx )
    {
        assert( idx < size && idx >= 0 );
        return buf[ idx ];
    }

    template<typename T>
    template<typename VT>
    void List<T>::Set( int idx, VT&& v )
    {
        assert( idx >= 0 && idx < size );
        if( std::is_pod<T>::value )
            buf[ idx ] = v;
        else
            buf[ idx ] = std::forward<VT>( v );
    }

    template<typename T>
    int List<T>::Find( T const& v )
    {
        for( int i = 0; i < size; ++i )
        {
            if( buf[ i ] == v ) return i;
        }
        return -1;
    }
    template<typename T>
    void List<T>::Erase( int idx )
    {
        assert( idx >= 0 && idx < size );
        --size;
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            memmove( buf + idx, buf + idx + 1, ( size - idx )*sizeof( T ) );
        }
        else
        {
            buf[ idx ].~T();
            for( int i = idx; i < size; ++i )
            {
                new ( buf + i ) T( std::move( buf[ i + 1 ] ) );
            }
        }
    }
    template<typename T>
    void List<T>::EraseFast( int idx )
    {
        assert( idx >= 0 && idx < size );
        --size;
        if( size == idx )
        {
            buf[ idx ].~T();
            return;
        }
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            buf[ idx ] = buf[ size ];
        }
        else
        {
            buf[ idx ].~T();
            new ( buf + idx ) T( std::move( buf[ size ] ) );
        }
    }

    template<typename T>
    template<typename VT>
    void List<T>::InsertAt( int idx, VT&& v )
    {
        EmplaceAt( idx, std::forward<VT>( v ) );
    }
    template<typename T>
    template<typename ...PTS>
    T& List<T>::EmplaceAt( int idx, PTS&& ...ps )
    {
        assert( idx >= 0 && idx <= size );
        Reserve( size + 1 );                       // todo: 理论上讲这句可以展开，于扩容时直接将 Insert 的元素的内存位置留出来

        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            memmove( buf + idx + 1, buf + idx, ( size - idx )*sizeof( T ) );
            ++size;
            return *new ( buf + idx ) T( std::forward<PTS>( ps )... );
        }
        else
        {
            for( int i = size; i > idx; --i )
            {
                new ( buf + i ) T( std::move( buf[ i - 1 ] ) );
            }
            ++size;
            return *new ( buf + idx ) T( std::forward<PTS>( ps )... );
        }
    }
























    template<typename T>
    int List<T>::GetWriteBufferSize() const
    {
        if( std::is_pod<T>::value )
        {
            return sizeof( int ) + sizeof( T ) * size;
        }

        int siz = sizeof( int );
        for( int i = 0; i < size; ++i )
        {
            siz += BufferUtils::GetSize( buf[ i ] );
        }
        return siz;
    }

    template<typename T>
    void List<T>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( size );
        if( std::is_pod<T>::value )
        {
            fb.Write( (char*)buf, size * sizeof( T ) );
            return;
        }
        for( int i = 0; i < size; ++i )
        {
            fb.Write( buf[ i ] );
        }
    }

    template<typename T>
    void List<T>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( size );
        if( std::is_pod<T>::value )
        {
            fb.WriteDirect( (char*)buf, size * sizeof( T ) );
            return;
        }
        for( int i = 0; i < size; ++i )
        {
            fb.WriteDirect( buf[ i ] );
        }
    }


    template<typename T>
    bool List<T>::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0 ) return false;
        if( std::is_pod<T>::value )
        {
            int siz = len * ( int )sizeof( T );
            if( fb.Offset() + siz > fb.Size() ) return false;
            Clear();
            Resize( len, false );
            memcpy( buf, fb.Data() + fb.Offset(), siz );
            fb.Offset() += siz;
            return true;
        }
        Clear();
        Reserve( len );
        for( int i = 0; i < len; ++i )
        {
            new ( buf + i ) T();
            size = i + 1;
            if( !buf[ i ].ReadBuffer( fb ) ) return false;
        }
        return true;
    }


}

#endif
