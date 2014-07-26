template<typename ...TS>
void Cout( TS const& ...parms )
{
    String s;
    s.append( parms... );
    puts( s.c_str() );
}

template<typename ...TS>
void Coutn( TS const& ...parms )
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
