#ifndef _FLATQUEUE_HPP__
#define _FLATQUEUE_HPP__

namespace xxx
{

    template <class T>
    template<typename ...PTS>
    void FlatQueue<T>::Emplace( PTS&& ...ps )
    {
        new ( _buf + _tail++ ) T( std::forward<PTS>( ps )... );
        if( _tail == _maxSize ) _tail = 0;
        if( _tail == _head ) Reserve( _maxSize + 1, true );
    }

    template <class T>
    FlatQueue<T>::FlatQueue( int capacity )
    {
        assert( capacity > 0 );
        auto byteLen = int( capacity * sizeof( T ) );
        if( byteLen < 64 ) byteLen = 64;
        else byteLen = (int)Round2n( byteLen );
        _buf = ( T* )new char[ byteLen ];
        _maxSize = byteLen / sizeof( T );
        _head = 0;
        _tail = 0;
    }

    template <class T>
    FlatQueue<T>::~FlatQueue()
    {
        if( _buf )
        {
            Clear();
            delete[]( char* )_buf;
        }
    }


    template <class T>
    int FlatQueue<T>::Size() const
    {
        if( _head <= _tail )
            return _tail - _head;
        else
            return _maxSize - _head + _tail;
    }

    template <class T>
    bool FlatQueue<T>::Empty() const
    {
        return _head == _tail;
    }


    template <class T>
    void FlatQueue<T>::Pop()
    {
        assert( _head != _tail );
        _buf[ _head++ ].~T();
        if( _head == _maxSize ) _head = 0;
    }

    template <class T>
    T const& FlatQueue<T>::Top() const
    {
        assert( _head != _tail );
        return _buf[ _head ];
    }

    template <class T>
    T& FlatQueue<T>::Top()
    {
        assert( _head != _tail );
        return _buf[ _head ];
    }


    template <class T>
    void FlatQueue<T>::Reserve( int capacity, bool afterPush )
    {
        assert( capacity > 0 );
        if( capacity <= _maxSize ) return;

        int size = afterPush ? _maxSize : this->Size();

        auto byteLen = (int)Round2n( capacity * sizeof( T ) );
        auto newBuf = ( T* )new char[ byteLen ];

        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            if( _head <= _tail )
            {
                memcpy( newBuf, _buf + _tail, ( _maxSize - _tail ) * sizeof( T ) );
                memcpy( newBuf + _maxSize - _tail, _buf, _head * sizeof( T ) );
            }
            else
            {
                memcpy( newBuf, _buf + _tail, size * sizeof( T ) );
            }
        }
        else
        {
            if( _head <= _tail )
            {
                for( int i = 0; i < _maxSize - _tail; ++i )
                    new ( newBuf + i ) T( std::move( _buf[ _tail + i ] ) );
                auto p = newBuf + _maxSize - _tail;
                for( int i = 0; i < _head; ++i )
                    new ( p + i ) T( std::move( _buf[ i ] ) );
            }
            else
            {
                for( int i = 0; i < size; ++i )
                    new ( newBuf + i ) T( std::move( _buf[ _tail + i ] ) );
            }
        }
        delete[]( char* )_buf;
        _buf = newBuf;
        _maxSize = byteLen / sizeof( T );
        _head = 0;
        _tail = size;
    }

    template <class T>
    void FlatQueue<T>::Push( T const& v )
    {
        new ( _buf + _tail++ ) T( v );
        if( _tail == _maxSize ) _tail = 0;
        if( _tail == _head ) Reserve( _maxSize + 1, true );
    }


    template <class T>
    void FlatQueue<T>::Clear()
    {
        if( !std::is_pod<T>::value )
        {
            if( _head <= _tail )
            {
                for( int i = 0; i < _head; ++i ) _buf[ i ].~T();
                for( int i = 0; i < _maxSize - _tail; ++i ) _buf[ _tail + i ].~T();
            }
            else
            {
                for( int i = _tail; i < _head; ++i ) _buf[ i ].~T();
            }
        }
        _head = 0;
        _tail = 0;
    }


    template<typename T>
    T const & FlatQueue<T>::operator[]( int idx ) const
    {
        return At( idx );
    }

    template<typename T>
    T& FlatQueue<T>::operator[]( int idx )
    {
        return At( idx );
    }

    template<typename T>
    T const & FlatQueue<T>::At( int idx ) const
    {
        if( _head + idx >= _maxSize )
            return _buf[ _head + idx - _maxSize ];
        else
            return _buf[ _head + idx ];
    }

    template<typename T>
    T& FlatQueue<T>::At( int idx )
    {
        if( _head + idx >= _maxSize )
            return _buf[ _head + idx - _maxSize ];
        else
            return _buf[ _head + idx ];
    }


    template <class T>
    bool FlatQueue<T>::Pop( T& outVal )
    {
        if( _head == _tail ) return false;
        outVal = _buf[ _head ];
        Pop();
        return true;
    }

}

#endif
