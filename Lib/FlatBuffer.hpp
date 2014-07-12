
void binaryWrite( char* dest, String const & src );
void binaryWrite( char* dest, FlatBuffer const & src );

int binaryRead( String& dest, char const* src, int len );
int binaryRead( FlatBuffer& dest, char const* src, int len );





template<typename T>
int getSizeCore( T const& v ) { return sizeof( T ); }
inline int getSizeCore( String const& v ) { return v.size() + sizeof( int ); };
inline int getSizeCore( FlatBuffer const& v ) { return v.size() + sizeof( int ); };

template<typename T>
void getSizeCore( int& len, T const& v )
{
    len += getSizeCore( v );
}

template<typename T, typename ...TS>
void getSizeCore( int& len, T const & v, TS const & ...vs )
{
    getSizeCore( len, v );
    getSizeCore( len, vs... );
}

template<typename ...TS>
int getSize( TS const & ...vs )         // 批量统计 binary 长度
{
    int len = 0;
    getSize( len, vs... );
    return len;
}


template<typename T>
void FlatBuffer::write( T const& v )
{
    reserve( _dataLen + Utils::getSize( v ) );
    Utils::binaryWrite( _buf + _dataLen, v );
    _dataLen += sizeof( v );
}

template<typename T>
void writesCore( char* buf, int& offset, T const& v )
{
    Utils::binaryWrite( buf + offset, v );
    offset += sizeof( v );
}

template<typename T, typename ...TS>
void writesCore( char* buf, int& offset, T const& v, TS const & ...vs )
{
    writesCore( buf, offset, v );
    writesCore( buf, offset, vs... );
}

template<typename ...TS>
void FlatBuffer::writes( TS const & ...vs )
{
    reserve( _dataLen + Utils::getSize( vs ) );
    writesCore( _buf, _dataLen, vs... );
}


template<typename T>
bool FlatBuffer::read( T& v )
{
    auto len = Utils::getSize( v );
    if( _offset + len > _dataLen ) return false;
    Utils::binaryRead( v, _buf + _offset );
    _offset += len;
    return true;
}
