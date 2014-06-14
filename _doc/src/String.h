#ifndef _STRING_HH_
#define _STRING_HH_

const int DefaultStringBufferSize = 32;

struct String
{
    int _len;            // 数据长度
    int _bufSize;        // 容器长度
    char * _buf;         // 容器

    static String const defaultValue;

    explicit String( int bufSize = DefaultStringBufferSize );
    String( String const & o );
    String( String && o );

    String( char const * s );           // 这几个构造会将 _bufSize 限制在接近 s/len 的长度。这意味着如果后面要拼接字串的话，该构造可能影响执行效率
    template<int len>
    String( char const (&s)[ len ] );
    String( char const * buf, int len );

    String( wchar const * s );           // wchar 版, -> utf8
    template<int len>
    String( wchar const (&s)[ len ] );
    String( wchar const * buf, int len );

    ~String();
    String & operator=( String const & o );
    String & operator=( String && o );

    template<typename T>
    String & operator=( T const & v );

    template<typename T>
    String & operator+=( T const & v );

    template<typename T>
    String & operator+( T const & v );

    bool operator==( String const & s ) const;
    bool operator!=( String const & s ) const;
    bool operator<( String const & s ) const;
    bool operator>( String const & s ) const;
    bool operator<=( String const & s ) const;
    bool operator>=( String const & s ) const;



    template<int len>
    void        assign( char const (&s)[ len ] );       // 覆盖 s 串的内容到体内
    void        assign( char const * s );               // 覆盖 s 串的内容到体内
    void        assign( String && s );                  // 覆盖/移动 s 串的内容到体内
    void        assign( char const * buf, int len );    // 覆盖 buf len 长的内容到体内
    template<typename T>
    void        assign( T const & v );                  // 覆盖 v 的字串内容到体内

    void        clear();                                // 清 _len ( _len = 0 )
    void        clear( int bufSize );                   // 清 _len 并确保 bufSize 足够长（不会减肥）

    void        grow( int bufSize );                    // 扩容, _len 不变. 数据保留. len 按 2 的幂来对齐
    void        ensure( int len );                      // 确保 getFreeBufferLen() >= len ( 可能会触发扩容操作 )

    int         getFreeBufferLen() const;               // 返回可写区间字节数 ( _bufSize - _len )

    char const* c_str();                                // 在末尾填一个 '\0' 并返回 _buf

    // write
    void        write( uint64 v );
    void        write( uint32 v );
    void        write( uint16 v );
    void        write( uint8  v );
    void        write( int64  v );
    void        write( int32  v );
    void        write( int16  v );
    void        write( int8   v );
    void        write( float  v );
    void        write( double v );
    void        write( bool   v );
    void        write( char const * v, int len );
    void        write( char const * v );
//    void        write( char * v );
    void        write( char v );    // 单个字符
    void        write( String const & v );

    void        write( wchar const * v, int len );  // wchar 版
    void        write( wchar const * v );
//    void        write( wchar * v );
    void        write( wchar v );

    template<typename T>
    void        write( T const & v );

    // write hex
    void        writeHex( uint8  v );
    void        writeHex( uint16 v );
    void        writeHex( uint32 v );
    void        writeHex( uint64 v );

    template<typename T>
    String & operator<<( T const & v );

    // 扩展部分，令 String 支持扩展数据类型的操作
    /*

    扩展类型需要实现下列函数：

    void toString( String & s ) const;

    */

};



#endif
