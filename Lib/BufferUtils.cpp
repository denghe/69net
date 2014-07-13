#include "All.h"

namespace BufferUtils
{

    void dumpCore( String & s, char const * buf, int len )
    {
        for( int i = 0; i < len; ++i )
        {
            uint8 c = buf[ i ];
            if( c < 32 || c > 126 ) s.append( '.' );
            else s.append( (char)c );
        }
    }

    void dump( String & s, char const * buf, int len )
    {
        if( len == 0 ) return;
        s.append( "\n--------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F" );
        int i = 0;
        for( ; i < len; ++i )
        {
            if( ( i % 16 ) == 0 )
            {
                if( i )
                {           // output ascii to the end of the line
                    s.append( "  " );
                    dumpCore( s, buf + i - 16, 16 );
                }
                s.append( '\n' );
                s.appendHex( i );
                s.append( "  " );
            }
            else if( i && ( i % 4 == 0 ) )
            {
                s.append( "  " );
            }
            else s.append( ' ' );
            s.appendHex( (uint8)buf[ i ] );
        }
        int left = i % 16;
        if( left )
        {
            len = len + 16 - left;
            for( ; i < len; ++i )
            {
                if( i && ( i % 4 == 0 ) )
                    s.append( "  " );
                else s.append( ' ' );
                s.append( "  " );
            }
            s.append( "  " );
            dumpCore( s, buf + i - 16, left );
        }
    }

}