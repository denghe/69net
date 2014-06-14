#include "Precompile.h"

Bytes const Bytes::defaultValue;

Bytes::~Bytes()
{
    if( _buf ) free( _buf );
}

Bytes::Bytes( Bytes && o )
	: _readIdx( o._readIdx )
	, _len( o._len )
	, _bufSize( o._bufSize )
	, _buf( o._buf )
{
    o._buf = nullptr;
}

Bytes::Bytes( Bytes const & o )
	: _readIdx( o._readIdx )
	, _len( o._len )
	, _bufSize( o._bufSize )
{
    _buf = (char*)malloc( o._bufSize );
    memcpy( _buf, o._buf, o._bufSize );
}


Bytes & Bytes::operator=( Bytes const & o )
{
    if( _bufSize < o._len )
    {
        _bufSize = (int)round2n( o._len );
        free( _buf );
        _buf = (char*)malloc( _bufSize );
    }
    memcpy( _buf, o._buf, o._len );
    _len = o._len;
    _readIdx = o._readIdx;
    return *this;
}

Bytes & Bytes::operator=( Bytes && o )
{
    std::swap( _buf, o._buf );
    _bufSize = o._bufSize;
    _len = o._len;
    _readIdx = o._readIdx;
    return *this;
}


Bytes::Bytes( int bufSize /*= 4096 */ )
	: _readIdx( 0 )
	, _len( 0 )
{
    _bufSize = (int)round2n( bufSize );
    _buf = (char*)malloc( _bufSize );
}

void Bytes::clear( int bufSize )
{
    if( _bufSize < bufSize )
    {
        _bufSize = (int)round2n( bufSize );
        free( _buf );
        _buf = (char*)malloc( _bufSize );
    }
    _readIdx = 0;
    _len = 0;
}

void Bytes::clear()
{
    _readIdx = _len = 0;
}

void Bytes::clearLength()
{
    _len = 0;
}

void Bytes::clearReadIndex()
{
    _readIdx = 0;
}


int Bytes::getFreeReadSpace() const
{
    return _len - _readIdx;
}

int Bytes::getFreeWriteSpace() const
{
    return _bufSize - _len;
}

void Bytes::ensure( int len )
{
    if( getFreeWriteSpace() < len )
    {
        _bufSize = (int)round2n( _len + len );
        _buf = (char*)realloc( _buf, _bufSize );
    }
}

void Bytes::dump( String & s ) const
{
    s << "Bytes Dump: Buffer Size = " << _bufSize << ", Data Length = " << _len;
    BinaryHelper::dump( s, _buf, _len );
}

void Bytes::toString( String & s ) const
{
    s.ensure( _len * 2 );
    char *p = s._buf + s._len;
    for( int i = 0; i < _len; ++i )
    {
        Converts::toStringHex( p + i * 2, (uint8)_buf[ i ] );
    }
    s._len += _len * 2;
}






void Bytes::writeDirect( uint64 v )
{
    BinaryHelper::writeDirect764( _buf, _len, v );
}
void Bytes::writeDirect( uint32 v )
{
    BinaryHelper::writeDirect732( _buf, _len, v );
}
void Bytes::writeDirect( uint16 v )
{
    BinaryHelper::writeDirect732( _buf, _len, v );
}

void Bytes::writeDirect( int64 v )
{
    BinaryHelper::writeDirect764( _buf, _len, BinaryHelper::zegZagEncode( v ) );
}
void Bytes::writeDirect( int32 v )
{
    BinaryHelper::writeDirect732( _buf, _len, BinaryHelper::zegZagEncode( v ) );
}
void Bytes::writeDirect( int16 v )
{
    BinaryHelper::writeDirect732( _buf, _len, BinaryHelper::zegZagEncode( v ) );
}

#define BYTES_DW( T )                      \
void Bytes::writeDirect( T v )             \
{                                          \
    BinaryHelper::write( _buf, _len, v );  \
}
BYTES_DW( int8   )
BYTES_DW( uint8  )
BYTES_DW( bool   )
BYTES_DW( float  )
BYTES_DW( double )

void Bytes::writeDirect( const char * v, int len )
{
    writeDirect( (uint)len );
    BinaryHelper::write( _buf, _len, v, len );
}

void Bytes::writeDirect( Bytes const & v )
{
    writeDirect( v._buf, v._len );
}

void Bytes::writeDirect( String const & v )
{
    writeDirect( v._buf, v._len );
}

void Bytes::writeDirect( const char * v )
{
    writeDirect( v, (uint)strlen( v ) );
}

void Bytes::writeDirect( DateTime const & v )
{
    BinaryHelper::write( _buf, _len, v._value );
}






