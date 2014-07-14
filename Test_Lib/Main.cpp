#include "Lib/All.h"
using namespace std;


struct Foo
{
    int i;
    double d;
    float f;
    String s;
    // for FlatBuffer write
    inline int getBufferSize() const
    {
        return sizeof( int ) + sizeof( double ) + sizeof( float ) + s.size();
    }
    inline void writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes( i, d, f, s );
    }
    inline void writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect( i, d, f, s );
    }
    inline bool readBuffer( FlatBuffer& fb )
    {
        return fb.reads( i, d, f, s );
    }
};

struct Foos
{
    int i;
    double d;
    float f;
    String s;
    List<Foo> fs;
    // for FlatBuffer write
    inline int getBufferSize() const
    {
        return sizeof( int ) + sizeof( double ) + sizeof( float ) + s.size() + fs.getBufferSize();
    }
    inline void writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes( i, d, f, s, fs );
    }
    inline void writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect( i, d, f, s, fs );
    }
    inline bool readBuffer( FlatBuffer& fb )
    {
        return fb.reads( i, d, f, s, fs );
    }
};


int main()
{
    //FlatBuffer fb;
    //fb.writes( 123, 234.0f, 345.01, "0123456789", true );
    //int I;
    //if( !fb.read( I ) )
    //{
    //    cout << "!fb.read( I )" << endl;
    //    return 0;
    //}
    //float F;
    //if( !fb.read( F ) )
    //{
    //    cout << "!fb.read( F )" << endl;
    //    return 0;
    //}
    //double D;
    //if( !fb.read( D ) )
    //{
    //    cout << "!fb.read( D )" << endl;
    //    return 0;
    //}
    //String S;
    //if( !fb.read( S ) )
    //{
    //    cout << "!fb.read( S )" << endl;
    //    return 0;
    //}
    //byte B;
    //if( !fb.read( B ) )
    //{
    //    cout << "!fb.read( B )" << endl;
    //    return 0;
    //}
    //cout << I << ", " << F << ", " << D << ", " << S.c_str() << ", " << (B?"true":"false") << endl;
    //cout << fb.offset() << endl;

    //fb.offset() = 0;
    //I = 0; F = 0; D = 0; S.clear(); B = false;
    //fb.reads( I, F, D, S, B );
    //cout << I << ", " << F << ", " << D << ", " << S.c_str() << ", " << ( B ? "true" : "false" ) << endl;

    //cout << fb.offset() << endl;

    FlatBuffer fb;
    {
        List<Foos> fooss;
        Foos foos = { 234, 234.567, 234.567f, "kjjljkjlkj" };
        Foo f = { 123, 123.456, 123.456f, "asdfqwer" };
        foos.fs.push( f );
        foos.fs.push( f );
        foos.fs.push( f );
        fooss.push( foos );
        fooss.push( foos );
        fooss.push( foos );
        fb.write( fooss );
    }
    cout << fb.dump().c_str() << endl;
    {
        List<Foos> fooss;
        fb.read( fooss );
        cout << fooss.size() << endl;
    }


    //FlatBuffer fb111;
    //{
    //    List<List<List<String>>> llls;
    //    List<List<String>> lls;
    //    List<String> ls;
    //    ls.push( "ooooo" );
    //    ls.push( "xxxxx" );
    //    lls.push( ls );
    //    lls.push( ls );
    //    llls.push( lls );
    //    llls.push( lls );

    //    FlatBuffer fb;
    //    Stopwatch sw;
    //    for( int i = 0; i < 9999999; i++ )
    //    {
    //        fb.clear();
    //        fb.writes( llls, 123, 1.23f, 1.23, "asdfqwerzxcv1234", (byte)123, true );
    //    }
    //    cout << sw.elapsed() << endl;
    //    cout << fb.dump().c_str() << endl;

    //    fb111 = fb;
    //}

    // todo: FBRead

    //int I;
    //if( !fb111.read( I ) )
    //{
    //    cout << "!fb111.read( I )" << endl;
    //    return 0;
    //}
    //List<List<List<String>>> llls;
    //if( !fb111.read( llls ) )
    //{
    //    cout << "!fb111.read( llls )" << endl;
    //    return 0;
    //}
    //float F;
    //if( !fb111.read( F ) )
    //{
    //    cout << "!fb111.read( F )" << endl;
    //    return 0;
    //}
    //double D;
    //if( !fb111.read( D ) )
    //{
    //    cout << "!fb111.read( D )" << endl;
    //    return 0;
    //}
    //String S;
    //if( !fb111.read( S ) )
    //{
    //    cout << "!fb111.read( S )" << endl;
    //    return 0;
    //}
    //byte B;
    //if( !fb111.read( B ) )
    //{
    //    cout << "!fb111.read( B )" << endl;
    //    return 0;
    //}

    //for( int i = 0; i < llls.size(); ++i )
    //{
    //    auto& lls = llls[ i ];
    //    for( int j = 0; j < lls.size(); ++j )
    //    {
    //        auto& ls = lls[ j ];
    //        for( int k = 0; k < ls.size(); ++k )
    //        {
    //            cout << ls[ k ].c_str() << endl;
    //        }
    //    }
    //}


    return 0;
}
