#include "All.h"

namespace xxx
{


    Pool FlatBuffer::emptyPool;     // for do not need delete's buffer

    FlatBuffer::FlatBuffer( int capacity )
    {
        if( capacity < 1024 ) bufLen = 1024;
        else bufLen = (int)Round2n( capacity );
        disposer = nullptr;
        buf = new char[ bufLen ];
        dataLen = offset = 0;
    }

    FlatBuffer::FlatBuffer( Pool& p )
    {
        buf = (char*)p.Alloc();
        bufLen = p.ItemBufLen();
        dataLen = 0;
        disposer = &p;
    }

    FlatBuffer::FlatBuffer( char* _buf, int _bufLen, int _dataLen /*= 0*/, bool isRef /*= false */ )
    {
        dataLen = _dataLen;
        offset = 0;
        if( isRef )
        {
            buf = _buf;
            bufLen = _bufLen;
            disposer = &emptyPool;
        }
        else
        {
            bufLen = (int)Round2n( _dataLen );
            buf = new char[ bufLen ];
            memcpy( buf, _buf, _dataLen );
            disposer = nullptr;
        }
    }

    FlatBuffer::FlatBuffer( FlatBuffer const & other )
        : FlatBuffer( other.dataLen + 1 )
    {
        memcpy( buf, other.buf, other.dataLen + 1 );
        dataLen = other.dataLen;
    }

    FlatBuffer::FlatBuffer( FlatBuffer&& other )
        : buf( other.buf )
        , bufLen( other.bufLen )
        , dataLen( other.dataLen )
        , offset( other.offset )
        , disposer( other.disposer )
    {
        other.buf = nullptr;
        other.bufLen = 0;
        other.dataLen = 0;
        other.offset = 0;
        other.disposer = nullptr;
    }

    FlatBuffer::~FlatBuffer()
    {
        Dispose();
    }

    void FlatBuffer::Clear()
    {
        dataLen = offset = 0;
    }

    bool FlatBuffer::Empty() const
    {
        return dataLen == 0;
    }

    bool FlatBuffer::Full() const
    {
        return dataLen == offset;
    }

    int FlatBuffer::Size() const
    {
        return dataLen;
    }

    int& FlatBuffer::Size()
    {
        return dataLen;
    }

    char const* FlatBuffer::Data() const
    {
        return buf;
    }

    char* FlatBuffer::Data()
    {
        return buf;
    }

    void FlatBuffer::Assign( char const* _buf, int _bufLen, int _dataLen /*= 0*/, bool isRef /*= false */ )
    {
        assert( buf != _buf );
        offset = 0;
        if( isRef )
        {
            Dispose();
            buf = (char*)_buf;
            bufLen = _bufLen;
            dataLen = _dataLen;
            disposer = &emptyPool;
            return;
        }
        if( bufLen < _dataLen )
        {
            Dispose();
            bufLen = (int)Round2n( _dataLen );
            disposer = nullptr;
            buf = new char[ bufLen ];
        }
        dataLen = _dataLen;
        memcpy( buf, _buf, _dataLen );
    }

    void FlatBuffer::Reserve( int capacity )
    {
        if( capacity <= bufLen ) return;
        bufLen = (int)Round2n( capacity );
        auto newBuf = new char[ bufLen ];
        memcpy( newBuf, buf, dataLen );
        Dispose();
        disposer = nullptr;
        buf = newBuf;
    }

    void FlatBuffer::Resize( int _dataLen )
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

    FlatBuffer& FlatBuffer::operator=( FlatBuffer const& other )
    {
        if( this == &other ) return *this;
        offset = 0;
        dataLen = other.dataLen;
        if( bufLen >= other.dataLen )
        {
            memcpy( buf, other.buf, other.dataLen );
        }
        else
        {
            Dispose();
            bufLen = (int)Round2n( other.dataLen );
            disposer = nullptr;
            buf = new char[ bufLen ];
            memcpy( buf, other.buf, other.dataLen );
        }
        return *this;
    }

    FlatBuffer& FlatBuffer::operator=( FlatBuffer&& other )
    {
        Dispose();
        buf = other.buf;
        bufLen = other.bufLen;
        dataLen = other.dataLen;
        offset = 0;
        disposer = other.disposer;
        other.buf = nullptr;
        other.bufLen = 0;
        other.dataLen = 0;
        other.offset = 0;
        other.disposer = nullptr;
        return *this;
    }

    char FlatBuffer::operator[]( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char& FlatBuffer::operator[]( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char FlatBuffer::At( int idx ) const
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    char& FlatBuffer::At( int idx )
    {
        assert( idx >= 0 && idx < dataLen );
        return buf[ idx ];
    }

    int& FlatBuffer::Offset()
    {
        return offset;
    }

    int FlatBuffer::Offset() const
    {
        return offset;
    }

    String FlatBuffer::Dump()
    {
        String rtv;
        BufferUtils::Dump( rtv, buf, dataLen );
        return rtv;
    }



    int FlatBuffer::GetWriteBufferSize()
    {
        return sizeof( int ) + Size();
    }
    void FlatBuffer::WriteBuffer( FlatBuffer& fb )
    {
        fb.Write( dataLen );
        fb.Write( buf, dataLen );
    }
    void FlatBuffer::WriteBufferDirect( FlatBuffer& fb )
    {
        fb.WriteDirect( dataLen );
        fb.WriteDirect( buf, dataLen );
    }
    bool FlatBuffer::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0
            || fb.Offset() + len > fb.Size() ) return false;
        Clear();
        Reserve( len );
        dataLen = len;
        memcpy( buf, fb.Data() + fb.Offset(), len );
        fb.Offset() += len;
        return true;
    }



    void FlatBuffer::WriteDirect( char const* _buf, int _dataLen )
    {
        memcpy( buf + dataLen, _buf, _dataLen );
        dataLen += _dataLen;
    }
    void FlatBuffer::Write( char const* _buf, int _dataLen )
    {
        Reserve( dataLen + _dataLen );
        WriteDirect( _buf, _dataLen );
    }

    bool FlatBuffer::Read( char* _buf, int _dataLen )
    {
        if( offset + _dataLen > dataLen ) return false;
        memcpy( _buf, buf + offset, _dataLen );
        offset += _dataLen;
        return true;
    }

    void FlatBuffer::Dispose()
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


}
