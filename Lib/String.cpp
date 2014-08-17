#include "All.h"

Pool String::_emptyPool;

String::String( int capacity /*= 64 */ )
{
    if( capacity < 64 ) _bufLen = 64;
    else _bufLen = (int)Utils::round2n( capacity );
    _disposer = nullptr;
    _dataLen = 0;
    _buf = new char[ _bufLen ];
    _buf[ 0 ] = '\0';
}

String::String( Pool& p )
{
    assert( p.attachThis() && p.itemBufLen() > sizeof( Pool* ) );
    _buf = (char*)p.alloc();
    _bufLen = p.itemBufLen() - sizeof( Pool* );
    _dataLen = 0;
    _disposer = &p;
    _buf[ 0 ] = '\0';
}

String::String( Pool& p, char const* buf, int dataLen )
{
    assert( p.attachThis() && p.itemBufLen() > ( int )sizeof( Pool* ) && p.itemBufLen() - ( int )sizeof( Pool* ) >= dataLen + 1 );
    _buf = (char*)p.alloc();
    _bufLen = p.itemBufLen() - sizeof( Pool* );
    _dataLen = dataLen;
    _disposer = &p;
    memcpy( _buf, buf, dataLen );
    _buf[ dataLen ] = '\0';
}

String::String( char const* buf, int bufLen, int dataLen, bool isRef )
{
    _dataLen = dataLen;
    if( isRef )
    {
        _buf = (char*)buf;
        _bufLen = bufLen;
        _disposer = &_emptyPool;
    }
    else
    {
        _bufLen = (int)Utils::round2n( dataLen + 1 );
        _buf = new char[ _bufLen ];
        _disposer = nullptr;
        memcpy( _buf, buf, dataLen );
        _buf[ _dataLen ] = '\0';
    }
}

String::String( char const* s, bool isRef )
{
    _dataLen = (int)strlen( s );
    if( isRef )
    {
        _buf = (char*)s;
        _bufLen = _dataLen + 1;
        _disposer = &_emptyPool;
    }
    else
    {
        _bufLen = (int)Utils::round2n( _dataLen + 1 );
        _buf = new char[ _bufLen ];
        _disposer = nullptr;
        memcpy( _buf, s, _dataLen + 1 );
    }
}

String::String( String const & other )
    : String( other._dataLen + 1 )
{
    memcpy( _buf, other._buf, other._dataLen + 1 );
    _dataLen = other._dataLen;
}

String::String( String&& other )
    : _buf( other._buf )
    , _bufLen( other._bufLen )
    , _dataLen( other._dataLen )
    , _disposer( other._disposer )
{
    other._buf = nullptr;
    other._bufLen = 0;
    other._dataLen = 0;
    other._disposer = nullptr;
}


String& String::operator=( String const& other )
{
    if( this == &other ) return *this;
    if( _bufLen > other._dataLen )
    {
        _dataLen = other._dataLen;
        memcpy( _buf, other._buf, other._dataLen + 1 );
    }
    else
    {
        dispose();
        _bufLen = (int)Utils::round2n( other._dataLen + 1 );
        _disposer = nullptr;
        _buf = new char[ _bufLen ];
        _dataLen = other._dataLen;
        memcpy( _buf, other._buf, other._dataLen + 1 );
    }
    return *this;
}

String& String::operator=( String && other )
{
    dispose();
    _buf = other._buf;
    _bufLen = other._bufLen;
    _dataLen = other._dataLen;
    _disposer = other._disposer;
    other._buf = nullptr;
    other._bufLen = 0;
    other._dataLen = 0;
    other._disposer = nullptr;
    return *this;
}


String::~String()
{
    dispose();
}

void String::assign( char const* buf, int bufLen, int dataLen /*= 0*/, bool isRef /*= true */ )
{
    assert( _buf != buf );
    if( isRef )
    {
        dispose();
        _buf = (char*)buf;
        _bufLen = bufLen;
        _dataLen = dataLen;
        _disposer = nullptr;
        return;
    }
    if( _bufLen <= dataLen )
    {
        dispose();
        _bufLen = (int)Utils::round2n( dataLen + 1 );
        _disposer = nullptr;
        _buf = new char[ _bufLen ];
    }
    _dataLen = dataLen;
    memcpy( _buf, buf, dataLen );
    _buf[ _dataLen ] = '\0';
}

void String::assign( char const* s, bool isRef /*= false */ )
{
    auto dataLen = (int)strlen( s );
    assign( s, dataLen + 1, dataLen, isRef );
}

void String::reserve( int capacity )
{
    if( capacity + 1 <= _bufLen ) return;
    _bufLen = (int)Utils::round2n( capacity + 1 );
    auto newBuf = new char[ _bufLen ];
    memcpy( newBuf, _buf, _dataLen + 1 );
    dispose();
    _disposer = nullptr;
    _buf = newBuf;
}

