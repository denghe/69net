#ifndef _STRING_H__
#define _STRING_H__

// support import fixed buffer
// support ref char const * string
// todo:
// 各种运算符重载
// 支持各种常用 字串 处理
// 内存池
class String
{
public:
    explicit String( int capacity = 64 );                       // prepare
    String( char* buf, int bufLen, int dataLen = 0 );           // ref
    String( char const* s, bool isRef = false );                // copy or ref
    String( String const & other );                             // copy
    String( String && other );                                  // move
    String& operator=( String const & other );                  // copy
    String& operator=( String && other );                       // move
    ~String();                                                  // dispose buf
    void reserve( int len );                                    // prepare memory
    void resize( int len, bool fillZero = true );               // cut( maybe damage utf8 ) or grow up
    void clear();                                               // set _dataLen to 0
    int size() const;                                           // return _dataLen;
    char* c_str() const;                                        // return _buf
    char* data();                                               // return _buf
    char& operator[] ( int idx ) const;                         // return _buf[ idx ]
    char& operator[] ( int idx );                               // return _buf[ idx ]
    char at( int idx ) const;
    //char at( int idx );
    void set( int idx, char v );
    void push( char c );
    // todo: top, pop

    bool operator==( String const& other ) const;
    bool operator!=( String const& other ) const;
    bool operator<( String const& other ) const;
    bool operator>( String const& other ) const;
    bool operator<=( String const& other ) const;
    bool operator>=( String const& other ) const;


    void toLower();
    void toUpper();
    void toLowerUnsafe();                                       // 2x faster than toLower(). bad chars：   @[\]^_` 

    int getHashCode() const;

    template<typename ...TS>
    void append( TS const & ...vs );


    template<typename ...TS>
    void appendFormat( char const* format, TS const & ...vs );


    template<typename ...TS>
    static String make( TS const & ...vs );

    template<typename ...TS>
    static String makeFormat( char const* format, TS const & ...vs );

    std::string std_str();


    // todo: more util funcs

private:
    template<typename T>
    void appendFormatCore( List<String>& ss, int& i, T const & v );

    template<typename T, typename ...TS>
    void appendFormatCore( List<String>& ss, int& i, T const & v, TS const & ...vs );

    typedef void ( String::*Disposer )( );
    void disposeIncommingBuffer();
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    Disposer    _disposer;
};

#include "String.hpp"

















// todo

class HashString
{
public:
    HashString( String const& s )
    {
        _s = s;
        _h = _s.getHashCode();
    }
    HashString( String && s )
    {
        _s = std::move( s );
        _h = _s.getHashCode();
    }
    HashString( HashString const& other )
        : _s( other._s )
        , _h( other._h )
    {
    }
    HashString( HashString && other )
        : _s( std::move( other._s ) )
        , _h( other._h )
    {
    }
    inline HashString & operator=( HashString && other )
    {
        _s = std::move( other._s );
        _h = other._h;
        return *this;
    }
    inline HashString & operator=( HashString const & other )
    {
        _s = other._s;
        _h = other._h;
        return *this;
    }
    inline bool operator==( HashString const& other ) const
    {
        return _h == other._h && _s == other._s;
    }
    //inline operator String const&( )
    //{
    //    return _s;
    //}
//private:
    String _s;
    int _h;
};







#endif
