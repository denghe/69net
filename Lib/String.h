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

    template<typename ...TS>
    void append( TS const & ...vs );

    //template<typename ...TS>
    //void appendFormat( char const* format, TS const & ... vs ); // {0}  {1}  {2} .....




    template<typename T>
    void appendFormatCore( List<String>& ss, int& i, T const & v )
    {
        ss[ i ].append( v );
    }

    template<typename T, typename ...TS>
    void appendFormatCore( List<String>& ss, int& i, T const & v, TS const & ...vs )
    {
        ss[ i++ ].append( v );
        appendFormatCore( ss, i, vs... );
    }

    template<typename ...TS>
    void appendFormat( char* format, TS const & ...vs )
    {
        char buf[ sizeof...( vs ) ][ 128 ];
        List<String> ss( sizeof...( vs ) );
        for( int i = 0; i < sizeof...( vs ); ++i )
            ss.push( String( buf[ i ], 128 ) );
        int num = 0;
        appendFormatCore( ss, num, vs... );

        char numBuf[ 32 ];
        String numStr( numBuf, 32 );
        int offset = 0;
        while( auto c = format[ offset ] )
        {
            if( c == '{' )
            {
                c = format[ ++offset ];
                if( c == '{' )
                {
                    push( '{' );
                }
                else
                {
                    while( c = format[ offset ] )
                    {
                        if( c == '}' )
                        {
                            Utils::fromString( num, numBuf );
                            numStr.clear();
                            if( num < 0 || num >= sizeof...( vs ) )
                            {
                                throw std::invalid_argument( "argument out of range." );
                            }
                            append( ss[ num ] );
                            break;
                        }
                        else
                        {
                            numStr.push( c );
                        }
                        ++offset;
                    }
                }
            }
            else
            {
                push( c );
            }
            ++offset;
        }
    }





    template<typename ...TS>
    static String make( TS const & ...vs );




    int getHashCode() const;                                    // only support align of 4 _buf on some ARM cpu and unaligned buffer



    // todo: more util funcs

private:
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


#endif
