#ifndef _LIST_HH_
#define _LIST_HH_

const static int ListDefaultBufferSize = 512;  // 这指的是字节数

template<typename T>
struct ListBase
{
    int _len;           // 已放入的 T 的个数
    int _bufSize;       // _buf 的字节长
    T * _buf;
};

template<typename T>
struct List : public ListBase<T> {
    List();
    explicit List( int capacity );
    List( List const & o );
    List( List && o );
    ~List();
    void                clear();
    int                 getFreeSpace();
    void                ensure( int len );
    template<typename TV>
    void                add( TV && v );             // add 前会 ensure( 1 )
    template<typename TV>
    void                addDirect( TV && v );       // 直接 add, 不 ensure
    template<typename TV>
    List &              operator<<( TV && v );
    void                set( int idx, T const & v );
    T const &           get( int idx ) const;
    T &                 operator[]( int idx );
    T const &           operator[]( int idx ) const;
    void                toString( String & s ) const;
};

template<>                                           
struct List<bool> : public ListBase<uint8> {        // bool 特化按位存
    typedef List<bool> TT;
    List();                                          
    List( int capacity );                            
    List( TT const & o );
    List( TT && o );
    ~List();                                         
    void                clear();                     
    int                 getFreeSpace();              
    void                ensure( int len );           
    void                add( bool v );
    void                addDirect( bool v );
    TT &                operator<<( bool v );
    void                set( int idx, bool v );
    bool                get( int idx ) const;
    bool                operator[]( int idx ) const;
    void                toString( String & s ) const;
};


template<>
inline void List<String>::ensure( int len )               // string 的 ensure 操作直接 memcpy 即可
{
    if( getFreeSpace() < len )
    {
        _bufSize = (int)round2n( ( _len + len ) * sizeof(String) );
        _buf = (decltype(_buf))realloc( _buf, _bufSize );
    }
}

template<>
inline void List<String>::toString( String & s ) const    // string 元素 toString 要加引号
{
    s << "{ ";
    for( int i = 0; i < _len; ++i )
    {
        if( i ) s << ", ";
        s << '"' << (*this)[ i ] << '"';
    }
    s << " }";
}

#endif

