#include "Lib/All.h"
#include "String.h"
int main()
{
    auto 定长串 = "asdfqer";
    //auto 宽定长串 = L"啊喔额啊喔额";
    Stopwatch sw;
    sw.reset();
    {
        String s;
        s.reserve( 128 );
        for( int i = 0; i < 9999999; ++i )
        {
            s.clear();  // 手手清，不测内存分配
            s.append( 定长串, i, 定长串, i, 定长串, i, 定长串, i ); //, 宽定长串
        }
        std::cout << s.c_str()[ 0 ] << std::endl;
    }
    std::cout << sw.elapsed() << std::endl;
    sw.reset();
    {
        std::string s;
        //std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        s.reserve( 128 );
        for( int i = 0; i < 9999999; ++i )
        {
            s.clear();  // 手手清，不测内存分配
            s.append( 定长串 );
            s.append( std::to_string( i ) );
            s.append( 定长串 );
            s.append( std::to_string( i ) );
            s.append( 定长串 );
            s.append( std::to_string( i ) );
            s.append( 定长串 );
            s.append( std::to_string( i ) );
            //s.append( conv.to_bytes( 宽定长串 ) );
        }
        std::cout << s.c_str()[ 0 ] << std::endl;
    }
    std::cout << sw.elapsed() << std::endl;
    return 0;
}















































//#include "../Lib/All.h"
//#include "String.h"
//
//int main()
//{
//    {
//        char buf[ 16 ];
//        String s( buf );
//        auto check = [ &] { std::cout << ( s.c_str() == buf ? "true" : "false" ) << "\n"; };
//        s.append( "asdf!" );
//        check();
//        s.append( " sizeof(String) = ", sizeof( String ), ", lalalalala", 123.456, " ", 6666 );
//        check();
//        std::cout << s.c_str() << "\n";
//
//        String ss = "asdf";
//        ss.append( "**********************************************************************************************" );
//        ss.append( "**********************************************************************************************" );
//        String s2 = std::move( ss );
//        std::cout << s2.c_str() << "\n";
//    }
//    return 0;
//}





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











//#include "Lib/All.h"
//
//inline void tolowerPtr( char *p );
//
//inline void tolowerStr( std::string& s )
//{
//    char* c = const_cast<char*>( s.c_str() );
//    size_t l = s.size();
//    for( char* c2 = c; c2 < c + l; c2++ )tolowerPtr( c2 );
//};
//
//inline void tolowerPtr( char *p )
//{
//    switch( *p )
//    {
//    case 'A':*p = 'a'; return;
//    case 'B':*p = 'b'; return;
//    case 'C':*p = 'c'; return;
//    case 'D':*p = 'd'; return;
//    case 'E':*p = 'e'; return;
//    case 'F':*p = 'f'; return;
//    case 'G':*p = 'g'; return;
//    case 'H':*p = 'h'; return;
//    case 'I':*p = 'i'; return;
//    case 'J':*p = 'j'; return;
//    case 'K':*p = 'k'; return;
//    case 'L':*p = 'l'; return;
//    case 'M':*p = 'm'; return;
//    case 'N':*p = 'n'; return;
//    case 'O':*p = 'o'; return;
//    case 'P':*p = 'p'; return;
//    case 'Q':*p = 'q'; return;
//    case 'R':*p = 'r'; return;
//    case 'S':*p = 's'; return;
//    case 'T':*p = 't'; return;
//    case 'U':*p = 'u'; return;
//    case 'V':*p = 'v'; return;
//    case 'W':*p = 'w'; return;
//    case 'X':*p = 'x'; return;
//    case 'Y':*p = 'y'; return;
//    case 'Z':*p = 'z'; return;
//    };
//    return;
//}
//
//static byte lowerchars[] =
//{
//    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
//};
//static byte upperchars[] =
//{
//    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
//};
//void toLower( std::string& word )
//{
//    auto p = (byte*)word.c_str();
//    for( int i = 0; i < word.size(); ++i ) p[ i ] = lowerchars[ p[ i ] ];
//}
//void toUpper( std::string& word )
//{
//    auto p = (byte*)word.c_str();
//    for( int i = 0; i < word.size(); ++i ) p[ i ] = upperchars[ p[ i ] ];
//}
//
//void toLower2( std::string& word )
//{
//    auto p = (byte*)word.c_str();
//    for( int i = 0; i < word.size(); ++i ) if( p[ i ] >= 'A' && p[ i ] <= 'Z' ) p[ i ] += 'a' - 'A';
//}
//
//// 32 ~ 126 的常见显示用字符中，这 7 个会因该算法受伤：   @[\]^_` 
//// 已知这个函数有 bug. for 超了
//void fastToLower( std::string& word )
//{
//    auto p = (byte*)word.c_str();
//    int i;
//    for( i = 0; i < word.size(); i += 8 ) *(uint64*)( p + i ) |= 0x2020202020202020;
//    *(uint64*)( p + i ) |= ( 0x2020202020202020 >> ( ( 8 - i - word.size() ) * 8 ) );
//}
//
//
//void testtransform( std::string& word )
//{
//    std::string word2 = word;
//    time_t t;
//    time_t t2;
//    time( &t );
//    std::cout << "testtransform: start " << "\n";
//    int i = 0;
//    for( ; i < 99999999; i++ )
//    {
//        word2 = word;
//        std::transform( word2.begin(), word2.end(), word2.begin(), ::tolower );
//    }
//    time( &t2 );
//    std::cout << word2 << "\n";
//    std::cout << "testtransform: end " << i << ":" << t2 - t << "\n";
//}
//
//void testmytolower( std::string& word )
//{
//    std::string word2 = word;
//    time_t t;
//    time_t t2;
//    time( &t );
//    std::cout << "testmytolower: start " << "\n";
//    int i = 0;
//    for( ; i < 99999999; i++ )
//    {
//        word2 = word;
//        tolowerStr( word2 );
//    }
//    time( &t2 );
//    std::cout << word2 << "\n";
//    std::cout << "testmytolower: end " << i << ":" << t2 - t << "\n";
//}
//
//void test2( std::string& word )
//{
//    std::string word2 = word;
//    time_t t;
//    time_t t2;
//    time( &t );
//    std::cout << "testmytolower: start " << "\n";
//    int i = 0;
//    for( ; i < 99999999; i++ )
//    {
//        word2 = word;
//        toLower( word2 );
//    }
//    time( &t2 );
//    std::cout << word2 << "\n";
//    std::cout << "testmytolower: end " << i << ":" << t2 - t << "\n";
//}
//
//void test3( std::string& word )
//{
//    std::string word2 = word;
//    time_t t;
//    time_t t2;
//    time( &t );
//    std::cout << "testmytolower: start " << "\n";
//    int i = 0;
//    for( ; i < 99999999; i++ )
//    {
//        word2 = word;
//        fastToLower( word2 );
//    }
//    time( &t2 );
//    std::cout << word2 << "\n";
//    std::cout << "testmytolower: end " << i << ":" << t2 - t << "\n";
//}
//
//int main( int argc, char* argv[] )
//{
//    std::string word = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//    word = word + word + word + word + word + word + word + word + word + word + word + word + word + word + word;
//    //testtransform( word );
//    //testmytolower( word );
//    //test2( word );
//    test3( word );
//    return 0;
//}
