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
    bool readBuffer( FlatBuffer& fb );




    // do not reserve
    void writeDirect( char const* buf, int dataLen );                           // do not write dataLen
    template<int len>
    void writeDirect( char const( &s )[ len ] );                                // same as String( len + buf )
    template<typename T>
    void writeDirect( T const& v );
    template<typename ...TS>
    void writesDirect( TS const& ...vs );

    // reserve
    void write( char const* buf, int dataLen );
    template<int len>
    void write( char const( &s )[ len ] );
    template<typename T>
    void write( T const& v );

    // reserve once
    template<typename ...TS>
    void writes( TS const& ...vs );



    // return true: success
    template<typename T>
    bool read( T& v );
    bool read( char* buf, int dataLen );
    template<typename ...TS>
    bool reads( TS& ...vs );



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

    template<typename T>
    bool readsCore( T& v );
    template<typename T, typename ...TS>
    bool readsCore( T& v, TS& ...vs );



    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    int         _offset;
    Disposer    _disposer;
};


#endif