void Bytes::readDirect( uint64 & v )
{
    BinaryHelper::readDirect764( v, _buf, _readIdx );
}
void Bytes::readDirect( uint32 & v )
{
    BinaryHelper::readDirect732( v, _buf, _readIdx );
}
void Bytes::readDirect( uint16 & v )
{
    uint32 i;
    BinaryHelper::readDirect732( i, _buf, _readIdx );
    v = (uint16)i;
}

void Bytes::readDirect( int64 & v )
{
    uint64 i;
    readDirect( i );
    v = BinaryHelper::zegZagDecode( i );
}
void Bytes::readDirect( int32 & v )
{
    uint32 i;
    readDirect( i );
    v = BinaryHelper::zegZagDecode( i );
}
void Bytes::readDirect( int16 & v )
{
    uint16 i;
    readDirect( i );
    v = BinaryHelper::zegZagDecode( i );
}

#define BYTES_DR( T )                          \
void Bytes::readDirect( T & v )                \
{                                              \
    BinaryHelper::read( v, _buf, _readIdx );   \
}

BYTES_DR( uint8  )
BYTES_DR( int8   )
BYTES_DR( bool   )
BYTES_DR( float  )
BYTES_DR( double )

void Bytes::readDirect( Bytes & v )
{
    uint len = 0;
    readDirect( len );
    v.clear( len );
    memcpy( v._buf, _buf + _readIdx, len );
    _readIdx += len;
}

void Bytes::readDirect( String & v )
{
    uint len = 0;
    readDirect( len );
    v.assign( _buf + _readIdx, len );
    _readIdx += len;
}

void Bytes::readDirect( DateTime & v )
{
    BinaryHelper::read( v._value, _buf, _readIdx );
}





#define BYTES_W( T )        \
void Bytes::write( T v )    \
{                           \
    ensure( sizeof(T) + 1 );\
    writeDirect( v );       \
}

BYTES_W( uint64 )
BYTES_W( uint32 )
BYTES_W( uint16 )
BYTES_W( uint8  )
BYTES_W( int64  )
BYTES_W( int32  )
BYTES_W( int16  )
BYTES_W( int8   )
BYTES_W( bool   )
BYTES_W( float  )
BYTES_W( double )

void Bytes::write( const char * v, int len )
{
    ensure( sizeof(int) + 1 + len  ); writeDirect( v, (uint)len );
}

void Bytes::write( Bytes const & v )
{
    write( v._buf, v._len );
}

void Bytes::write( String const & v )
{
    write( v._buf, v._len );
}

void Bytes::write( const char * v )
{
    write( v, (int)strlen( v ) );
}

void Bytes::write( DateTime const & v )
{
    write( v._value );
}






ReadStatus Bytes::read( uint64 & v )
{
    return BinaryHelper::read764( v, _buf, _len, _readIdx );
}
ReadStatus Bytes::read( uint32 & v )
{
    return BinaryHelper::read732( v, _buf, _len, _readIdx );
}
ReadStatus Bytes::read( uint16 & v )
{
    return BinaryHelper::read716( v, _buf, _len, _readIdx );
}

ReadStatus Bytes::read( int64 & v )
{
    auto rtv = BinaryHelper::read764( (uint64&)v, _buf, _len, _readIdx );
    if( rtv != ReadStatus::Success ) return rtv;
    v = BinaryHelper::zegZagDecode( (uint64)v );
    return ReadStatus::Success;
}
ReadStatus Bytes::read( int32 & v )
{
    auto rtv = BinaryHelper::read732( (uint32&)v, _buf, _len, _readIdx );
    if( rtv != ReadStatus::Success ) return rtv;
    v = BinaryHelper::zegZagDecode( (uint32)v );
    return ReadStatus::Success;
}
ReadStatus Bytes::read( int16 & v )
{
    auto rtv = BinaryHelper::read716( (uint16&)v, _buf, _len, _readIdx );
    if( rtv != ReadStatus::Success ) return rtv;
    v = BinaryHelper::zegZagDecode( (uint16)v );
    return ReadStatus::Success;
}


#define BYTES_R( T )                                                        \
ReadStatus Bytes::read( T & v )                                             \
{                                                                           \
    if( getFreeReadSpace() < (int)sizeof(T) ) return ReadStatus::NotEnoughData;  \
    readDirect( v );                                                        \
    return ReadStatus::Success;                                             \
}

BYTES_R( uint8  )
BYTES_R( int8   )
BYTES_R( bool   )
BYTES_R( float  )
BYTES_R( double )

ReadStatus Bytes::read( Bytes & v )
{
    uint len = 0;
    auto rtv = read( len );
    if( rtv != ReadStatus::Success ) return rtv;
    if( getFreeReadSpace() < (int)len ) return ReadStatus::NotEnoughData;
    v.clear( len );
    memcpy( v._buf, _buf + _readIdx, len );
    _readIdx += len;
    return ReadStatus::Success;
}

