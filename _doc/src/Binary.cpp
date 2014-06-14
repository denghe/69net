#include "Precompile.h"

Binary const Binary::defaultValue;

Binary::~Binary()
{
    if( _buf ) free( _buf );
}

Binary::Binary( Binary && o )
	: _readIdx( o._readIdx )
	, _len( o._len )
	, _bufSize( o._bufSize )
	, _buf( o._buf )
{
    o._buf = nullptr;
}

Binary::Binary( Binary const & o )
	: _readIdx( o._readIdx )
	, _len( o._len )
    , _bufSize( o._bufSize )
{
    _buf = (char*)malloc( o._bufSize );
    memcpy( _buf, o._buf, o._bufSize );
}


Binary & Binary::operator=( Binary const & o )
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

Binary & Binary::operator=( Binary && o )
{
    std::swap( _buf, o._buf );
    _bufSize = o._bufSize;
    _len = o._len;
    _readIdx = o._readIdx;
    return *this;
}


Binary::Binary( int bufSize /*= 4096 */ )
	: _readIdx( 0 )
    , _len( 0 )
{
    _bufSize = (int)round2n( bufSize );
    _buf = (char*)malloc( _bufSize );
}

void Binary::clear( int bufSize )
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

void Binary::clear()
{
    _readIdx = _len = 0;
}

void Binary::clearLength()
{
    _len = 0;
}

void Binary::clearReadIndex()
{
    _readIdx = 0;
}


int Binary::getFreeReadSpace() const
{
    return _len - _readIdx;
}

int Binary::getFreeWriteSpace() const
{
    return _bufSize - _len;
}

void Binary::ensure( int len )
{
    if( getFreeWriteSpace() < len )
    {
        _bufSize = (int)round2n( _len + len );
        _buf = (char*)realloc( _buf, _bufSize );
    }
}

void Binary::dump( String & s ) const
{
    s << "Binary Dump: Buffer Size = " << _bufSize << ", Data Length = " << _len;
    BinaryHelper::dump( s, _buf, _len );
}

void Binary::toString( String & s ) const
{
    s.ensure( _len * 2 );
    char *p = s._buf + s._len;
    for( int i = 0; i < _len; ++i )
    {
        Converts::toStringHex( p + i * 2, (uint8)_buf[ i ] );
    }
    s._len += _len * 2;
}





#define BINARY_DW( T )                      \
void Binary::writeDirect( T v )             \
{                                           \
    BinaryHelper::write( _buf, _len, v );   \
}

BINARY_DW( uint64 )
BINARY_DW( uint32 )
BINARY_DW( uint16 )
BINARY_DW( uint8  )
BINARY_DW( int64  )
BINARY_DW( int32  )
BINARY_DW( int16  )
BINARY_DW( int8   )
BINARY_DW( bool   )
BINARY_DW( float  )
BINARY_DW( double )


void Binary::writeDirect( Binary const & v )
{
    writeDirect( v._buf, v._len );
}

void Binary::writeDirect( String const & v )
{
    writeDirect( v._buf, v._len );
}

void Binary::writeDirect( const char * v )
{
    writeDirect( v, (int)strlen( v ) );
}

void Binary::writeDirect( const char * v, int len )
{
    BinaryHelper::write( _buf, _len, len ); 
    BinaryHelper::write( _buf, _len, v, len );
}

void Binary::writeDirect( DateTime const & v )
{
    writeDirect( v._value );
}







#define BINARY_DR( T )                          \
void Binary::readDirect( T & v )                \
{                                               \
    BinaryHelper::read( v, _buf, _readIdx );    \
}

BINARY_DR( uint64 )
BINARY_DR( uint32 )
BINARY_DR( uint16 )
BINARY_DR( uint8  )
BINARY_DR( int64  )
BINARY_DR( int32  )
BINARY_DR( int16  )
BINARY_DR( int8   )
BINARY_DR( bool   )
BINARY_DR( float  )
BINARY_DR( double )


void Binary::readDirect( Binary & v )
{
    int len = 0;
    BinaryHelper::read( len, _buf, _readIdx );
    v.clear( len );
    memcpy( v._buf, _buf + _readIdx, len );
    _readIdx += len;
}

void Binary::readDirect( String & v )
{
    int len = 0;
    BinaryHelper::read( len, _buf, _readIdx );
    v.assign( _buf + _readIdx, len );
    _readIdx += len;
}

