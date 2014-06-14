#include "Precompile.h"

Console_t Console;
Cout_t Cout;

Console_t::Console_t()
{
#ifdef __linux
    setlocale(LC_ALL, "zh_CN.UTF-8");
#else
    setlocale(LC_ALL, "chs");
#endif
}

String Console_t::ReadLine()
{
    _std_s.clear();
    std::cin >> _std_s;
    return String( _std_s.c_str(), (int)_std_s.size() );
}