void String::resize( int len, bool fillZero /*= true */ )
{
    if( len == _dataLen ) return;
    else if( len < _dataLen )
    {
        _dataLen = len;
        _buf[ len ] = '\0';
    }
    else
    {
        reserve( len );
        if( fillZero )
            memset( _buf + _dataLen, 0, len - _dataLen + 1 );
        _dataLen = len;
    }
}

void String::clear()
{
    _dataLen = 0;
    _buf[ 0 ] = '\0';
}

char const* String::c_str() const
{
    return _buf;
}
char* String::data() const
{
    return _buf;
}

char String::operator[]( int idx ) const
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}
char& String::operator[]( int idx )
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

char String::at( int idx ) const
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

char& String::at( int idx )
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

void String::push( char c )
{
    if( _dataLen == _bufLen ) reserve( _dataLen + 1 );
    _buf[ _dataLen ] = c;
    _buf[ _dataLen + 1 ] = '\0';
    ++_dataLen;
}

int String::size() const
{
    return _dataLen;
}

void String::dispose()
{
    if( _disposer )
    {
        if( _disposer->itemBufLen() ) _disposer->free( _buf );
    }
    else
    {
        delete[] _buf;
    }
}




static byte const lowerchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
    117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
    157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
    177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
    197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216,
    217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};
static byte const upperchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125,
    126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
    146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
    166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
    186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
    206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
    226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
    246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

void String::toLower()
{
    for( int i = 0; i < _dataLen; ++i ) _buf[ i ] = lowerchars[ _buf[ i ] ];
}

void String::toUpper()
{
    for( int i = 0; i < _dataLen; ++i ) _buf[ i ] = upperchars[ _buf[ i ] ];
}

void String::toLowerUnsafe()
{
    int i = 0;
    int mod = (size_t)_buf & ( sizeof( size_t ) - 1 );
    if( mod )
    {
        if( mod > _dataLen ) mod = _dataLen;
        for( ; i < mod; ++i ) _buf[ i ] |= 0x20;
    }
    mod = ( _dataLen - mod ) % sizeof( size_t );
    for( ; i < _dataLen - mod; i += sizeof( size_t ) )
    {
        *(size_t*)( _buf + i ) |= size_t( 0x2020202020202020 );
    }
    for( ; i < _dataLen; ++i ) _buf[ i ] |= 0x20;
}

bool String::operator==( String const& other ) const
{
    if( this == &other ) return true;
    if( _dataLen != other._dataLen ) return false;
    return memcmp( _buf, other._buf, _dataLen ) == 0;
}

bool String::operator!=( String const& other ) const
{
    return !operator==( other );
}

bool String::operator<( String const& other ) const
{
    if( this == &other ) return false;
    auto r = memcmp( _buf, other._buf, MIN( _dataLen, other._dataLen ) );
    if( r == 0 ) return _dataLen < other._dataLen;
    return r < 0;
}

bool String::operator>( String const& other ) const
{
    if( this == &other ) return false;
    auto r = memcmp( _buf, other._buf, MIN( _dataLen, other._dataLen ) );
    if( r == 0 ) return _dataLen > other._dataLen;
    return r > 0;
}

bool String::operator<=( String const& other ) const
{
    return !operator>( other );
}

bool String::operator>=( String const& other ) const
{
    return !operator<( other );
}

int String::getHashCode() const
{
#ifdef __IA
    return Utils::getHash_CS( (byte const*)_buf, _dataLen );
#else
    if( _dataLen >= 4 && ( (size_t)_buf % sizeof( size_t ) == 0 ) )
        return Utils::getHash_CS( (byte const*)_buf, _dataLen );
    else
        return Utils::getHash_Lua( (byte const*)_buf, _dataLen );
#endif
}

std::string String::std_str()
{
    return std::string( _buf, _dataLen );
}




int String::getWriteBufferSize() const
{
    return sizeof( int ) + size();
}

void String::writeBuffer( FlatBuffer& fb ) const
{
    fb.write( _dataLen );
    fb.write( _buf, _dataLen );
}
void String::writeBufferDirect( FlatBuffer& fb ) const
{
    fb.writeDirect( _dataLen );
    fb.writeDirect( _buf, _dataLen );
}

bool String::readBuffer( FlatBuffer& fb )
{
    int len;
    if( !fb.read( len ) || len < 0
        || fb.offset() + len > fb.size() ) return false;
    assign( fb.data() + fb.offset(), 0, len, false );
    fb.offset() += len;
    return true;
}

void String::pop()
{
    assert( _dataLen > 0 );
    --_dataLen;
}

char& String::top()
{
    assert( _dataLen > 0 );
    return _buf[ _dataLen - 1 ];
}

char const& String::top() const
{
    assert( _dataLen > 0 );
    return _buf[ _dataLen - 1 ];
}
