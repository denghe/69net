#include "All.h"

namespace xxx
{

    BlockBuffer::BlockBuffer( Pool& p )
        : _wp( nullptr )
        , _rp( nullptr )
        , _wpLen( 0 )
        , _rpLen( 0 )
        , _size( 0 )
        , _pool( &p )
    {
        assert( p.ItemBufLen() - sizeof( Page* ) > 0 );
    }

    BlockBuffer::~BlockBuffer()
    {
        Clear();
    }

    void BlockBuffer::Clear()
    {
        Page* p = _rp;
        _wp->next = nullptr;
        while( p )
        {
            _pool->Free( p );
            p = p->next;
        }
        _wp = _rp = nullptr;
        _wpLen = _rpLen = _size = 0;
    }

    bool BlockBuffer::Empty() const
    {
        return _size == 0;
    }

    int BlockBuffer::Size() const
    {
        return _size;
    }

    void BlockBuffer::Write( char const* buf, int len )
    {
        assert( buf );
        if( !len ) return;
        auto bak = len;

        int ps = _pool->ItemBufLen() - sizeof( Page* );
        if( !_wp ) _rp = _wp = (Page*)_pool->Alloc();
Begin:
        auto space = ps - _wpLen;
        if( len <= space )
        {
            memcpy( _wp->data + _wpLen, buf, len );
            _wpLen += len;
            _size += bak;
            return;
        }
        memcpy( _wp->data + _wpLen, buf, space );
        buf += space;
        len -= space;
        _wp->next = (Page*)_pool->Alloc();
        _wp = _wp->next;
        _wpLen = 0;
        goto Begin;
    }

    void BlockBuffer::Copy( char* buf, int len )
    {
        assert( len <= _size );
        int ps = _pool->ItemBufLen() - sizeof( Page* );
        auto rp = _rp;
        int rpLen = _rpLen;
Begin:
        auto space = ps - _rpLen;
        if( len <= space )
        {
            memcpy( buf, rp->data + rpLen, len );
            return;
        }
        memcpy( buf, rp->data + rpLen, space );
        buf += space;
        len -= space;
        rp = rp->next;
        rpLen = 0;
        goto Begin;
    }

    int BlockBuffer::Read( char* buf, int len )
    {
        if( !_size ) return 0;
        int ps = _pool->ItemBufLen() - sizeof( Page* );
        if( len > _size ) len = _size;
        _size -= len;
        int bak = len;
Begin:
        auto space = ps - _rpLen;
        if( len <= space )
        {
            memcpy( buf, _rp->data + _rpLen, len );
            _rpLen += len;
            return bak;
        }
        memcpy( buf, _rp->data + _rpLen, space );
        buf += space;
        len -= space;
        auto next = _rp->next;
        _pool->Free( _rp );
        _rp = next;
        _rpLen = 0;
        goto Begin;
    }

}
