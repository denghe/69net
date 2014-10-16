#include "All.h"

namespace xxx
{

    Pool String::emptyPool;

    String::String( int capacity /*= 64 */ )
    {
        if( capacity < 64 ) bufLen = 64;
        else bufLen = (int)Round2n( capacity );
        disposer = nullptr;
        dataLen = 0;
        buf = new char[ bufLen ];
        buf[ 0 ] = '\0';
    }

    String::String( Pool& p )
    {
        buf = (char*)p.Alloc();
        bufLen = p.ItemBufLen();
        dataLen = 0;
        disposer = &p;
        buf[ 0 ] = '\0';
    }

    String::String( Pool& p, char const* _buf, int _dataLen )
    {
        assert( p.ItemBufLen() - ( int )sizeof( Pool* ) >= _dataLen + 1 );
        buf = (char*)p.Alloc();
        bufLen = p.ItemBufLen();
        dataLen = _dataLen;
        disposer = &p;
        memcpy( buf, _buf, _dataLen );
        buf[ _dataLen ] = '\0';
    }

    String::String( char const* _buf, int _bufLen, int _dataLen, bool isRef )
    {
        dataLen = _dataLen;
        if( isRef )
        {
            buf = (char*)_buf;
            bufLen = _bufLen;
            disposer = &emptyPool;
        }
        else
        {
            bufLen = (int)Round2n( _dataLen + 1 );
            buf = new char[ bufLen ];
            disposer = nullptr;
            memcpy( buf, _buf, _dataLen );
            buf[ dataLen ] = '\0';
        }
    }

    String::String( char const* s, bool isRef )
    {
        dataLen = (int)strlen( s );
        if( isRef )
        {
            buf = (char*)s;
            bufLen = dataLen + 1;
            disposer = &emptyPool;
        }
        else
        {
            bufLen = (int)Round2n( dataLen + 1 );
            buf = new char[ bufLen ];
            disposer = nullptr;
            memcpy( buf, s, dataLen + 1 );
        }
    }

    String::String( String const & o )
        : String( o.dataLen + 1 )
    {
        memcpy( buf, o.buf, o.dataLen + 1 );
        dataLen = o.dataLen;
    }

    String::String( String&& o )
        : buf( o.buf )
        , bufLen( o.bufLen )
        , dataLen( o.dataLen )
        , disposer( o.disposer )
    {
        o.buf = nullptr;
        o.bufLen = 0;
        o.dataLen = 0;
        o.disposer = nullptr;
    }


    String& String::operator=( String const& o )
    {
        if( this == &o ) return *this;
        dataLen = o.dataLen;
        if( bufLen > o.dataLen )
        {
            memcpy( buf, o.buf, o.dataLen + 1 );
        }
        else
        {
            Dispose();
            bufLen = (int)Round2n( o.dataLen + 1 );
            disposer = nullptr;
            buf = new char[ bufLen ];
            memcpy( buf, o.buf, o.dataLen + 1 );
        }
        return *this;
    }

    String& String::operator=( String && o )
    {
        Dispose();
        buf = o.buf;
        bufLen = o.bufLen;
        dataLen = o.dataLen;
        disposer = o.disposer;
        o.buf = nullptr;
        o.bufLen = 0;
        o.dataLen = 0;
        o.disposer = nullptr;
        return *this;
    }


    String::~String()
    {
        Dispose();
    }

    void String::Assign( char const* _buf, int _bufLen, int _dataLen /*= 0*/, bool isRef /*= true */ )
    {
        assert( buf != _buf );
        if( isRef )
        {
            Dispose();
            buf = (char*)_buf;
            bufLen = _bufLen;
            dataLen = _dataLen;
            disposer = &emptyPool;
            return;
        }
        if( bufLen <= _dataLen )
        {
            Dispose();
            bufLen = (int)Round2n( _dataLen + 1 );
            disposer = nullptr;
            buf = new char[ bufLen ];
        }
        dataLen = _dataLen;
        memcpy( buf, _buf, _dataLen );
        buf[ dataLen ] = '\0';
    }

    void String::Assign( char const* s, bool isRef /*= false */ )
    {
        auto _dataLen = (int)strlen( s );
        Assign( s, _dataLen + 1, _dataLen, isRef );
    }

    void String::Reserve( int capacity )
    {
        if( capacity + 1 <= bufLen ) return;
        bufLen = (int)Round2n( capacity + 1 );
        auto newBuf = new char[ bufLen ];
        memcpy( newBuf, buf, dataLen + 1 );
        Dispose();
        disposer = nullptr;
        buf = newBuf;
    }