ReadStatus Bytes::read( String & v )
{
    uint len = 0;
    auto rtv = read( len );
    if( rtv != ReadStatus::Success ) return rtv;
    if( getFreeReadSpace() < (int)len ) return ReadStatus::NotEnoughData;
    v.assign( _buf + _readIdx, len );
    _readIdx += len;
    return ReadStatus::Success;
}

ReadStatus Bytes::read( DateTime & v )
{
    return read( v._value );
}


















#define BYTES_DW_VECTOR( T )                                \
void Bytes::writeDirect( List<T> const & vs )               \
{                                                           \
    writeDirect( (uint)vs._len );                           \
    memcpy( _buf + _len, vs._buf, vs._len * sizeof(T) );    \
    _len += (int)sizeof(T) * vs._len;                       \
}

BYTES_DW_VECTOR( uint8  )
BYTES_DW_VECTOR( int8   )
BYTES_DW_VECTOR( float  )
BYTES_DW_VECTOR( double )
BYTES_DW_VECTOR( DateTime )

void Bytes::writeDirect( List<bool> const & vs )
{
    writeDirect( (uint)vs._len );
    int size = ( vs._len - 1 ) / 8 + 1;
    memcpy( _buf + _len, vs._buf, size );
    _len += size;
    _buf[ _len - 1 ] &= (uint8)~( (uint8)0xFF << ( vs._len % 8 ) );
}






#define BYTES_DR_VECTOR( T )                    \
void Bytes::readDirect( List<T> & vs )          \
{                                               \
    vs.clear();                                 \
    uint len = 0;                               \
    readDirect( len );                          \
    vs.ensure( len );                           \
    int size = len * (int)sizeof(T);            \
    memcpy( vs._buf, _buf + _readIdx, size );   \
    vs._len = len;                              \
    _readIdx += size;                           \
}

BYTES_DR_VECTOR( uint8  )
BYTES_DR_VECTOR( int8   )
BYTES_DR_VECTOR( float  )
BYTES_DR_VECTOR( double )
BYTES_DR_VECTOR( DateTime )

void Bytes::readDirect( List<bool> & vs )         
{                                       
    vs.clear();
    uint len = 0;
    readDirect( len );
    vs.ensure( len );
    int size = ( len - 1 ) / 8 + 1;
    memcpy( vs._buf, _buf + _readIdx, size );
    vs._len = len;
    _readIdx += size;
}






#define BYTES_W_VECTOR( T )                             \
void Bytes::write( List<T> const & vs )                 \
{                                                       \
    write( (uint)vs._len );                             \
    int size = (int)sizeof(T) * vs._len;                \
    ensure( size );                                     \
    memcpy( _buf + _len, vs._buf, size );               \
    _len += size;                                       \
}

BYTES_W_VECTOR( uint8  )
BYTES_W_VECTOR( int8   )
BYTES_W_VECTOR( float  )
BYTES_W_VECTOR( double )
BYTES_W_VECTOR( DateTime )

void Bytes::write( List<bool> const & vs )
{
    write( (uint)vs._len );
    int size = ( vs._len - 1 ) / 8 + 1;
    ensure( size );
    memcpy( _buf + _len, vs._buf, size );
    _len += size;
    _buf[ _len - 1 ] &= (uint8)~( (uint8)0xFF << ( vs._len % 8 ) );
}






#define BYTES_R_VECTOR( T )                                                     \
ReadStatus Bytes::read( List<T> & vs )                                          \
{                                                                               \
    vs.clear();                                                                 \
    uint len = 0;                                                               \
    auto rtv = read( len );                                                     \
    if( rtv != ReadStatus::Success ) return rtv;                                \
    vs.ensure( len );                                                           \
    int size = len * (int)sizeof(T);                                            \
    if( getFreeReadSpace() < size ) return ReadStatus::NotEnoughData;           \
    vs.ensure( len );                                                           \
    memcpy( vs._buf, _buf + _readIdx, size );                                   \
    vs._len = len;                                                              \
    _readIdx += size;                                                           \
    return ReadStatus::Success;                                                 \
}

BYTES_R_VECTOR( uint8  )
BYTES_R_VECTOR( int8   )
BYTES_R_VECTOR( float  )
BYTES_R_VECTOR( double )
BYTES_R_VECTOR( DateTime )

ReadStatus Bytes::read( List<bool> & vs )                          
{
    vs.clear();
    uint len = 0; 
    auto rtv = read( len );
    if( rtv != ReadStatus::Success ) return rtv;
    vs.ensure( len );
    int size = ( len - 1 ) / 8 + 1;
    if( getFreeReadSpace() < size ) return ReadStatus::NotEnoughData;
    vs.ensure( len );
    memcpy( vs._buf, _buf + _readIdx, size );
    vs._len = len;
    _readIdx += size;
    return ReadStatus::Success;
}

