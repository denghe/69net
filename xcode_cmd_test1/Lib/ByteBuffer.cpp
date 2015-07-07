#include "All.h"

namespace xxx
{
    ByteBuffer::ByteBuffer( int capacity )
    {
        if( capacity < 1024 )
        {
            bufLen = 1024;
        }
        else
        {
            bufLen = (int)Round2n( capacity );
        }
        isRef = false;
        buf = new char[ bufLen ];
        dataLen = offset = 0;
    }

    ByteBuffer::ByteBuffer( char* _buf, int _bufLen, int _dataLen /*= 0*/, bool _isRef /*= false */ )
    {
        dataLen = _dataLen;
        offset = 0;
        if( _isRef )
        {
            buf = _buf;
            bufLen = _bufLen;
            isRef = true;
        }
        else
        {
            bufLen = (int)Round2n( _dataLen );
            buf = new char[ bufLen ];
            memcpy( buf, _buf, _dataLen );
            isRef = false;
        }
    }

    ByteBuffer::ByteBuffer( ByteBuffer const& other )
        : ByteBuffer( other.dataLen + 1 )
    {
        memcpy( buf, other.buf, other.dataLen + 1 );
        dataLen = other.dataLen;
    }

    ByteBuffer::ByteBuffer( ByteBuffer&& other )
        : buf( other.buf )
        , bufLen( other.bufLen )
        , dataLen( other.dataLen )
        , offset( other.offset )
        , isRef( other.isRef )
    {
        //other.buf = nullptr;
        //other.bufLen = 0;
        //other.dataLen = 0;
        //other.offset = 0;
        other.isRef = true;     // 防回收
    }

    void ByteBuffer::Destroy()
    {
        if( isRef ) return;
        delete[] buf;
#if __DEBUG
        buf = nullptr;
#endif
    }

    ByteBuffer::~ByteBuffer()
    {
        Destroy();
#if ENABLE_BYTEBUFFER_PTR_SUPPORT
        PtrStoreDestroy();
        IdxStoreDestroy();
#endif
    }

    void ByteBuffer::Clear()
    {
        dataLen = offset = 0;
    }

    bool ByteBuffer::Empty() const
    {
        return dataLen == 0;
    }

    bool ByteBuffer::Full() const
    {
        return dataLen == offset;
    }


    void ByteBuffer::Assign( char const* _buf, int _bufLen, int _dataLen /*= 0*/, bool _isRef /*= false */ )
    {
        assert( buf != _buf );
        offset = 0;
        if( _isRef )
        {
            Destroy();
            buf = (char*)_buf;
            bufLen = _bufLen;
            dataLen = _dataLen;
            isRef = _isRef;
            return;
        }
        if( bufLen < _dataLen )
        {
            Destroy();
            bufLen = (int)Round2n( _dataLen );
            _isRef = false;
            buf = new char[ bufLen ];
        }
        dataLen = _dataLen;
        memcpy( buf, _buf, _dataLen );
    }

    void ByteBuffer::Reserve( int capacity )
    {
        if( capacity <= bufLen ) return;
        bufLen = (int)Round2n( capacity );
        auto newBuf = new char[ bufLen ];
        memcpy( newBuf, buf, dataLen );
        Destroy();
        isRef = false;
        buf = newBuf;
    }

    void ByteBuffer::Resize( int _dataLen )
    {
        if( _dataLen == dataLen ) return;
        else if( _dataLen < dataLen )
        {
            dataLen = _dataLen;
            if( dataLen > offset ) offset = dataLen;
        }
        else
        {
            Reserve( _dataLen );
            dataLen = _dataLen;
        }
    }

    ByteBuffer& ByteBuffer::operator=( ByteBuffer const& other )
    {
        if( this == &other ) return *this;
        offset = other.offset;
        dataLen = other.dataLen;
        if( bufLen >= other.dataLen )
        {
            memcpy( buf, other.buf, other.dataLen );
        }
        else
        {
            Destroy();
            bufLen = (int)Round2n( other.dataLen );
            isRef = false;
            buf = new char[ bufLen ];
            memcpy( buf, other.buf, other.dataLen );
        }
        return *this;
    }

