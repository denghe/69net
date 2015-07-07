#include "All.h"

namespace xxx
{

    BlockBuffer::BlockBuffer( Pool& p )
        : wp( nullptr )
        , rp( nullptr )
        , wpLen( 0 )
        , rpLen( 0 )
        , size( 0 )
        , pool( &p )
    {
        assert( p.ItemBufLen() - sizeof( Page* ) > 0 );
    }

    BlockBuffer::~BlockBuffer()
    {
        Clear();
    }

    void BlockBuffer::Clear()
    {
        Page* _p = rp;
        wp->next = nullptr;
        while( _p )
        {
            pool->Free( _p );
            _p = _p->next;
        }
        wp = rp = nullptr;
        wpLen = rpLen = size = 0;
    }

    bool BlockBuffer::Empty() const
    {
        return size == 0;
    }

    int BlockBuffer::Size() const
    {
        return size;
    }

    void BlockBuffer::Write( char const* buf, int len )
    {
        assert( buf );
        if( !len ) return;
        auto _len = len;

        int ps = pool->ItemBufLen() - sizeof( Page* );
        if( !wp ) rp = wp = (Page*)pool->Alloc();
Begin:
        auto _space = ps - wpLen;
        if( len <= _space )
        {
            memcpy( wp->data + wpLen, buf, len );
            wpLen += len;
            size += _len;
            return;
        }
        memcpy( wp->data + wpLen, buf, _space );
        buf += _space;
        len -= _space;
        wp->next = (Page*)pool->Alloc();
        wp = wp->next;
        wpLen = 0;
        goto Begin;
    }

    void BlockBuffer::Copy( char* buf, int len )
    {
        assert( len <= size );
        int ps = pool->ItemBufLen() - sizeof( Page* );
        auto _rp = rp;
        int _rpLen = rpLen;
Begin:
        auto _space = ps - rpLen;
        if( len <= _space )
        {
            memcpy( buf, _rp->data + _rpLen, len );
            return;
        }
        memcpy( buf, _rp->data + _rpLen, _space );
        buf += _space;
        len -= _space;
        _rp = _rp->next;
        _rpLen = 0;
        goto Begin;
    }

    int BlockBuffer::Read( char* buf, int len )
    {
        if( !size ) return 0;
        int ps = pool->ItemBufLen() - sizeof( Page* );
        if( len > size ) len = size;
        size -= len;
        int _len = len;
Begin:
        auto _space = ps - rpLen;
        if( len <= _space )
        {
            memcpy( buf, rp->data + rpLen, len );
            rpLen += len;
            return _len;
        }
        memcpy( buf, rp->data + rpLen, _space );
        buf += _space;
        len -= _space;
        auto _next = rp->next;
        pool->Free( rp );
        rp = _next;
        rpLen = 0;
        goto Begin;
    }

}
