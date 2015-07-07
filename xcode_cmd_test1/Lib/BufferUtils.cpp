#include "All.h"

namespace xxx
{

    namespace BufferUtils
    {

        void DumpCore( String & s, char const * buf, int len )
        {
            for( int i = 0; i < len; ++i )
            {
                uint8 c = buf[ i ];
                if( c < 32 || c > 126 ) s.Append( '.' );
                else s.Append( (char)c );
            }
        }

        void Dump( String & s, char const * buf, int len )
        {
            if( len == 0 ) return;
            s.Append( "\n--------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F" );
            int i = 0;
            for( ; i < len; ++i )
            {
                if( ( i % 16 ) == 0 )
                {
                    if( i )
                    {           // output ascii to the end of the line
                        s.Append( "  " );
                        DumpCore( s, buf + i - 16, 16 );
                    }
                    s.Append( '\n' );
                    s.AppendHex( i );
                    s.Append( "  " );
                }
                else if( i && ( i % 4 == 0 ) )
                {
                    s.Append( "  " );
                }
                else s.Append( ' ' );
                s.AppendHex( (uint8)buf[ i ] );
            }
            int left = i % 16;
            if( left )
            {
                len = len + 16 - left;
                for( ; i < len; ++i )
                {
                    if( i && ( i % 4 == 0 ) )
                        s.Append( "  " );
                    else s.Append( ' ' );
                    s.Append( "  " );
                }
                s.Append( "  " );
                DumpCore( s, buf + i - 16, left );
            }
        }

    }

}