    ByteBuffer& ByteBuffer::operator=( ByteBuffer&& other )
    {
        Destroy();
        buf = other.buf;
        bufLen = other.bufLen;
        dataLen = other.dataLen;
        offset = other.offset;
        isRef = other.isRef;
        //other.buf = nullptr;
        //other.bufLen = 0;
        //other.dataLen = 0;
        //other.offset = 0;
        other.isRef = true;
        return *this;
    }

    char ByteBuffer::operator[]( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char& ByteBuffer::operator[]( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char ByteBuffer::At( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char& ByteBuffer::At( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    String ByteBuffer::Dump()
    {
        String rtv;
        BufferUtils::Dump( rtv, buf, dataLen );
        return rtv;
    }




    void ByteBuffer::FastWrite( char const* _buf, int _dataLen )
    {
        memcpy( buf + dataLen, _buf, _dataLen );
        dataLen += _dataLen;
    }

    void ByteBuffer::Write( char const* _buf, int _dataLen )
    {
        Reserve( dataLen + _dataLen );
        FastWrite( _buf, _dataLen );
    }

    bool ByteBuffer::Read( char* _buf, int _dataLen )
    {
        if( offset + _dataLen > dataLen ) return false;
        memcpy( _buf, buf + offset, _dataLen );
        offset += _dataLen;
        return true;
    }








    void ByteBuffer::WriteTo( ByteBuffer& bb ) const
    {
        bb.VarWrite( (uint)dataLen );
        if( !dataLen ) return;
        bb.Write( buf, dataLen );
    }

    void ByteBuffer::FastWriteTo( ByteBuffer& bb ) const
    {
        bb.FastVarWrite( (uint)dataLen );
        if( !dataLen ) return;
        bb.FastWrite( buf, dataLen );
    }

    bool ByteBuffer::ReadFrom( ByteBuffer& bb )
    {
        int len;
        if( !bb.VarRead( *(uint*)&len )
            || len < 0
            || bb.offset + len > bb.dataLen ) return false;
        Clear();
        Reserve( len );
        dataLen = len;
        memcpy( buf, bb.buf + bb.offset, len );
        bb.offset += len;
        return true;
    }








#if ENABLE_BYTEBUFFER_PTR_SUPPORT
    void ByteBuffer::PtrStoreInit()
    {
        if( ptrStore ) ptrStore->Clear();
        else ptrStore = new Dict<void*, uint>();
    }
    void ByteBuffer::PtrStoreDestroy()
    {
        if( ptrStore ) delete ptrStore;
    }

    void ByteBuffer::IdxStoreInit()
    {
        if( idxStore ) idxStore->Clear();
        else idxStore = new Dict<uint, void*>();
    }
    void ByteBuffer::IdxStoreDestroy()
    {
        if( idxStore ) delete idxStore;
    }

#endif

    void ByteBuffer::Write7Core( char* buf, int& offset, uint32 v )
    {
Lab1:
        auto b7 = uint8( v );
        if( ( v >>= 7 ) )
        {
            buf[ offset++ ] = b7 | 0x80u;
            goto Lab1;
        }
        buf[ offset++ ] = b7;
    }

    void ByteBuffer::Write7Core( char* buf, int& offset, uint64 v )
    {
        auto idx8 = offset + 8;
Lab1:
        auto b7 = uint8( v );
        if( ( v >>= 7 ) )
        {
            buf[ offset++ ] = b7 | 0x80u;
            if( offset == idx8 )
                buf[ offset++ ] = (uint8)v;
            else
                goto Lab1;
        }
        else
        {
            buf[ offset++ ] = b7;
        }
    }

    void ByteBuffer::FastRead7Core( uint64& v, char* buf, int& offset )
    {
        auto p = buf + offset;
        uint64 i = 0, b7;
        uint lshift = 0, idx = 0;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx < 8 )
            {
                i |= ( b7 & 0x7F ) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else i |= ( b7 << lshift ) | ( (uint64)p[ idx++ ] << 28 << 28 );
        }
        else i |= b7 << lshift;
        v = i;
        offset += idx;
    }

    bool ByteBuffer::Read7Core( uint64& v, char* buf, int dataLen, int& offset )
    {
        if( offset >= dataLen ) return false;// ReadStatus::NotEnoughData;
        auto p = buf + offset;
        uint64 i = 0, b7;
        int lshift = 0, idx = 0;
        dataLen -= offset;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx >= dataLen ) return false;// ReadStatus::NotEnoughData;
            if( idx < 8 )
            {
                i |= ( b7 & 0x7F ) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else i |= ( b7 << lshift ) | ( (uint64)p[ idx++ ] << 28 << 28 );
        }
        else i |= b7 << lshift;
        v = i;
        offset += idx;
        return true;// ReadStatus::Success;
    }

    bool ByteBuffer::Read7Core( uint32& v, char * buf, int len, int & offset )
    {
        if( offset >= len ) return false;// ReadStatus::NotEnoughData;
        auto p = buf + offset;
        uint i = 0, b7;
        int lshift = 0, idx = 0;
        len -= offset;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx == 5 ) return false;// ReadStatus::Overflow;
            if( idx >= len ) return false;// ReadStatus::NotEnoughData;
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else
        {
            if( idx == 5 && b7 > 15 ) return false;// ReadStatus::Overflow;
            else i |= b7 << lshift;
        }
        v = i;
        offset += idx;
        return true;// ReadStatus::Success;
    }

    void ByteBuffer::FastRead7Core( uint32& v, char* buf, int& offset )
    {
        auto p = buf + offset;
        uint i = 0, b7, lshift = 0, idx = 0;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else i |= b7 << lshift;
        v = i;
        offset += idx;
    }





    void ByteBuffer::VarWrite( int v )
    {
        Reserve( dataLen + sizeof( decltype(v) ) );
        ByteBuffer::Write7Core( buf, dataLen, ByteBuffer::ZegZagEncode( v ) );
    }

    void ByteBuffer::VarWrite( uint v )
    {
        Reserve( dataLen + sizeof( decltype( v ) ) );
        ByteBuffer::Write7Core( buf, dataLen, v );
    }

    void ByteBuffer::VarWrite( int64 v )
    {
        Reserve( dataLen + sizeof( decltype( v ) ) );
        ByteBuffer::Write7Core( buf, dataLen, ByteBuffer::ZegZagEncode( v ) );
    }

    void ByteBuffer::VarWrite( uint64 v )
    {
        Reserve( dataLen + sizeof( decltype( v ) ) );
        ByteBuffer::Write7Core( buf, dataLen, v );
    }




    bool ByteBuffer::VarRead( int& v )
    {
        uint i;
        if( !VarRead( i ) ) return false;
        v = ByteBuffer::ZegZagDecode( i );
        return true;
    }

    bool ByteBuffer::VarRead( uint& v )
    {
        return ByteBuffer::Read7Core( v, buf, dataLen, offset );
    }

    bool ByteBuffer::VarRead( int64& v )
    {
        uint64 i;
        if( !VarRead( i ) ) return false;
        v = ByteBuffer::ZegZagDecode( i );
        return true;
    }

    bool ByteBuffer::VarRead( uint64& v )
    {
        return ByteBuffer::Read7Core( v, buf, dataLen, offset );
    }




    void ByteBuffer::FastVarWrite( int v )
    {
        ByteBuffer::Write7Core( buf, dataLen, ByteBuffer::ZegZagEncode( v ) );
    }

    void ByteBuffer::FastVarWrite( uint v )
    {
        ByteBuffer::Write7Core( buf, dataLen, v );
    }

    void ByteBuffer::FastVarWrite( int64 v )
    {
        ByteBuffer::Write7Core( buf, dataLen, ByteBuffer::ZegZagEncode( v ) );
    }

    void ByteBuffer::FastVarWrite( uint64 v )
    {
        ByteBuffer::Write7Core( buf, dataLen, v );
    }




    void ByteBuffer::FastVarRead( int& v )
    {
        uint i;
        FastVarRead( i );
        v = ByteBuffer::ZegZagDecode( i );
    }

    void ByteBuffer::FastVarRead( uint& v )
    {
        ByteBuffer::FastRead7Core( v, buf, offset );
    }

    void ByteBuffer::FastVarRead( int64& v )
    {
        uint64 i;
        FastVarRead( i );
        v = ByteBuffer::ZegZagDecode( i );
    }

    void ByteBuffer::FastVarRead( uint64& v )
    {
        ByteBuffer::FastRead7Core( v, buf, offset );
    }

}
