#include "Precompile.h"


// todo: 根据 linux std 的 vector<bool> 的做法，是将最后正在操作的那个 uint64 单元存起来（表现为 it ），
// 这样追加的时候就可以极快的计算偏移，以后再改


List<bool>::List()
{
    _len = 0;
    _bufSize = ListDefaultBufferSize;
    _buf = (decltype(_buf))malloc( ListDefaultBufferSize );
    memset( _buf, 0, _bufSize );
}
List<bool>::List( int capacity )
{
    if( capacity < 64 ) capacity = 64;
    _len = 0;
    _bufSize = (int)round2n( (capacity - 1) / 8 + 1 );
    _buf = (decltype(_buf))malloc( _bufSize );
    memset( _buf, 0, _bufSize );
}
List<bool>::List( TT const & o )
{
    _bufSize = o._bufSize;
    auto tmp = _len - o._len;
    _len = o._len;
    _buf = (decltype(_buf))malloc( _bufSize );
    memcpy( _buf, o._buf, (_len - 1) / 8 + 1 );
    if( tmp > 0 )	// 如果原串 _len 长过 o._len 则还需要清 0 长出来的区域
    {
    	memset( _buf + (_len - 1) / 8 + 1, 0, tmp >> 3 );
    }
}
List<bool>::List( TT && o )
{
    _buf = o._buf;
    _bufSize = o._bufSize;
    _len = o._len;
    o._buf = nullptr;
}
List<bool>::~List()
{
    if( _buf ) free( _buf );
}
void List<bool>::clear()
{
	memset( _buf, 0, (_len - 1) / 8 + 1 );
    _len = 0;
}
int List<bool>::getFreeSpace()
{
    return _bufSize * 8 - _len;
}
void List<bool>::ensure( int len )
{
    if( getFreeSpace() < len )
    {
        _bufSize = (int)round2n( ( _len + len - 1 ) / 8 + 1 );
        _buf = (decltype(_buf))realloc( _buf, _bufSize );
    }
}
void List<bool>::add( bool v )
{
    ensure( 1 );
    addDirect( v );
}
void List<bool>::addDirect( bool v )
{
	set( _len++, v );
}
List<bool> & List<bool>::operator<<( bool v )
{
    add( v );
    return *this;
}
void List<bool>::set( int idx, bool v )
{
    assert( idx < _len );
    if( v )
        ((size_t*)_buf)[ idx / (sizeof(size_t)*8) ] |= (size_t(1) << (idx % (sizeof(size_t)*8)));
    else
        ((size_t*)_buf)[ idx / (sizeof(size_t)*8) ] &= ~(size_t(1) << (idx % (sizeof(size_t)*8)));
}
bool List<bool>::get( int idx ) const
{
    return ( ((size_t*)_buf)[ idx / (sizeof(size_t)*8) ] & (size_t(1) << (idx % (sizeof(size_t)*8))) ) > 0;
}
bool List<bool>::operator[]( int idx ) const
{
    return get( idx );
}
void List<bool>::toString( String & s ) const
{
    s << "{ ";
    for( int i = 0; i < _len; ++i )
    {
        if( i ) s << ", ";
        s << (*this)[ i ];
    }
    s << " }";
}





