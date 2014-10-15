#ifndef _FLATBUFFER_HPP__
#define _FLATBUFFER_HPP__

namespace xxx
{

    HAS_FUNC( writeBufferDirect_checker, WriteBufferDirect, void ( T::* )( FlatBuffer& ) const );
    template<typename T>
    typename std::enable_if<writeBufferDirect_checker<T>::value, void>::type writeBufferDirect_switch( FlatBuffer& fb, T const& v )
    {
        v.WriteBufferDirect( fb );
    };
    template<typename T>
    typename std::enable_if<!writeBufferDirect_checker<T>::value, void>::type writeBufferDirect_switch( FlatBuffer& fb, T const& v )
    {
        //static_assert( std::is_pod<T>::value, "forGet impl WriteBufferDirect func ?" );
        BufferUtils::Write( fb.Data() + fb.Size(), v );
        fb.Size() += BufferUtils::GetSize( v );
    };
    template<typename T>
    void FlatBuffer::WriteDirect( T const& v )
    {
        writeBufferDirect_switch( *this, v );
    }


    HAS_FUNC( writeBuffer_checker, WriteBuffer, void ( T::* )( FlatBuffer& ) const );
    template<typename T>
    typename std::enable_if<writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
    {
        v.WriteBuffer( fb );
    };
    template<typename T>
    typename std::enable_if<!writeBuffer_checker<T>::value, void>::type writeBuffer_switch( FlatBuffer& fb, T const& v )
    {
        //static_assert( std::is_pod<T>::value, "forGet impl WriteBuffer func ?" );
        int siz = BufferUtils::GetSize( v );
        fb.Reserve( fb.Size() + siz );
        BufferUtils::Write( fb.Data() + fb.Size(), v );
        fb.Size() += siz;
    };
    template<typename T>
    void FlatBuffer::Write( T const& v )
    {
        writeBuffer_switch( *this, v );
    }


    template<int len>
    void FlatBuffer::WriteDirect( char const( &s )[ len ] )
    {
        WriteDirect( len - 1 );
        WriteDirect( s, len - 1 );
    }
    template<int len>
    void FlatBuffer::Write( char const( &s )[ len ] )
    {
        Reserve( _dataLen + sizeof( int ) + len - 1 );
        WriteDirect( s );
    }


    template<typename T, int len>
    void FlatBuffer::WriteDirect( T const( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            WriteDirect( (char*)a, len * sizeof( T ) );
        }
        else
        {
            for( int i = 0; i < len; ++i )
            {
                writeBufferDirect_switch( *this, a[ i ] );
            }
        }
    }
    template<typename T, int len>
    void FlatBuffer::Write( T const( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            auto siz = len * ( int )sizeof( T );
            Reserve( _dataLen + siz );
            WriteDirect( (char*)a, len * sizeof( T ) );
        }
        else
        {
            for( int i = 0; i < len; ++i )
            {
                writeBuffer_switch( *this, a[ i ] );
            }
        }
    }





    template<typename T>
    void FlatBuffer::WritesCore( T const& v )
    {
        WriteDirect( v );
    }

    template<typename T, typename ...TS>
    void FlatBuffer::WritesCore( T const& v, TS const& ...vs )
    {
        WriteDirect( v );
        WritesCore( vs... );
    }

    template<typename ...TS>
    void FlatBuffer::Writes( TS const& ...vs )
    {
        Reserve( _dataLen + BufferUtils::GetSizes( vs... ) );
        WritesCore( vs... );
    }

    template<typename ...TS>
    void FlatBuffer::WritesDirect( TS const& ...vs )
    {
        WritesCore( vs... );
    }



    HAS_FUNC( ReadBuffer_checker, ReadBuffer, bool ( T::* )( FlatBuffer& ) );
    template<typename T>
    typename std::enable_if<ReadBuffer_checker<T>::value, bool>::type ReadBuffer_switch( FlatBuffer& fb, T& v )
    {
        return v.ReadBuffer( fb );
    };
    template<typename T>
    typename std::enable_if<!ReadBuffer_checker<T>::value, bool>::type ReadBuffer_switch( FlatBuffer& fb, T& v )
    {
        assert( std::is_pod<T>::value );
        auto siz = BufferUtils::GetSize( v );
        if( fb.Offset() + siz > fb.Size() ) return false;
        BufferUtils::Read( v, fb.Data() + fb.Offset() );
        fb.Offset() += siz;
        return true;
    };
    template<typename T>
    bool FlatBuffer::Read( T& v )
    {
        return ReadBuffer_switch( *this, v );
    }


    template<typename T, int len>
    bool FlatBuffer::Read( T( &a )[ len ] )
    {
        if( std::is_pod<T>::value )
        {
            int siz = len * ( int )sizeof( T );
            if( _offset + siz > _dataLen ) return false;
            memcpy( &a, _buf + _offset, siz );
            _offset += siz;
            return true;
        }
        for( int i = 0; i < len; ++i )
        {
            if( !ReadBuffer_switch( *this, a[ i ] ) ) return false;
        }
        return true;
    }



    template<typename T>
    bool FlatBuffer::ReadsCore( T& v )
    {
        return Read( v );
    }

    template<typename T, typename ...TS>
    bool FlatBuffer::ReadsCore( T& v, TS& ...vs )
    {
        if( !Read( v ) ) return false;
        return ReadsCore( vs... );
    }

    template<typename ...TS>
    bool FlatBuffer::Reads( TS& ...vs )
    {
        return ReadsCore( vs... );
    }






    template<typename T>
    void FlatBuffer::ReadDirect( T& v )
    {
        assert( std::is_pod<T>::value );
        BufferUtils::Read( v, _buf + _offset );
        _offset += BufferUtils::GetSize( v );
    }
    template<typename T>
    void FlatBuffer::ReadsDirectCore( T& v )
    {
        ReadDirect( v );
    }

    template<typename T, typename ...TS>
    void FlatBuffer::ReadsDirectCore( T& v, TS& ...vs )
    {
        ReadDirect( v );
        ReadsDirectCore( vs... );
    }

    template<typename ...TS>
    void FlatBuffer::ReadsDirect( TS& ...vs )
    {
        ReadsDirectCore( vs... );
    }



}


#endif
