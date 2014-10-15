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
        _size = 0;
        _buf = ( T* )new char[ byteLen ];
        _maxSize = byteLen / sizeof( T );
    }

    template<typename T>
    List<T>::~List()
    {
        if( _buf )
        {
            Clear();
            delete[]( char* )_buf;
        }
    }

    template<typename T>
    List<T>::List( List&& other )
        : _buf( other._buf )
        , _size( other._size )
        , _maxSize( other._maxSize )
    {
        other._buf = nullptr;
        other._size = 0;
        other._maxSize = 0;
    }

    template<typename T>
    List<T>::List( List const& other )
        : List( other._size )
    {
        if( std::is_pod<T>::value )
            memcpy( _buf, other._buf, other.ByteSize() );
        else
            for( int i = 0; i < other._size; ++i )
                new ( _buf + i ) T( other._buf[ i ] );
        _size = other._size;
    }

    template<typename T>
    List<T>& List<T>::operator=( List&& other )
    {
        Clear();
        delete[]( char* )_buf;
        _buf = other._buf;
        _size = other._size;
        _maxSize = other._maxSize;
        other._buf = nullptr;
        other._size = 0;
        other._maxSize = 0;
        return *this;
    }

    template<typename T>
    List<T>& List<T>::operator=( List const& other )
    {
        if( this == &other ) return *this;
        Clear();
        _size = other._size;
        if( _maxSize < other._size )
        {
            auto byteLen = int( other._size * sizeof( T ) );
            if( byteLen < 64 ) byteLen = 64;
            else byteLen = (int)Round2n( byteLen );
            _maxSize = byteLen / sizeof( T );
            delete[]( char* )_buf;
            _buf = ( T* )new char[ byteLen ];
        }
        if( std::is_pod<T>::value )
            memcpy( _buf, other._buf, other.ByteSize() );
        else
            for( int i = 0; i < other._size; ++i )
                new ( _buf + i ) T( other._buf[ i ] );
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
        new ( _buf + _size++ ) T( std::forward<VT>( v ) );
    }

    template<typename T>
    template<typename ...PTS>
    T& List<T>::Emplace( PTS&& ...ps )
    {
        if( _size == _maxSize ) Reserve( _size + 1 );
        return *new ( _buf + _size++ ) T( std::forward<PTS>( ps )... );
    }


    template<typename T>
    void List<T>::Pop()
    {
        assert( _size > 0 );
        _buf[ --_size ].~T();
    }

    template<typename T>
    T& List<T>::Top()
    {
        assert( _size > 0 );
        return _buf[ _size - 1 ];
    }
    template<typename T>
    T const& List<T>::Top() const
    {
        assert( _size > 0 );
        return _buf[ _size - 1 ];
    }

    template<typename T>
    T& List<T>::TopPop()
    {
        assert( _size > 0 );
        return _buf[ --_size ];
    }
    template<typename T>
    T const& List<T>::TopPop() const
    {
        assert( _size > 0 );
        return _buf[ --_size ];
    }

    template<typename T>
    void List<T>::Clear()
    {
        for( int i = 0; i < _size; ++i )
            _buf[ i ].~T();
        _size = 0;
    }

    template<typename T>
    void List<T>::Reserve( int capacity )
    {
        assert( capacity > 0 );
        if( capacity <= _maxSize ) return;
        auto byteLen = (int)Round2n( capacity * sizeof( T ) );
        _maxSize = byteLen / sizeof( T );
        auto newBuf = ( T* )new char[ byteLen ];
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
            memcpy( newBuf, _buf, ByteSize() );
        else
            for( int i = 0; i < _size; ++i )
                new ( newBuf + i ) T( std::move( _buf[ i ] ) );
        delete[]( char* )_buf;
        _buf = newBuf;
    }

    template<typename T>
    void List<T>::Resize( int capacity, bool Init /*= true */ )
    {
        if( capacity == _size ) return;
        else if( capacity < _size )
        {
            if( !std::is_pod<T>::value )
                for( int i = capacity; i < _size; ++i )
                    _buf[ i ].~T();
        }
        else
        {
            Reserve( capacity );
            if( Init )
            {

                if( std::is_pod<T>::value )
                    memset( _buf + _size, 0, ( capacity - _size ) * sizeof( T ) );
                else
                    for( int i = _size; i < capacity; ++i )
                        new ( _buf + i ) T();
            }
        }
        _size = capacity;
    }

    template<typename T>
    T* List<T>::Data() const
    {
        return _buf;
    }

    template<typename T>
    int List<T>::Size() const
    {
        return _size;
    }

    template<typename T>
    int& List<T>::Size()
    {
        return _size;
    }

    template<typename T>
    int List<T>::ByteSize() const
    {
        return _size * sizeof( T );
    }

    template<typename T>
    int List<T>::ByteMaxSize() const
    {
        return _maxSize * sizeof( T );
    }

    template<typename T>
    T const & List<T>::operator[]( int idx ) const
    {
        return _buf[ idx ];
    }

    template<typename T>
    T& List<T>::operator[]( int idx )
    {
        return _buf[ idx ];
    }

    template<typename T>
    T const & List<T>::At( int idx ) const
    {
        return _buf[ idx ];
    }

    template<typename T>
    T& List<T>::At( int idx )
    {
        return _buf[ idx ];
    }

    template<typename T>
    template<typename VT>
    void List<T>::Set( int idx, VT&& v )
    {
        assert( idx >= 0 && idx < _size );
        if( std::is_pod<T>::value )
            _buf[ idx ] = v;
        else
            _buf[ idx ] = std::forward<VT>( v );
    }

    template<typename T>
    int List<T>::Find( T const& v )
    {
        for( int i = 0; i < _size; ++i )
        {
            if( _buf[ i ] == v ) return i;
        }
        return -1;
    }
    template<typename T>
    void List<T>::Erase( int idx )
    {
        assert( idx >= 0 && idx < _size );
        --_size;
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            memmove( _buf + idx, _buf + idx + 1, ( _size - idx )*sizeof( T ) );
        }
        else
        {
            _buf[ idx ].~T();
            for( int i = idx; i < _size; ++i )
            {
                new ( _buf + i ) T( std::move( _buf[ i + 1 ] ) );
            }
        }
    }
    template<typename T>
    void List<T>::EraseFast( int idx )
    {
        assert( idx >= 0 && idx < _size );
        --_size;
        if( !_size ) return;
        if( _size == idx )
        {
            _buf[ idx ].~T();
            return;
        }
        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            _buf[ idx ] = _buf[ _size ];
        }
        else
        {
            _buf[ idx ].~T();
            new ( _buf + idx ) T( std::move( _buf[ _size ] ) );
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
        assert( idx >= 0 && idx <= _size );
        Reserve( _size + 1 );                       // todo: 理论上讲这句可以展开，于扩容时直接将 Insert 的元素的内存位置留出来

        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            memmove( _buf + idx + 1, _buf + idx, ( _size - idx )*sizeof( T ) );
            ++_size;
            return *new ( _buf + idx ) T( std::forward<PTS>( ps )... );
        }
        else
        {
            for( int i = _size; i > idx; --i )
            {
                new ( _buf + i ) T( std::move( _buf[ i - 1 ] ) );
            }
            ++_size;
            return *new ( _buf + idx ) T( std::forward<PTS>( ps )... );
        }
    }
























    template<typename T>
    int List<T>::GetWriteBufferSize() const
    {
        if( std::is_pod<T>::value )
        {
            return sizeof( int ) + sizeof( T ) * _size;
        }

        int siz = sizeof( int );
        for( int i = 0; i < _size; ++i )
        {
            siz += BufferUtils::GetSize( _buf[ i ] );
        }
        return siz;
    }

    template<typename T>
    void List<T>::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( _size );
        if( std::is_pod<T>::value )
        {
            fb.Write( (char*)_buf, _size * sizeof( T ) );
            return;
        }
        for( int i = 0; i < _size; ++i )
        {
            fb.Write( _buf[ i ] );
        }
    }

    template<typename T>
    void List<T>::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( _size );
        if( std::is_pod<T>::value )
        {
            fb.WriteDirect( (char*)_buf, _size * sizeof( T ) );
            return;
        }
        for( int i = 0; i < _size; ++i )
        {
            fb.WriteDirect( _buf[ i ] );
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
            memcpy( _buf, fb.Data() + fb.Offset(), siz );
            fb.Offset() += siz;
            return true;
        }
        Clear();
        Reserve( len );
        for( int i = 0; i < len; ++i )
        {
            new ( _buf + i ) T();
            _size = i + 1;
            if( !_buf[ i ].ReadBuffer( fb ) ) return false;
        }
        return true;
    }


}

#endif
