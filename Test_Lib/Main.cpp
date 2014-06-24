#include "../Lib/All.h"
#include "String.h"

int main()
{
    {
        char buf[ 16 ];
        String s( buf );
        auto check = [ &] { std::cout << ( s.c_str() == buf ? "true" : "false" ) << "\n"; };
        s.append( "asdf!" );
        check();
        s.append( " sizeof(String) = ", sizeof( String ), ", lalalalala", 123.456, " ", 6666 );
        check();
        std::cout << s.c_str() << "\n";

        String ss = "asdf";
        ss.append( "**********************************************************************************************" );
        ss.append( "**********************************************************************************************" );
        String s2 = std::move( ss );
        std::cout << s2.c_str() << "\n";
    }
    return 0;
}

// todo: 实现个类似 map 的东西，当 key 传 String 时，存 Hash

/*

c# 的 hash 计算代码，因为是 for wchar_t 的，看看要不要处理( 2位2位的算? )

char* chPtr = str;
int num = 0x15051505;
int num2 = num;
int* numPtr = (int*) chPtr;
int length = this.Length;
while (length > 2)
{
num = (((num << 5) + num) + (num >> 0x1b)) ^ numPtr[0];
num2 = (((num2 << 5) + num2) + (num2 >> 0x1b)) ^ numPtr[1];
numPtr += 2;
length -= 4;
}
if (length > 0)
{
num = (((num << 5) + num) + (num >> 0x1b)) ^ numPtr[0];
}
return (num + (num2 * 0x5d588b65));

*/