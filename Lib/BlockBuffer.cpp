#include "All.h"

namespace xxx
{

    BlockBuffer::BlockBuffer( Pool& _p )
        : wp( nullptr )
        , rp( nullptr )
        , wpLen( 0 )
        , rpLen( 0 )
        , size( 0 )
        , pool( &_p )
    {
        assert( _p.ItemBufLen() - sizeof( Page* ) > 0 );
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

    void BlockBuffer::Write( char const* _buf, int _len )
    {
        assert( _buf );
        if( !_len ) return;
        auto _bak = _len;

        int _ps = pool->ItemBufLen() - sizeof( Page* );
        if( !wp ) rp = wp = (Page*)pool->Alloc();
Begin:
        auto _space = _ps - wpLen;
        if( _len <= _space )
        {
            memcpy( wp->data + wpLen, _buf, _len );
            wpLen += _len;
            size += _bak;
            return;
        }
        memcpy( wp->data + wpLen, _buf, _space );
        _buf += _space;
        _len -= _space;
        wp->next = (Page*)pool->Alloc();
        wp = wp->next;
        wpLen = 0;
        goto Begin;
    }

    void BlockBuffer::Copy( char* _buf, int _len )
    {
        assert( _len <= size );
        int _ps = pool->ItemBufLen() - sizeof( Page* );
        auto _rp = rp;
        int _rpLen = rpLen;
Begin:
        auto _space = _ps - rpLen;
        if( _len <= _space )
        {
            memcpy( _buf, _rp->data + _rpLen, _len );
            return;
        }
        memcpy( _buf, _rp->data + _rpLen, _space );
        _buf += _space;
        _len -= _space;
        _rp = _rp->next;
        _rpLen = 0;
        goto Begin;
    }

    int BlockBuffer::Read( char* _buf, int _len )
    {
        if( !size ) return 0;
        int _ps = pool->ItemBufLen() - sizeof( Page* );
        if( _len > size ) _len = size;
        size -= _len;
        int _bak = _len;
Begin:
        auto _space = _ps - rpLen;
        if( _len <= _space )
        {
            memcpy( _buf, rp->data + rpLen, _len );
            rpLen += _len;
            return _bak;
        }
        memcpy( _buf, rp->data + rpLen, _space );
        _buf += _space;
        _len -= _space;
        auto _next = rp->next;
        pool->Free( rp );
        rp = _next;
        rpLen = 0;
        goto Begin;
    }

}
