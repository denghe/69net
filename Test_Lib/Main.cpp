#include "Lib/All.h"
int main()
{
    //String s = "a";
    //std::cout << s.getHash() << std::endl;
    //s = "as";
    //std::cout << s.getHash() << std::endl;
    //s = "asd";
    //std::cout << s.getHash() << std::endl;
    //s = "asdf";
    //std::cout << s.getHash() << std::endl;
    //s = "asdfq";
    //std::cout << s.getHash() << std::endl;
    //s = "asdfqw";
    //std::cout << s.getHash() << std::endl;
    //s = "asdfqwe";
    //std::cout << s.getHash() << std::endl;
    //s = "asdfqwer";
    //std::cout << s.getHash() << std::endl;
    String s = "asdfqwerasdfqwerasdfqwerasdfqwer";
    int hash = 0;
    Stopwatch sw;
    for( int i = 0; i < 99999999; ++i )
    {
        hash = s.getHash_CS();
    }
    std::cout << hash << ", " << sw.elapsed() << std::endl;
    sw.reset();
    for( int i = 0; i < 99999999; ++i )
    {
        hash = s.getHash_Java();
    }
    std::cout << hash << ", " << sw.elapsed() << std::endl;
    sw.reset();
    for( int i = 0; i < 99999999; ++i )
    {
        hash = s.getHash_Lua();
    }
    std::cout << hash << ", " << sw.elapsed() << std::endl;
    return 0;
}




//#include "Lib/All.h"
//
//int main()
//{
//String s1( "ASDFQWERASDFQWERASDFQWER", true );          // ref str
//char buf[ 16 ];
//String s2( buf, _countof( buf ), 0 );   // ref buf
//s2 = s1;                                // copy str to buf
//String s3 = buf;                        // copy from buf


//Stopwatch sw;
//{
//    std::string s;
//    for( int i = 0; i < 99999999; ++i )
//    {
//        s = s1;
//        for( int i = 0; i < s.size(); i++ )
//            s[ i ] = tolower( s[ i ] );
//    }
//    std::cout << s << std::endl;
//}
//std::cout << sw.elapsed() << std::endl;

//sw.reset();
//{
//    String s;
//    for( int i = 0; i < 99999999; ++i )
//    {
//        s = s1;
//        //s.toLower();
//        s.toLowerUnsafe();
//    }
//    std::cout << s << std::endl;
//}
//std::cout << sw.elapsed() << std::endl;

////int equal = 0, less = 0, larger = 0;
////Stopwatch sw;
////{
////    std::string s1 = "aaaasdfasdf", s2 = s1, s3 = "aaaasdfasda";
////    for( int i = 0; i < 99999999; ++i )
////    {
////        if( s1 == s2 ) ++equal;
////        if( s3 < s1 ) ++less;
////        if( s1 > s3 ) ++larger;
////    }
////}
////std::cout << equal << " " << less << " " << larger << " " << sw.elapsed() << std::endl;

////equal = 0, less = 0, larger = 0;
////sw.reset();
////{
////    String s1 = "aaaasdfasdf", s2 = s1, s3 = "aaaasdfasda";
////    for( int i = 0; i < 99999999; ++i )
////    {
////        if( s1 == s2 ) ++equal;
////        if( s3 < s1 ) ++less;
////        if( s1 > s3 ) ++larger;
////    }
////}
////std::cout << equal << " " << less << " " << larger << " " << sw.elapsed() << std::endl;

//    return 0;
//}
//









//
//#include "Lib/All.h"
//#include "String.h"
//using namespace std;
//int main()
//{
//    auto 定长串 = "asdfqer";
//    Stopwatch sw;
//    sw.reset();
//    {
//        String s;
//        //s.reserve( 128 );
//        for( int i = 0; i < 9999999; ++i )
//        {
//            //s.clear();  // 手手清，不测内存分配
//            s.append( 定长串, i, 定长串, i, 定长串, i, 定长串, i );
//        }
//        std::cout << s.c_str()[ 0 ] << s.size() << std::endl;
//    }
//    std::cout << sw.elapsed() << std::endl;
//    sw.reset();
//    {
//        std::string s;
//        //s.reserve( 128 );
//        for( int i = 0; i < 9999999; ++i )
//        {
//            //s.clear();  // 手手清，不测内存分配
//            s.append( 定长串 );
//            s.append( std::to_string( i ) );
//            s.append( 定长串 );
//            s.append( std::to_string( i ) );
//            s.append( 定长串 );
//            s.append( std::to_string( i ) );
//            s.append( 定长串 );
//            s.append( std::to_string( i ) );
//        }
//        std::cout << s.c_str()[ 0 ] << s.size() << std::endl;
//    }
//    std::cout << sw.elapsed() << std::endl;
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