    void String::Resize( int len, bool fillZero /*= true */ )
    {
        if( len == dataLen ) return;
        else if( len < dataLen )
        {
            dataLen = len;
            buf[ len ] = '\0';
        }
        else
        {
            Reserve( len );
            if( fillZero )
                memset( buf + dataLen, 0, len - dataLen + 1 );
            dataLen = len;
        }
    }

    void String::Clear()
    {
        dataLen = 0;
        buf[ 0 ] = '\0';
    }

    char const* String::C_str() const
    {
        return buf;
    }
    char* String::Data() const
    {
        return buf;
    }

    char String::operator[]( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }
    char& String::operator[]( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char String::At( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char& String::At( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    void String::Push( char c )
    {
        if( dataLen == bufLen ) Reserve( dataLen + 1 );
        buf[ dataLen ] = c;
        buf[ dataLen + 1 ] = '\0';
        ++dataLen;
    }

    int String::Size() const
    {
        return dataLen;
    }

    void String::Dispose()
    {
        if( disposer )
        {
            if( disposer->ItemBufLen() ) disposer->Free( buf );
        }
        else
        {
            delete[] buf;
        }
    }




    static byte const lowerchars[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
        107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95,
        96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
        117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
        137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
        157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
        177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
        197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216,
        217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
        237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
    };
    static byte const upperchars[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
        77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69,
        70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125,
        126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
        146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
        166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
        186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
        206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
        226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
        246, 247, 248, 249, 250, 251, 252, 253, 254, 255
    };

    void String::ToLower()
    {
        for( int i = 0; i < dataLen; ++i ) buf[ i ] = lowerchars[ buf[ i ] ];
    }

    void String::ToUpper()
    {
        for( int i = 0; i < dataLen; ++i ) buf[ i ] = upperchars[ buf[ i ] ];
    }

    void String::ToLowerUnsafe()
    {
        int i = 0;
        int mod = (size_t)buf & ( sizeof( size_t ) - 1 );
        if( mod )
        {
            if( mod > dataLen ) mod = dataLen;
            for( ; i < mod; ++i ) buf[ i ] |= 0x20;
        }
        mod = ( dataLen - mod ) % sizeof( size_t );
        for( ; i < dataLen - mod; i += sizeof( size_t ) )
        {
            *(size_t*)( buf + i ) |= size_t( 0x2020202020202020 );
        }
        for( ; i < dataLen; ++i ) buf[ i ] |= 0x20;
    }

    bool String::operator==( String const& o ) const
    {
        if( this == &o ) return true;
        if( dataLen != o.dataLen ) return false;
        return memcmp( buf, o.buf, dataLen ) == 0;
    }

    bool String::operator!=( String const& o ) const
    {
        return !operator==( o );
    }

    bool String::operator<( String const& o ) const
    {
        if( this == &o ) return false;
        auto r = memcmp( buf, o.buf, MIN( dataLen, o.dataLen ) );
        if( r == 0 ) return dataLen < o.dataLen;
        return r < 0;
    }

    bool String::operator>( String const& o ) const
    {
        if( this == &o ) return false;
        auto r = memcmp( buf, o.buf, MIN( dataLen, o.dataLen ) );
        if( r == 0 ) return dataLen > o.dataLen;
        return r > 0;
    }

    bool String::operator<=( String const& o ) const
    {
        return !operator>( o );
    }

    bool String::operator>=( String const& o ) const
    {
        return !operator<( o );
    }

    int String::GetHashCode() const
    {
#ifdef __IA
        return GetHash_CS( (byte const*)buf, dataLen );
#else
        if( dataLen >= 4 && ( (size_t)buf % sizeof( size_t ) == 0 ) )
            return GetHash_CS( (byte const*)buf, dataLen );
        else
            return GetHash_Lua( (byte const*)buf, dataLen );
#endif
    }

    std::string String::Std_str()
    {
        return std::string( buf, dataLen );
    }




    int String::GetWriteBufferSize() const
    {
        return sizeof( int ) + Size();
    }

    void String::WriteBuffer( FlatBuffer& fb ) const
    {
        fb.Write( dataLen );
        fb.Write( buf, dataLen );
    }
    void String::WriteBufferDirect( FlatBuffer& fb ) const
    {
        fb.WriteDirect( dataLen );
        fb.WriteDirect( buf, dataLen );
    }

    bool String::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0
            || fb.Offset() + len > fb.Size() ) return false;
        Assign( fb.Data() + fb.Offset(), 0, len, false );
        fb.Offset() += len;
        return true;
    }

    void String::Pop()
    {
        assert( dataLen > 0 );
        --dataLen;
    }

    char& String::Top()
    {
        assert( dataLen > 0 );
        return buf[ dataLen - 1 ];
    }

    char const& String::Top() const
    {
        assert( dataLen > 0 );
        return buf[ dataLen - 1 ];
    }

}
