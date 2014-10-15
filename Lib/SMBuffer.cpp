#include "All.h"

namespace xxx
{


    void SMBuffer::Init( int bufLen )
    {
        _bufLen = bufLen - sizeof( int ) * 4;   // - sizeof( _bufLen, _wo, _ro, _size );
        Clear();
    }

    int SMBuffer::Size() const
    {
        return _size;
    }

    int SMBuffer::Space() const
    {
        return _bufLen - _size;
    }

    bool SMBuffer::Empty() const
    {
        return _size == 0;
    }

    void SMBuffer::Clear()
    {
        _size = 0;
        _ro = 0;
        _wo = 0;
    }

    int SMBuffer::Write( char const* buf, int len )
    {
        int size = _size;
        int free = _bufLen - size;
        if( len == 0 || free == 0 ) return 0;
        else if( len > free ) len = free;

        auto rightSpace = _bufLen - _wo;
        if( _wo <= size || rightSpace >= len )
        {
            memcpy( _buf + _wo, buf, len );
            if( _wo + len == _bufLen ) _wo = 0;
            else _wo += len;
        }
        else
        {
            memcpy( _buf + _wo, buf, rightSpace );
            memcpy( _buf, buf + rightSpace, len - rightSpace );
            _wo = len - rightSpace;
        }

        _size += len;
        return len;
    }

    int SMBuffer::Read( char* dest, int len )
    {
        int size = _size;
        if( len == 0 || size == 0 ) return 0;
        else if( len > size ) len = size;

        auto rightLen = _bufLen - _ro;
        if( _ro + size <= _bufLen || rightLen >= len )
        {
            memcpy( dest, _buf + _ro, len );
            if( _ro + len == _bufLen ) _ro = 0;
            else _ro += len;
        }
        else
        {
            memcpy( dest, _buf + _ro, rightLen );
            memcpy( dest + rightLen, _buf, len - rightLen );
            _ro = len - rightLen;
        }

        _size -= len;
        return len;
    }

    int SMBuffer::Copy( char* dest, int len )
    {
        int size = _size;
        if( len == 0 || size == 0 ) return 0;
        else if( len > size ) len = size;

        auto rightLen = _bufLen - _ro;
        if( _ro + size <= _bufLen || rightLen >= len )
        {
            memcpy( dest, _buf + _ro, len );
            //if( _ro + len == _bufLen ) _ro = 0;
            //else _ro += len;
        }
        else
        {
            memcpy( dest, _buf + _ro, rightLen );
            memcpy( dest + rightLen, _buf, len - rightLen );
            //_ro = len - rightLen;
        }

        //_size -= len;
        return len;
    }

    int SMBuffer::Skip( int len )
    {
        int size = _size;
        if( len == 0 || size == 0 ) return 0;
        else if( len > size ) len = size;

        auto rightLen = _bufLen - _ro;
        if( _ro + size <= _bufLen || rightLen >= len )
        {
            //memcpy( dest, _buf + _ro, len );
            if( _ro + len == _bufLen ) _ro = 0;
            else _ro += len;
        }
        else
        {
            //memcpy( dest, _buf + _ro, rightLen );
            //memcpy( dest + rightLen, _buf, len - rightLen );
            _ro = len - rightLen;
        }

        _size -= len;
        return len;
    }


}
