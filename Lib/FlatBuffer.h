#ifndef _FLATBUFFER_H__
#define _FLATBUFFER_H__


// todo: Pool, read
class FlatBuffer
{
public:
    explicit FlatBuffer( int capacity = 1024 );                                 // prepare
    FlatBuffer( Pool& p );                                                      // prepare with pool buffer
    FlatBuffer( char* buf, int bufLen, int dataLen = 0, bool isRef = false );   // copy or ref
    FlatBuffer( FlatBuffer const& other );                                      // copy
    FlatBuffer( FlatBuffer&& other );                                           // move
    FlatBuffer& operator=( FlatBuffer const& other );                           // copy
    FlatBuffer& operator=( FlatBuffer&& other );                                // move
    ~FlatBuffer();
    void assign( char const* buf, int bufLen, int dataLen = 0, bool isRef = false );  // copy or ref
    void reserve( int capacity );
    void resize( int widx );
    void clear();                                                               // _dataLen = _offset = 0
    int size() const;                                                           // _dataLen
    int& size();                                                                // _dataLen
    int offset() const;                                                         // _offset
    int& offset();                                                              // _offset
    bool empty() const;                                                         // _dataLen == 0
    bool eof() const;                                                           // _dataLen == _offset
    char const* data() const;                                                   // _buf
    char* data();                                                               // _buf
    char operator[] ( int idx ) const;                                          // _buf[ idx ]
    char& operator[] ( int idx );                                               // _buf[ idx ]
    char at( int idx ) const;                                                   // _buf[ idx ]
    char& at( int idx );                                                        // _buf[ idx ]



    // for FlatBuffer write
    int getBufferSize();
    void writeBuffer( FlatBuffer& fb );
    void writeBufferDirect( FlatBuffer& fb );



    // do not reserve
    void writeDirect( char const* buf, int dataLen );                           // do not write dataLen
    template<int len>
    void writeDirect( char const( &s )[ len ] );                                // same as String( len + buf )
    template<typename T>
    void writeDirect( T const& v );

    // reserve
    void write( char const* buf, int dataLen );
    template<int len>
    void write( char const( &s )[ len ] );
    template<typename T>
    void write( T const& v );

    // reserve once
    template<typename ...TS>
    void writes( TS const& ...vs );


    //template<typename T>
    //bool read( T& v )
    //{
    //    auto siz = BufferUtils::getSize( v );
    //    if( _offset + siz > _dataLen ) return false;
    //    if( Utils::isValueType<T>() )
    //    {
    //        BufferUtils::read( v, _buf );
    //        return true;
    //    }
    //    // todo
    //}

    //inline bool read( char* buf, int dataLen )
    //{
    //    if( _offset + dataLen > _dataLen ) return false;
    //    memcpy( buf, _buf + _offset, dataLen );
    //    _offset += dataLen;
    //    return true;
    //}



    //template<typename T>
    //bool FlatBuffer::read( T& v )
    //{
    //    auto len = Utils::getSize( v );
    //    if( _offset + len > _dataLen ) return false;
    //    Utils::binaryRead( v, _buf + _offset );
    //    _offset += len;
    //    return true;
    //}





    //inline int binaryRead( String& dest, char const* src, int srcLen )
    //{
    //    int destLen;
    //    int readLen = binaryRead( destLen, src, srcLen );
    //    if( readLen <= 0 ) return readLen;
    //    srcLen -= readLen;
    //    src += readLen;
    //    if( destLen > srcLen ) return srcLen - destLen;
    //    dest.resize( destLen, false );
    //    memcpy( dest.data(), src + sizeof( int ), destLen );
    //    return destLen + readLen;
    //}
    //
    //inline bool binaryRead( FlatBuffer& dest, char const* src, int srcLen )
    //{
    //    int destLen;
    //    if( !binaryRead( destLen, src, srcLen ) ) return false;
    //    dest.assign( src + sizeof( int ), 0, destLen );
    //    return true;
    //}
    //template<typename T>
    //bool binaryRead( List<T> const& dest, char const* src, int srcLen )
    //{
    //    int destLen;
    //    if( !binaryRead( destLen, src, srcLen ) ) return false;
    //    if( isValueType<T>() )
    //    {
    //        auto siz = destLen * sizeof( T );
    //        if( siz > srcLen - sizeof( T ) ) return false;
    //        dest.resize( destLen, false );
    //        memcpy( dest.data(), src + sizeof( int ), siz );
    //        return true;
    //    }
    //    else
    //    {
    //        dest.clear();
    //        dest.reserve( destLen );
    //        dest.size() = 0;
    //        src += sizeof( int );
    //        srcLen -= sizeof( int );
    //        for( int i = 0; i < destLen; ++i )
    //        {
    //            new ( &dest[ i ] ) T();
    //            dest.size() = i + 1;
    //            if( !binaryRead( vs[ i ], src + sizeof( int ), srcLen - sizeof( int ) ) ) return false;
    //        }
    //        return true;
    //    }
    //
    //}
    //




    // todo: writeVar  readVar support

    String dump();

private:
    typedef void ( FlatBuffer::*Disposer )( );
    void disposePoolBuffer();
    void disposeNewBuffer();

    template<typename T>
    void writesCore( T const& v );
    template<typename T, typename ...TS>
    void writesCore( T const& v, TS const& ...vs );


    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    int         _offset;
    Disposer    _disposer;
};


#endif
