#pragma once

namespace xxx
{

    template <class T>
    template<typename ...PTS>
    void FlatQueue<T>::Emplace( PTS&& ...ps )
    {
        new ( buf + tail++ ) T( std::forward<PTS>( ps )... );
        if( tail == maxSize ) tail = 0;
        if( tail == head ) Reserve( maxSize + 1, true );
    }

    template <class T>
    FlatQueue<T>::FlatQueue( int capacity )
    {
        assert( capacity > 0 );
        auto byteLen = int( capacity * sizeof( T ) );
        if( byteLen < 64 ) byteLen = 64;
        else byteLen = (int)Round2n( byteLen );
        buf = ( T* )new char[ byteLen ];
        maxSize = byteLen / sizeof( T );
        head = 0;
        tail = 0;
    }

    template <class T>
    FlatQueue<T>::~FlatQueue()
    {
        if( buf )
        {
            Clear();
            delete[]( char* )buf;
        }
    }


    template <class T>
    int FlatQueue<T>::Size() const
    {
        if( head <= tail )
            return tail - head;
        else
            return maxSize - head + tail;
    }

    template <class T>
    bool FlatQueue<T>::Empty() const
    {
        return head == tail;
    }


    template <class T>
    void FlatQueue<T>::Pop()
    {
        assert( head != tail );
        buf[ head++ ].~T();
        if( head == maxSize ) head = 0;
    }

    template <class T>
    T const& FlatQueue<T>::Top() const
    {
        assert( head != tail );
        return buf[ head ];
    }

    template <class T>
    T& FlatQueue<T>::Top()
    {
        assert( head != tail );
        return buf[ head ];
    }


    template <class T>
    void FlatQueue<T>::Reserve( int capacity, bool afterPush )
    {
        assert( capacity > 0 );
        if( capacity <= maxSize ) return;

        int size = afterPush ? maxSize : this->Size();

        auto byteLen = (int)Round2n( capacity * sizeof( T ) );
        auto newBuf = ( T* )new char[ byteLen ];

        if( std::is_pod<T>::value
            || std::is_base_of<Memmoveable, T>::value )
        {
            if( head <= tail )
            {
                memcpy( newBuf, buf + tail, ( maxSize - tail ) * sizeof( T ) );
                memcpy( newBuf + maxSize - tail, buf, head * sizeof( T ) );
            }
            else
            {
                memcpy( newBuf, buf + tail, size * sizeof( T ) );
            }
        }
        else
        {
            if( head <= tail )
            {
                for( int i = 0; i < maxSize - tail; ++i )
                    new ( newBuf + i ) T( std::move( buf[ tail + i ] ) );
                auto p = newBuf + maxSize - tail;
                for( int i = 0; i < head; ++i )
                    new ( p + i ) T( std::move( buf[ i ] ) );
            }
            else
            {
                for( int i = 0; i < size; ++i )
                    new ( newBuf + i ) T( std::move( buf[ tail + i ] ) );
            }
        }
        delete[]( char* )buf;
        buf = newBuf;
        maxSize = byteLen / sizeof( T );
        head = 0;
        tail = size;
    }

    template <typename T>
    template <typename VT>
    void FlatQueue<T>::Push( VT&& v )
    {
        new ( buf + tail++ ) T( std::forward<VT>( v ) );
        if( tail == maxSize ) tail = 0;
        if( tail == head ) Reserve( maxSize + 1, true );
    }


    template <class T>
    void FlatQueue<T>::Clear()
    {
        if( !std::is_pod<T>::value )
        {
            if( head < tail )
            {
                for( int i = 0; i < head; ++i ) buf[ i ].~T();
                for( int i = 0; i < maxSize - tail; ++i ) buf[ tail + i ].~T();
            }
            else
            {
                for( int i = tail; i < head; ++i ) buf[ i ].~T();
            }
        }
        head = 0;
        tail = 0;
    }


    template<typename T>
    T const & FlatQueue<T>::operator[]( int idx ) const
    {
        return IndexAt( idx );
    }

    template<typename T>
    T& FlatQueue<T>::operator[]( int idx )
    {
        return IndexAt( idx );
    }

    template<typename T>
    T const & FlatQueue<T>::IndexAt( int idx ) const
    {
        if( head + idx >= maxSize )
            return buf[ head + idx - maxSize ];
        else
            return buf[ head + idx ];
    }

    template<typename T>
    T& FlatQueue<T>::IndexAt( int idx )
    {
        if( head + idx >= maxSize )
            return buf[ head + idx - maxSize ];
        else
            return buf[ head + idx ];
    }


    template <class T>
    bool FlatQueue<T>::Pop( T& outVal )
    {
        if( head == tail ) return false;
        outVal = buf[ head ];
        Pop();
        return true;
    }

}
