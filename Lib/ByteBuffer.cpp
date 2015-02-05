#include "All.h"

namespace xxx
{
    ByteBuffer::ByteBuffer( int capacity )
    {
        if( capacity < 1024 )
        {
            bufLen = 1024;
        }
        else bufLen = (int)Round2n( capacity );
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
        other.isRef = true;     // ·À»ØÊÕ
    }

    void ByteBuffer::Dispose()
    {
        if( isRef ) return;
        delete[] buf;
#if __DEBUG
        buf = nullptr;
#endif
    }

    ByteBuffer::~ByteBuffer()
    {
        Dispose();
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
            Dispose();
            buf = (char*)_buf;
            bufLen = _bufLen;
            dataLen = _dataLen;
            isRef = _isRef;
            return;
        }
        if( bufLen < _dataLen )
        {
            Dispose();
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
        Dispose();
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
            Dispose();
            bufLen = (int)Round2n( other.dataLen );
            isRef = false;
            buf = new char[ bufLen ];
            memcpy( buf, other.buf, other.dataLen );
        }
        return *this;
    }

    ByteBuffer& ByteBuffer::operator=( ByteBuffer&& other )
    {
        Dispose();
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






    void ByteBuffer::WriteTo( ByteBuffer& fb )
    {
        fb.Write( dataLen );
        fb.Write( buf, dataLen );
    }

    void ByteBuffer::FastWriteTo( ByteBuffer& fb )
    {
        fb.FastWrite( dataLen );
        fb.FastWrite( buf, dataLen );
    }

    bool ByteBuffer::ReadFrom( ByteBuffer& fb )
    {
        int len;
        if( !fb.Read( len )
            || len < 0
            || fb.offset + len > fb.dataLen ) return false;
        Clear();
        Reserve( len );
        dataLen = len;
        memcpy( buf, fb.buf + fb.offset, len );
        fb.offset += len;
        return true;
    }

}
