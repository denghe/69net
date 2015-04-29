#include <iostream>
#include <string>
#include <vector>
#include <locale>
#include <assert.h>
using namespace std;
typedef unsigned char byte;
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
class FastDW
{
public:
    FastDW()
    {
    }
    FastDW( vector<wstring> const & dirtyWords )
    {
        init( dirtyWords );
    }
    ~FastDW()
    {
        clearBuffer();
    }
    inline void clearBuffer()
    {
        for( auto& buf : _bufs ) delete[] buf;
        _bufs.clear();
    }
    inline void init( vector<wstring> const & dirtyWords )
    {
        _maxLength = 0;
        clearBuffer();
        for( auto const & word : dirtyWords )
            if( _maxLength < word.size() )
                _maxLength = word.size();
        for( int i = 0; i < _maxLength; ++i )
        {
            auto buf = new char[ 65536 ];
            memset( buf, 0, 65536 );
            _bufs.push_back( buf );
        }

        for( size_t i = 0; i < _maxLength; ++i )
        {
            for( auto const & word : dirtyWords )
            {
                if( i >= word.size() ) continue;
                auto &c = word[ i ];
                if( i == word.size() - 1 )
                    _bufs[ i ][ c ] = 1;
                else
                    if( _bufs[ i ][ c ] != 1 )
                        _bufs[ i ][ c ] = 2;
            }
        }
    }
    inline wstring calc( wstring const & s ) const
    {
        assert( _bufs.size() );
        wstring o;
        o.reserve( s.size() );
        int lastMatch;
        for( size_t offset = 0; offset < s.size(); ++offset )
        {
            lastMatch = -1;
            for( size_t i = 0; i < MIN( _maxLength, s.size() - offset ); ++i )
            {
                auto& v = _bufs[ i ][ s[ offset + i ] ];
                if( v == 0 ) break;
                else if( v == 1 ) lastMatch = (int)i;
            }
            if( lastMatch == -1 )
            {
                o.push_back( s[ offset ] );
            }
            else
            {
                o.append( lastMatch + 1, '*' );
                offset += lastMatch;    // for will + 1
            }
        }
        return o;
    }
private:
    vector<char*> _bufs;
    size_t _maxLength;
    FastDW( FastDW const & other ) = delete;
    FastDW operator=( FastDW const & other ) = delete;
};
int main()
{
    vector<wstring> words =
    {
        L"操你",
        L"操你大爷",
        L"操你妈",
    };
    wstring s = L"我操你大爷,操你全家!操";
    FastDW fdw( words );
    auto o = fdw.calc( s );

    setlocale( LC_ALL, "chs" );
    std::wcout << o << endl;
}