void Binary::readDirect( DateTime & v )
{
    readDirect( v._value );
}





#define BINARY_W( T )       \
void Binary::write( T v )   \
{                           \
    ensure( sizeof(T) );    \
    writeDirect( v );       \
}

BINARY_W( uint64 )
BINARY_W( uint32 )
BINARY_W( uint16 )
BINARY_W( uint8  )
BINARY_W( int64  )
BINARY_W( int32  )
BINARY_W( int16  )
BINARY_W( int8   )
BINARY_W( bool   )
BINARY_W( float  )
BINARY_W( double )

void Binary::write( Binary const & v )
{
    write( v._buf, v._len );
}

void Binary::write( String const & v )
{
    write( v._buf, v._len );
}

void Binary::write( const char * v )
{
    write( v, (int)strlen( v ) );
}

void Binary::write( const char * v, int len )
{
    ensure( sizeof(int) + len ); writeDirect( v, len );
}

void Binary::write( DateTime const & v )
{
    write( v._value );
}






#define BINARY_R( T )                                                       \
ReadStatus Binary::read( T & v )                                            \
{                                                                           \
    if( getFreeReadSpace() < (int)sizeof(T) ) return ReadStatus::NotEnoughData;  \
    readDirect( v );                                                        \
    return ReadStatus::Success;                                             \
}

BINARY_R( uint64 )
BINARY_R( uint32 )
BINARY_R( uint16 )
BINARY_R( uint8  )
BINARY_R( int64  )
BINARY_R( int32  )
BINARY_R( int16  )
BINARY_R( int8   )
BINARY_R( bool   )
BINARY_R( float  )
BINARY_R( double )

ReadStatus Binary::read( Binary & v )
{
    if( getFreeReadSpace() < (int)sizeof(int) ) return ReadStatus::NotEnoughData;
    int len = 0;
    BinaryHelper::read( len, _buf, _readIdx );
    if( getFreeReadSpace() < len ) return ReadStatus::NotEnoughData;
    v.clear( len );
    memcpy( v._buf, _buf + _readIdx, len );
    _readIdx += len;
    return ReadStatus::Success;
}

ReadStatus Binary::read( String & v )
{
    if( getFreeReadSpace() < (int)sizeof(int) ) return ReadStatus::NotEnoughData;
    int len = 0;
    BinaryHelper::read( len, _buf, _readIdx );
    if( getFreeReadSpace() < len ) return ReadStatus::NotEnoughData;
    v.assign( _buf + _readIdx, len );
    _readIdx += len;
    return ReadStatus::Success;
}

ReadStatus Binary::read( DateTime & v )
{
    return read( v._value );
}



















#define BINARY_DW_VECTOR( T )                               \
void Binary::writeDirect( List<T> const & vs )              \
{                                                           \
    writeDirect( vs._len );                                 \
    memcpy( _buf + _len, vs._buf, vs._len * sizeof(T) );    \
    _len += (int)sizeof(T) * vs._len;                       \
}

BINARY_DW_VECTOR( uint64 )
BINARY_DW_VECTOR( uint32 )
BINARY_DW_VECTOR( uint16 )
BINARY_DW_VECTOR( uint8  )
BINARY_DW_VECTOR( int64  )
BINARY_DW_VECTOR( int32  )
BINARY_DW_VECTOR( int16  )
BINARY_DW_VECTOR( int8   )
BINARY_DW_VECTOR( float  )
BINARY_DW_VECTOR( double )
BINARY_DW_VECTOR( DateTime )

void Binary::writeDirect( List<bool> const & vs )
{
    writeDirect( vs._len );
    int size = ( vs._len - 1 ) / 8 + 1;
    memcpy( _buf + _len, vs._buf, size );
    _len += size;
    _buf[ _len - 1 ] &= (uint8)~( (uint8)0xFF << ( vs._len % 8 ) );
}






#define BINARY_DR_VECTOR( T )                   \
void Binary::readDirect( List<T> & vs )         \
{                                               \
    vs.clear();                                 \
    int len = 0;                                \
    readDirect( len );                          \
    vs.ensure( len );                           \
    int size = len * (int)sizeof(T);            \
    memcpy( vs._buf, _buf + _readIdx, size );   \
    vs._len = len;                              \
    _readIdx += size;                           \
}

