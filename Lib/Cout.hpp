#pragma once

namespace xxx
{

    template<typename ...TS>
    void Cout( TS const& ...parms )
    {
        String s;
        s.Append( parms... );
        //printf( "%s", s.C_str() );
        fwrite( s.C_str(), 1, s.Size(), stdout );
    }

    template<typename ...TS>
    void CoutLine( TS const& ...parms )
    {
        Cout( parms..., '\n' );
    }

    template<typename ...TS>
    void CoutPos( short x, short y, TS const& ...parms )
    {
#if __WIN
        static auto h = GetStdHandle( STD_OUTPUT_HANDLE );
        static COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition( h, pos );
#else
        printf( "\033[%d;%dH", x + 1, y + 1 );
#endif
        Cout( parms... );
    }


}
