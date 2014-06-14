struct Console_t
{
    std::string _std_s;
    std::mutex _mtx;
    String _s;
    List<wchar> _ws;

    Console_t();
    String ReadLine();

    template<typename T>
    void Write( T && v )
    {
        std::lock_guard<std::mutex> lg( _mtx );
        _s.clear();
        _s << std::forward<T>( v );
        if( !_s._len ) return;
        _ws.clear();
        _ws.ensure( _s._len + 1 );
#pragma warning(disable:4996)
        auto len = Converts::toStringUnicode( _ws._buf, _s._buf, _s._len );
        _ws._buf[ len ] = 0;
        std::wcout << _ws._buf;
    }

    template<typename T>
    void WriteLine( T && v )
    {
        Write( std::forward<T>( v ) );
        std::cout << std::endl;
    }
};

extern Console_t Console;


struct Cout_t
{
    template<typename T>
    Cout_t const & operator<<( T && v ) const
    {
        Console.Write( std::forward<T>( v ) );
        return *this;
    }

};

extern Cout_t Cout;
