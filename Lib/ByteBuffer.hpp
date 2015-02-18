#ifndef _BYTEBUFFER_HPP__
#define _BYTEBUFFER_HPP__

namespace xxx
{

    template<typename T>
    void ByteBuffer::WriteCore( char* dest, T const& src )
    {
#ifdef __IA
        *(T*)dest = src;
#else
        auto p = (char*)&src;
        if( sizeof( T ) >= 1 )
        {
            dest[ 0 ] = p[ 0 ];
        }
        if( sizeof( T ) >= 2 )
        {
            dest[ 1 ] = p[ 1 ];
        }
        if( sizeof( T ) >= 4 )
        {
            dest[ 2 ] = p[ 2 ];
            dest[ 3 ] = p[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            dest[ 4 ] = p[ 4 ];
            dest[ 5 ] = p[ 5 ];
            dest[ 6 ] = p[ 6 ];
            dest[ 7 ] = p[ 7 ];
        }
#endif
    }



    template<typename T>
    void ByteBuffer::ReadCore( T& dest, char const* src )
    {
#ifdef __IA
        dest = *(T*)src;
#else
        auto p = (char*)&dest;
        if( sizeof( T ) >= 1 )
        {
            p[ 0 ] = src[ 0 ];
        }
        if( sizeof( T ) >= 2 )
        {
            p[ 1 ] = src[ 1 ];
        }
        if( sizeof( T ) >= 4 )
        {
            p[ 2 ] = src[ 2 ];
            p[ 3 ] = src[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            p[ 4 ] = src[ 4 ];
            p[ 5 ] = src[ 5 ];
            p[ 6 ] = src[ 6 ];
            p[ 7 ] = src[ 7 ];
        }
#endif
    }



    HAS_FUNC( HasFunc_FastWriteTo, FastWriteTo, void ( T::* )( ByteBuffer& ) const );
    template<typename T>
    typename std::enable_if<HasFunc_FastWriteTo<T>::value, void>::type FastWriteSwitch( ByteBuffer& b, T const& v )
    {
        auto rtv = ptrStore->Insert( &v, (uint)dataLen );
        VarWrite( rtv.first->value );
        if( !rtv.second ) return;
        v.FastWriteTo( b );
    };
    template<typename T>
    typename std::enable_if<!HasFunc_FastWriteTo<T>::value, void>::type FastWriteSwitch( ByteBuffer& b, T const& v )
    {
        static_assert( std::is_pod<T>::value, "forget impl FastWriteTo func ?" );
        ByteBuffer::WriteCore( b.buf + b.dataLen, v );
        b.dataLen += sizeof( v );
    };
    template<typename T>
    void ByteBuffer::FastWrite( T const& v )
    {
        FastWriteSwitch( *this, v );
    }

    template<int len>
    void ByteBuffer::FastWrite( char const( &s )[ len ] )
    {
        FastWrite( len - 1 );
        FastWrite( s, len - 1 );
    }

    template<typename T, int len>
    void ByteBuffer::FastWrite( T const( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            FastWrite( (char*)a, len * sizeof( T ) );
        }
        else
        {
            for( int i = 0; i < len; ++i )
            {
                FastWriteSwitch( *this, a[ i ] );
            }
        }
    }

    template<typename T>
    void ByteBuffer::FastWriteMultiCore( T const& v )
    {
        FastWrite( v );
    }
    template<typename T, typename ...TS>
    void ByteBuffer::FastWriteMultiCore( T const& v, TS const& ...vs )
    {
        FastWrite( v );
        WriteMultiCore( vs... );
    }
    template<typename ...TS>
    void ByteBuffer::FastWriteMulti( TS const& ...vs )
    {
        FastWriteMultiCore( vs... );
    }






    template<typename T>
    void ByteBuffer::FastRead( T& v )
    {
        assert( std::is_pod<T>::value );
        ByteBuffer::ReadCore( v, buf + offset );
        offset += sizeof( v );
    }

    template<typename T>
    void ByteBuffer::FastReadMultiCore( T& v )
    {
        FastRead( v );
    }
    template<typename T, typename ...TS>
    void ByteBuffer::FastReadMultiCore( T& v, TS& ...vs )
    {
        FastRead( v );
        FastReadMultiCore( vs... );
    }

    template<typename ...TS>
    void ByteBuffer::FastReadMulti( TS& ...vs )
    {
        FastReadMultiCore( vs... );
    }







    HAS_FUNC( HasFunc_WriteTo, WriteTo, void ( T::* )( ByteBuffer& ) const );
    template<typename T>
    typename std::enable_if<HasFunc_WriteTo<T>::value, void>::type WriteSwitch( ByteBuffer& b, T const& v )
    {
        if( b.ptrStore )
        {
            auto rtv = b.ptrStore->Insert( (void*)&v, (uint)b.dataLen );
            b.VarWrite( rtv.first->value );
            if( !rtv.second ) return;
        }
        v.WriteTo( b );
    };
    template<typename T>
    typename std::enable_if<!HasFunc_WriteTo<T>::value, void>::type WriteSwitch( ByteBuffer& b, T const& v )
    {
        static_assert( std::is_pod<T>::value, "forget impl WriteTo func ?" );
        b.Reserve( b.dataLen + sizeof( T ) );
        ByteBuffer::WriteCore( b.buf + b.dataLen, v );
        b.dataLen += sizeof( T );
    };
    template<typename T>
    void ByteBuffer::Write( T const& v )
    {
        WriteSwitch( *this, v );
    }


    template<int len>
    void ByteBuffer::Write( char const( &s )[ len ] )
    {
        Reserve( dataLen + sizeof( int ) + len - 1 );
        FastWrite( s );
    }

    template<typename T, int len>
    void ByteBuffer::Write( T const( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            auto siz = len * ( int )sizeof( T );
            Reserve( dataLen + siz );
            FastWrite( (char*)a, len * sizeof( T ) );
        }
        else
        {
            for( int i = 0; i < len; ++i )
            {
                Write( a[ i ] );
            }
        }
    }

    template<typename T>
    void ByteBuffer::WriteMultiCore( T const& v )
    {
        Write( v );
    }
    template<typename T, typename ...TS>
    void ByteBuffer::WriteMultiCore( T const& v, TS const& ...vs )
    {
        Write( v );
        WriteMultiCore( vs... );
    }
    template<typename ...TS>
    void ByteBuffer::WriteMulti( TS const& ...vs )
    {
        WriteMultiCore( vs... );
    }

    template<typename T>
    void ByteBuffer::Write( T* v )
    {
        if( v )
        {
            Write( true );
            Write( *v );
        }
        else
        {
            Write( false );
        }
    }
    template<typename T>
    void xxx::ByteBuffer::RootWrite( T const& v )
    {
        PtrStoreInit();
        Write( v );
    }


    template<typename T>
    bool xxx::ByteBuffer::Read( T*& v )
    {
        bool b;
        if( !Read( b ) ) return false;
        if( b )
        {
            uint _offset, _offset_bak = (uint)offset;
            if( !VarRead( _offset ) ) return false;
            if( _offset == _offset_bak )
            {
                assert( !v );                       // todo: obj pool support ?
                v = new T();
                if( !Read( *v ) )
                {
                    delete v;
                    v = nullptr;
                    return false;
                }
                idxStore->Insert( _offset, v );     // todo: check
            }
            else
            {
                v = (T*)idxStore->At( _offset );
            }
        }
        else
        {
            v = nullptr;
        }
        return true;
    }
    template<typename T>
    bool xxx::ByteBuffer::RootRead( T& v )
    {
        IdxStoreInit();
        return Read( v );
    }




    HAS_FUNC( HasFunc_ReadFrom, ReadFrom, bool ( T::* )( ByteBuffer& ) );
    template<typename T>
    typename std::enable_if<HasFunc_ReadFrom<T>::value, bool>::type ReadSwitch( ByteBuffer& b, T& v )
    {
        if( b.idxStore )
        {
            uint _offset, _offset_bak = b.offset;
            if( !b.VarRead( _offset ) ) return false;
            b.idxStore->Insert( _offset, (void*)&v );   // todo: assert?
        }
        return v.ReadFrom( b );
    };
    template<typename T>
    typename std::enable_if<!HasFunc_ReadFrom<T>::value, bool>::type ReadSwitch( ByteBuffer& b, T& v )
    {
        assert( std::is_pod<T>::value );
        if( b.offset + (int)sizeof( T ) > b.dataLen ) return false;
        ByteBuffer::ReadCore( v, b.buf + b.offset );
        b.offset += sizeof( T );
        return true;
    };
    template<typename T>
    bool ByteBuffer::Read( T& v )
    {
        return ReadSwitch( *this, v );
    }

    template<typename T, int len>
    bool ByteBuffer::Read( T( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            int siz = len * ( int )sizeof( T );
            if( offset + siz > dataLen ) return false;
            memcpy( &a, buf + offset, siz );
            offset += siz;
            return true;
        }
        for( int i = 0; i < len; ++i )
        {
            if( !Read( a[ i ] ) ) return false;
        }
        return true;
    }



    template<typename T>
    bool ByteBuffer::ReadMultiCore( T& v )
    {
        return Read( v );
    }

    template<typename T, typename ...TS>
    bool ByteBuffer::ReadMultiCore( T& v, TS& ...vs )
    {
        if( !Read( v ) ) return false;
        return ReadMultiCore( vs... );
    }

    template<typename ...TS>
    bool ByteBuffer::ReadMulti( TS& ...vs )
    {
        return ReadMultiCore( vs... );
    }


}


#endif
