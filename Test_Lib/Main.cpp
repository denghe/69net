#include "Lib/All.h"
using namespace std;
int main()
{
    FlatBuffer fb111;
    {
        List<List<List<String>>> llls;
        List<List<String>> lls;
        List<String> ls;
        ls.push( "ooooo" );
        ls.push( "xxxxx" );
        lls.push( ls );
        lls.push( ls );
        llls.push( lls );
        llls.push( lls );

        FlatBuffer fb;
        fb.writes( 123, llls, 1.23f, 1.23, "asdf", (byte)123 );
        cout << fb.dump().c_str() << endl;

        fb111 = fb;
    }

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
