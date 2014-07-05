#include "Lib/All.h"
int main()
{
    std::cout << sizeof( Dict<int, int> ) << std::endl;
    std::cout << sizeof( std::map<int, int> ) << std::endl;
    return 0;
}
