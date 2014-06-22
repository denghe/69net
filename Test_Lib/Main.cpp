#include "../Lib/All.h"

// 支持传入定长 buffer 的字串
class String
{
public:
    typedef void ( String::*Disposer )( );

    explicit String( int capacity )
        : _buffer( new char[ capacity ] )
        , _bufferLength( capacity )
        , _dataLength( 0 )
        , _disposer( &String::disposeNewBuffer )
    {
        // todo: 做个池？
        // todo: if capacity <= pool.item.buffer.size 则从 pool 分配, _disposer 指向 deletePoolBuffer
    }

    template<size_t len>
    String( char( &buffer )[ len ] )
        : _buffer( buffer )
        , _bufferLength( len )
        , _dataLength( 0 )
        , _disposer( nullptr )
    {
    }

    String( char* buffer, int len )
        : _buffer( buffer )
        , _bufferLength( len )
        , _dataLength( 0 )
        , _disposer( nullptr )
    {
    }

    ~String()
    {
        if( _disposer ) ( this->*_disposer )( );
    }

    template<typename ...TS>
    void append( TS const & ...vs )
    {
        auto maxLen = Utils::getFillMaxLength( vs... );
        if( _dataLength + maxLen > _bufferLength )
            reserve( (int)Utils::round2n( _dataLength + maxLen ) );
        _dataLength += Utils::fill( _buffer + _dataLength, vs... );
    }

    void reserve( int len )
    {
        if( len <= _bufferLength ) return;
        auto newBuffer = new char[ len ];
        memcpy( newBuffer, _buffer, _dataLength );
        if( _disposer )
            ( this->*_disposer )( );
        else
            _disposer = &String::disposeNewBuffer;
        _buffer = newBuffer;
        _bufferLength = len;
    }

    char* c_str()
    {
        return _buffer;
    }

    void disposeIncommingBuffer()
    {
    }
    void disposePoolBuffer()
    {
    }
    void disposeNewBuffer()
    {
        delete[] _buffer;
    }

private:
    char*       _buffer;
    int         _bufferLength;
    int         _dataLength;
    Disposer    _disposer;
};

int main()
{
    char buf[ 16 ];
    String s( buf );
    s.append( "asdf!" );
    std::cout << ( s.c_str() == buf ? "true" : "false" ) << "\n";
    s.append( " sizeof(String) = ", sizeof( String ) );
    std::cout << ( s.c_str() == buf ? "true" : "false" ) << "\n";
    std::cout << s.c_str() << "\n";
    return 0;
}
