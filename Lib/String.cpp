#include "All.h"

// todo: 做个池？ 注意替换每个第 1 次 new 的位置
// todo: if capacity <= pool.item.buf.size 则从 pool 分配, _disposer 指向 disposePoolBuffer
String::String( int capacity /*= 64 */ )
{
    if( capacity < 64 ) _bufLen = 64;
    else _bufLen = (int)Utils::round2n( capacity );
    _disposer = &String::disposeNewBuffer;
    _dataLen = 0;
#if __DEBUG_STRING
    std::cout << "String::String( int capacity ) new char[ " << _bufLen << " ]\n";
#endif
    _buf = new char[ _bufLen ];
    _buf[ 0 ] = '\0';
}

String::String( char* buf, int bufLen, int dataLen )
    : _buf( buf )
    , _bufLen( bufLen )
    , _dataLen( dataLen )
    , _disposer( nullptr )
{
}

String::String( char const* s, bool isRef )
{
    _dataLen = (int)strlen( s );
    if( isRef )
    {
        _buf = (char*)s;
        _bufLen = _dataLen + 1;
        _disposer = nullptr;
    }
    else
    {
        _bufLen = (int)Utils::round2n( _dataLen + 1 );
        _buf = new char[ _bufLen ];
        memcpy( _buf, s, _dataLen + 1 );
        _disposer = &String::disposeNewBuffer;
    }
}

String::String( String const & other )
    : String( other._dataLen + 1 )
{
    memcpy( _buf, other._buf, other._dataLen + 1 );
    _dataLen = other._dataLen;
}

String::String( String && other )
    : _buf( other._buf )
    , _bufLen( other._bufLen )
    , _dataLen( other._dataLen )
    , _disposer( other._disposer )
{
    //other._buf = nullptr;  // 这三行不必要
    //other._bufLen = 0;
    //other._dataLen = 0;
    other._disposer = nullptr;
}

String::~String()
{
    if( _disposer ) ( this->*_disposer )( );
}

void String::reserve( int len )
{
    if( len + 1 <= _bufLen ) return;
    len = (int)Utils::round2n( len + 1 );
    _bufLen = len;
#if __DEBUG_STRING
    std::cout << "void String::reserve( int len ) new char[ " << len << " ]\n";
#endif
    auto newBuffer = new char[ len ];
    memcpy( newBuffer, _buf, _dataLen + 1 );
    if( _disposer ) ( this->*_disposer )( );
    _disposer = &String::disposeNewBuffer;
    _buf = newBuffer;
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

char* String::c_str() const
{
    return _buf;
}
char* String::c_str()
{
    return _buf;
}

String::operator char*( ) const
{
    return _buf;
}
String::operator char*( )
{
    return _buf;
}

char& String::operator[]( int idx ) const
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}
char& String::operator[]( int idx )
{
    assert( idx >= 0 && idx < _dataLen );
    return _buf[ idx ];
}

int String::size() const
{
    return _dataLen;
}

void String::disposeIncommingBuffer() {}

void String::disposePoolBuffer()
{
    // todo
}

void String::disposeNewBuffer()
{
#if __DEBUG_STRING
    std::cout << "delete[] _buf\n";
#endif
    delete[] _buf;
}



String& String::operator=( String const & other )
{
    if( this == &other ) return *this;
    if( _bufLen > other._dataLen )
    {
        _dataLen = other._dataLen;
        memcpy( _buf, other._buf, other._dataLen + 1 );
    }
    else
    {
        if( _disposer ) ( this->*_disposer )( );
        _bufLen = (int)Utils::round2n( other._dataLen + 1 );
        _disposer = &String::disposeNewBuffer;
#if __DEBUG_STRING
        std::cout << "String& String::operator=( String const & other ) new char[ " << _bufLen << " ]\n";
#endif
        _buf = new char[ _bufLen ];
        _dataLen = other._dataLen;
        memcpy( _buf, other._buf, other._dataLen + 1 );
    }
    return *this;
}

String& String::operator=( String && other )
{
    if( _disposer ) ( this->*_disposer )( );
    _buf = other._buf;
    _bufLen = other._bufLen;
    _dataLen = other._dataLen;
    _disposer = other._disposer;
    //other._buf = nullptr;  // 这三行不必要
    //other._bufLen = 0;
    //other._dataLen = 0;
    other._disposer = nullptr;
    return *this;
}


static byte const lowerchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};
static byte const upperchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
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

bool String::operator==( String const& other )
{
    if( this == &other ) return true;
    if( _dataLen != other._dataLen ) return false;
    return memcmp( _buf, other._buf, _dataLen ) == 0;
}

bool String::operator!=( String const& other )
{
    return !operator==( other );
}

bool String::operator<( String const& other )
{
    if( this == &other ) return false;
    auto r = memcmp( _buf, other._buf, std::min( _dataLen, other._dataLen ) );
    if( r == 0 ) return _dataLen < other._dataLen;
    return r < 0;
}

bool String::operator>( String const& other )
{
    if( this == &other ) return false;
    auto r = memcmp( _buf, other._buf, std::min( _dataLen, other._dataLen ) );
    if( r == 0 ) return _dataLen > other._dataLen;
    return r > 0;
}

bool String::operator<=( String const& other )
{
    return !operator>( other );
}

bool String::operator>=( String const& other )
{
    return !operator<( other );
}


int String::getHash_CS()
{
    assert( (size_t)_buf % 4 == 0 );
    if( !_dataLen ) return 0;
    int n1 = 0x15051505, n2 = n1, mod = _dataLen % 8, i = 0;
    int64 n = 0;
    auto np = (int*)&n;
    for( ; i < _dataLen - mod; i += 8 )
    {
        n = *(uint64*)( _buf + i );
        n1 = ( ( ( n1 << 5 ) + n1 ) + ( n1 >> 0x1b ) ) ^ np[ 1 ];
        n2 = ( ( ( n2 << 5 ) + n2 ) + ( n2 >> 0x1b ) ) ^ np[ 0 ];
    }
    if( mod )
    {
        n = *(uint64*)( _buf + i );
        n &= size_t( 0xFFFFFFFFFFFFFFFF ) >> ( ( 8 - mod ) * 8 );
        n1 = ( ( ( n1 << 5 ) + n1 ) + ( n1 >> 0x1b ) ) ^ np[ 1 ];
        if( np[ 0 ] ) n2 = ( ( ( n2 << 5 ) + n2 ) + ( n2 >> 0x1b ) ) ^ np[ 0 ];
    }
    return n1 + n2 * 0x5d588b65;
}

int String::getHash_Java()
{
    if( !_dataLen ) return 0;
    int hash = 0;
    for( int i = 0; i < _dataLen; i++ )
        hash = ( 31 * hash ) + _buf[ i ];
    return hash;
}

int String::getHash_Lua()
{
    if( !_dataLen ) return 0;
    uint seed = 131, hash = 0;
    for( int i = 0; i < _dataLen; ++i )
        hash = hash * seed + (uint8)_buf[ i ];
    return (int)hash;
}
