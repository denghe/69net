#include "../Lib/All.h"
#include "String.h"

int main()
{
    //{
    //    char buf[ 16 ];
    //    String s( buf );
    //    auto check = [ &] { std::cout << ( s.c_str() == buf ? "true" : "false" ) << "\n"; };
    //    s.append( "asdf!" );
    //    check();
    //    s.append( " sizeof(String) = ", sizeof( String ), ", lalalalala", 123.456, " ", 6666 );
    //    check();
    //    std::cout << s.c_str() << "\n";

    //    String ss = "asdf";
    //    ss.append( "**********************************************************************************************" );
    //    ss.append( "**********************************************************************************************" );
    //    String s2 = std::move( ss );
    //    std::cout << s2.c_str() << "\n";
    //}

    //char buf1[ 3 ];
    //char buf[] = "ADSGGGEFEFEFEFEFEEEEE";
    //String s( buf );
    //s.toLowerUnsafe();


    String s1( "asdfqwer" );        // ref
    char buf[ 16 ];
    String s2( buf );               // ref
    s2 = s1;
    String s3 = buf;                // copy

    return 0;
}











//#include "Lib/All.h"
//#include "String.h"
//using namespace std;
//int main()
//{
//    char s[] = "ASsdERdsdfs";
//    _strlwr_s( s );
//    cout << s << endl;
//
//    //auto 定长串 = "asdfqer";
//    ////auto 宽定长串 = L"啊喔额啊喔额";
//    //Stopwatch sw;
//    //sw.reset();
//    //{
//    //    String s;
//    //    s.reserve( 128 );
//    //    for( int i = 0; i < 9999999; ++i )
//    //    {
//    //        s.clear();  // 手手清，不测内存分配
//    //        s.append( 定长串, i, 定长串, i, 定长串, i, 定长串, i ); //, 宽定长串
//    //    }
//    //    std::cout << s.c_str()[ 0 ] << std::endl;
//    //}
//    //std::cout << sw.elapsed() << std::endl;
//    //sw.reset();
//    //{
//    //    std::string s;
//    //    //std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//    //    s.reserve( 128 );
//    //    for( int i = 0; i < 9999999; ++i )
//    //    {
//    //        s.clear();  // 手手清，不测内存分配
//    //        s.append( 定长串 );
//    //        s.append( std::to_string( i ) );
//    //        s.append( 定长串 );
//    //        s.append( std::to_string( i ) );
//    //        s.append( 定长串 );
//    //        s.append( std::to_string( i ) );
//    //        s.append( 定长串 );
//    //        s.append( std::to_string( i ) );
//    //        //s.append( conv.to_bytes( 宽定长串 ) );
//    //    }
//    //    std::cout << s.c_str()[ 0 ] << std::endl;
//    //}
//    //std::cout << sw.elapsed() << std::endl;
//    return 0;
//}
//
















































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


