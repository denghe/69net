template<typename T>
void FlatBuffer::write( T const& v )
{
    reserve( _dataLen + Utils::getSize( v ) );
    Utils::binaryWrite( _buf + _dataLen, v );
    _dataLen += Utils::getSize( v );
}

template<typename T>
void writesCore( char* buf, int& offset, T const& v )
{
    Utils::binaryWrite( buf + offset, v );
    offset += Utils::getSize( v );
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
    reserve( _dataLen + Utils::getSize( vs... ) );
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