BINARY_DR_VECTOR( uint64 )
BINARY_DR_VECTOR( uint32 )
BINARY_DR_VECTOR( uint16 )
BINARY_DR_VECTOR( uint8  )
BINARY_DR_VECTOR( int64  )
BINARY_DR_VECTOR( int32  )
BINARY_DR_VECTOR( int16  )
BINARY_DR_VECTOR( int8   )
BINARY_DR_VECTOR( float  )
BINARY_DR_VECTOR( double )
BINARY_DR_VECTOR( DateTime )

void Binary::readDirect( List<bool> & vs )         
{                                       
    vs.clear();
    int len = 0;
    readDirect( len );
    vs.ensure( len );
    int size = ( len - 1 ) / 8 + 1;
    memcpy( vs._buf, _buf + _readIdx, size );
    vs._len = len;
    _readIdx += size;
}






#define BINARY_W_VECTOR( T )                            \
void Binary::write( List<T> const & vs )                \
{                                                       \
    int size = int(sizeof(int) + sizeof(T) * vs._len);  \
    ensure( size );                                     \
    writeDirect( vs._len );                             \
    memcpy( _buf + _len, vs._buf, size );               \
    _len += size;                                       \
}

BINARY_W_VECTOR( uint64 )
BINARY_W_VECTOR( uint32 )
BINARY_W_VECTOR( uint16 )
BINARY_W_VECTOR( uint8  )
BINARY_W_VECTOR( int64  )
BINARY_W_VECTOR( int32  )
BINARY_W_VECTOR( int16  )
BINARY_W_VECTOR( int8   )
BINARY_W_VECTOR( float  )
BINARY_W_VECTOR( double )
BINARY_W_VECTOR( DateTime )

void Binary::write( List<bool> const & vs )
{
    int size = (int)sizeof(int) + ( vs._len - 1 ) / 8 + 1;
    ensure( size );
    writeDirect( vs._len );
    memcpy( _buf + _len, vs._buf, size );
    _len += size;
    _buf[ _len - 1 ] &= (uint8)~( (uint8)0xFF << ( vs._len % 8 ) );
}






#define BINARY_R_VECTOR( T )                                                    \
ReadStatus Binary::read( List<T> & vs )                                         \
{                                                                               \
    vs.clear();                                                                 \
    if( getFreeReadSpace() < (int)sizeof(int) ) return ReadStatus::NotEnoughData;    \
    int len = 0;                                                                \
    readDirect( len );                                                          \
    int size = len * (int)sizeof(T);                                            \
    if( getFreeReadSpace() < size ) return ReadStatus::NotEnoughData;           \
    vs.ensure( len );                                                           \
    memcpy( vs._buf, _buf + _readIdx, size );                                   \
    vs._len = len;                                                              \
    _readIdx += size;                                                           \
    return ReadStatus::Success;                                                 \
}

BINARY_R_VECTOR( uint64 )
BINARY_R_VECTOR( uint32 )
BINARY_R_VECTOR( uint16 )
BINARY_R_VECTOR( uint8  )
BINARY_R_VECTOR( int64  )
BINARY_R_VECTOR( int32  )
BINARY_R_VECTOR( int16  )
BINARY_R_VECTOR( int8   )
BINARY_R_VECTOR( float  )
BINARY_R_VECTOR( double )
BINARY_R_VECTOR( DateTime )

ReadStatus Binary::read( List<bool> & vs )                          
{
    vs.clear();
    if( getFreeReadSpace() < (int)sizeof(int) ) return ReadStatus::NotEnoughData;
    int len = 0;
    readDirect( len );
    int size = ( len - 1 ) / 8 + 1;
    if( getFreeReadSpace() < size ) return ReadStatus::NotEnoughData;
    vs.ensure( len );
    memcpy( vs._buf, _buf + _readIdx, size );
    vs._len = len;
    _readIdx += size;
    return ReadStatus::Success;
}








void Binary::bytesW( Bytes & b ) const
{
    b.write( _buf, _len );
}

ReadStatus Binary::bytesR( Bytes & b )
{
    clear();
    uint len;
    auto rtv = b >> len;
    if( rtv != ReadStatus::Success ) return rtv;
    ensure( len );
    memcpy( _buf, b._buf + b._readIdx, len );
    _len = len;
    b._readIdx += len;
    return ReadStatus::Success;
}

