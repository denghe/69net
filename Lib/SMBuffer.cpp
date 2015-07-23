#include "All.h"

namespace xxx
{


    void SMBuffer::Init( int _bufLen )
    {
        bufLen = _bufLen - sizeof( int ) * 4;   // - sizeof( bufLen, wo, ro, size );
        Clear();
    }

    int SMBuffer::Size() const
    {
        return size;
    }

    int SMBuffer::Space() const
    {
        return bufLen - size;
    }

    bool SMBuffer::Empty() const
    {
        return size == 0;
    }

    void SMBuffer::Clear()
    {
        size = 0;
		// todo: std::memory_order
        ro = 0;
        wo = 0;
    }

    int SMBuffer::Write( char const* _buf, int len )
    {
        int _size = size;
        int free = bufLen - _size;
        if( len == 0 || free == 0 ) return 0;
        else if( len > free ) len = free;

        auto rightSpace = bufLen - wo;
        if( wo <= _size || rightSpace >= len )
        {
            memcpy( buf + wo, _buf, len );
            if( wo + len == bufLen ) wo = 0;
            else wo += len;
        }
        else
        {
            memcpy( buf + wo, _buf, rightSpace );
            memcpy( buf, _buf + rightSpace, len - rightSpace );
            wo = len - rightSpace;
        }

		// todo: std::memory_order
        size += len;
        return len;
    }

    int SMBuffer::Read( char* dest, int len )
    {
        int _size = size;
        if( len == 0 || _size == 0 ) return 0;
        else if( len > _size ) len = _size;

        auto rightLen = bufLen - ro;
        if( ro + _size <= bufLen || rightLen >= len )
        {
            memcpy( dest, buf + ro, len );
            if( ro + len == bufLen ) ro = 0;
            else ro += len;
        }
        else
        {
            memcpy( dest, buf + ro, rightLen );
            memcpy( dest + rightLen, buf, len - rightLen );
            ro = len - rightLen;
        }

		// todo: std::memory_order
        size -= len;
        return len;
    }

    int SMBuffer::Copy( char* dest, int len )
    {
        int _size = size;
        if( len == 0 || _size == 0 ) return 0;
        else if( len > _size ) len = _size;

        auto rightLen = bufLen - ro;
        if( ro + _size <= bufLen || rightLen >= len )
        {
            memcpy( dest, buf + ro, len );
            //if( ro + len == bufLen ) ro = 0;
            //else ro += len;
        }
        else
        {
            memcpy( dest, buf + ro, rightLen );
            memcpy( dest + rightLen, buf, len - rightLen );
            //ro = len - rightLen;
        }

        //size -= len;
        return len;
    }

    int SMBuffer::Skip( int len )
    {
        int _size = size;
        if( len == 0 || _size == 0 ) return 0;
        else if( len > _size ) len = _size;

        auto rightLen = bufLen - ro;
        if( ro + _size <= bufLen || rightLen >= len )
        {
            //memcpy( dest, buf + ro, len );
            if( ro + len == bufLen ) ro = 0;
            else ro += len;
        }
        else
        {
            //memcpy( dest, buf + ro, rightLen );
            //memcpy( dest + rightLen, buf, len - rightLen );
            ro = len - rightLen;
        }

		// todo: std::memory_order
        size -= len;
        return len;
    }


}
