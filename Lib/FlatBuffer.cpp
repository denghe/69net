#include "All.h"

namespace xxx
{


    Pool FlatBuffer::_emptyPool;     // for do not need delete's buffer

    FlatBuffer::FlatBuffer( int capacity )
    {
        if( capacity < 1024 ) _bufLen = 1024;
        else _bufLen = (int)Round2n( capacity );
        _disposer = nullptr;
        _buf = new char[ _bufLen ];
        _dataLen = _offset = 0;
    }

    FlatBuffer::FlatBuffer( Pool& p )
    {
        _buf = (char*)p.Alloc();
        _bufLen = p.ItemBufLen();
        _dataLen = 0;
        _disposer = &p;
    }

    FlatBuffer::FlatBuffer( char* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
    {
        _dataLen = dataLen;
        _offset = 0;
        if( isRef )
        {
            _buf = buf;
            _bufLen = bufLen;
            _disposer = &_emptyPool;
        }
        else
        {
            _bufLen = (int)Round2n( dataLen );
            _buf = new char[ _bufLen ];
            memcpy( _buf, buf, dataLen );
            _disposer = nullptr;
        }
    }

    FlatBuffer::FlatBuffer( FlatBuffer const & other )
        : FlatBuffer( other._dataLen + 1 )
    {
        memcpy( _buf, other._buf, other._dataLen + 1 );
        _dataLen = other._dataLen;
    }

    FlatBuffer::FlatBuffer( FlatBuffer&& other )
        : _buf( other._buf )
        , _bufLen( other._bufLen )
        , _dataLen( other._dataLen )
        , _offset( other._offset )
        , _disposer( other._disposer )
    {
        other._buf = nullptr;
        other._bufLen = 0;
        other._dataLen = 0;
        other._offset = 0;
        other._disposer = nullptr;
    }

    FlatBuffer::~FlatBuffer()
    {
        Dispose();
    }

    void FlatBuffer::Clear()
    {
        _dataLen = _offset = 0;
    }

    bool FlatBuffer::Empty() const
    {
        return _dataLen == 0;
    }

    bool FlatBuffer::Full() const
    {
        return _dataLen == _offset;
    }

    int FlatBuffer::Size() const
    {
        return _dataLen;
    }

    int& FlatBuffer::Size()
    {
        return _dataLen;
    }

    char const* FlatBuffer::Data() const
    {
        return _buf;
    }

    char* FlatBuffer::Data()
    {
        return _buf;
    }

    void FlatBuffer::Assign( char const* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= false */ )
    {
        assert( _buf != buf );
        _offset = 0;
        if( isRef )
        {
            Dispose();
            _buf = (char*)buf;
            _bufLen = bufLen;
            _dataLen = dataLen;
            _disposer = &_emptyPool;
            return;
        }
        if( _bufLen < dataLen )
        {
            Dispose();
            _bufLen = (int)Round2n( dataLen );
            _disposer = nullptr;
            _buf = new char[ _bufLen ];
        }
        _dataLen = dataLen;
        memcpy( _buf, buf, dataLen );
    }

    void FlatBuffer::Reserve( int capacity )
    {
        if( capacity <= _bufLen ) return;
        _bufLen = (int)Round2n( capacity );
        auto newBuf = new char[ _bufLen ];
        memcpy( newBuf, _buf, _dataLen );
        Dispose();
        _disposer = nullptr;
        _buf = newBuf;
    }

    void FlatBuffer::Resize( int dataLen )
    {
        if( dataLen == _dataLen ) return;
        else if( dataLen < _dataLen )
        {
            _dataLen = dataLen;
            if( _dataLen > _offset ) _offset = _dataLen;
        }
        else
        {
            Reserve( dataLen );
            _dataLen = dataLen;
        }
    }

    FlatBuffer& FlatBuffer::operator=( FlatBuffer const& other )
    {
        if( this == &other ) return *this;
        _offset = 0;
        _dataLen = other._dataLen;
        if( _bufLen >= other._dataLen )
        {
            memcpy( _buf, other._buf, other._dataLen );
        }
        else
        {
            Dispose();
            _bufLen = (int)Round2n( other._dataLen );
            _disposer = nullptr;
            _buf = new char[ _bufLen ];
            memcpy( _buf, other._buf, other._dataLen );
        }
        return *this;
    }

    FlatBuffer& FlatBuffer::operator=( FlatBuffer&& other )
    {
        Dispose();
        _buf = other._buf;
        _bufLen = other._bufLen;
        _dataLen = other._dataLen;
        _offset = 0;
        _disposer = other._disposer;
        other._buf = nullptr;
        other._bufLen = 0;
        other._dataLen = 0;
        other._offset = 0;
        other._disposer = nullptr;
        return *this;
    }

    char FlatBuffer::operator[]( int idx ) const
    {
        assert( idx >= 0 && idx < _dataLen );
        return _buf[ idx ];
    }

    char& FlatBuffer::operator[]( int idx )
    {
        assert( idx >= 0 && idx < _dataLen );
        return _buf[ idx ];
    }

    char FlatBuffer::At( int idx ) const
    {
        assert( idx >= 0 && idx < _dataLen );
        return _buf[ idx ];
    }

    char& FlatBuffer::At( int idx )
    {
        assert( idx >= 0 && idx < _dataLen );
        return _buf[ idx ];
    }

    int& FlatBuffer::Offset()
    {
        return _offset;
    }

    int FlatBuffer::Offset() const
    {
        return _offset;
    }

    String FlatBuffer::Dump()
    {
        String rtv;
        BufferUtils::Dump( rtv, _buf, _dataLen );
        return rtv;
    }



    int FlatBuffer::GetWriteBufferSize()
    {
        return sizeof( int ) + Size();
    }
    void FlatBuffer::WriteBuffer( FlatBuffer& fb )
    {
        fb.Write( _dataLen );
        fb.Write( _buf, _dataLen );
    }
    void FlatBuffer::WriteBufferDirect( FlatBuffer& fb )
    {
        fb.WriteDirect( _dataLen );
        fb.WriteDirect( _buf, _dataLen );
    }
    bool FlatBuffer::ReadBuffer( FlatBuffer& fb )
    {
        int len;
        if( !fb.Read( len ) || len < 0
            || fb.Offset() + len > fb.Size() ) return false;
        Clear();
        Reserve( len );
        _dataLen = len;
        memcpy( _buf, fb.Data() + fb.Offset(), len );
        fb.Offset() += len;
        return true;
    }



    void FlatBuffer::WriteDirect( char const* buf, int dataLen )
    {
        memcpy( _buf + _dataLen, buf, dataLen );
        _dataLen += dataLen;
    }
    void FlatBuffer::Write( char const* buf, int dataLen )
    {
        Reserve( _dataLen + dataLen );
        WriteDirect( buf, dataLen );
    }

    bool FlatBuffer::Read( char* buf, int dataLen )
    {
        if( _offset + dataLen > _dataLen ) return false;
        memcpy( buf, _buf + _offset, dataLen );
        _offset += dataLen;
        return true;
    }

    void FlatBuffer::Dispose()
    {
        if( _disposer )
        {
            if( _disposer->ItemBufLen() ) _disposer->Free( _buf );
        }
        else
        {
            delete[] _buf;
        }
    }


}
