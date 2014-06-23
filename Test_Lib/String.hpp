template<size_t len>
String::String( char( &buffer )[ len ] )
    : _buffer( buffer )
    , _bufferLength( len )
    , _dataLength( 0 )
    , _disposer( nullptr )
{
}

template<size_t len>
String::String( char const( &s )[ len ] )
    : String( len )
{
    memcpy( _buffer, s, len );
    _dataLength = len - 1;
}

template<typename ...TS>
void String::append( TS const & ...vs )
{
    auto maxLen = Utils::getFillMaxLength( vs... );
    if( _dataLength + maxLen > _bufferLength )
        reserve( (int)Utils::round2n( _dataLength + maxLen ) );
    _dataLength += Utils::fill( _buffer + _dataLength, vs... );
}
