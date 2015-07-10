#include "Lib/All.h"

// todo:

// 整理 Sqlite 类相关
// 以类似 Sqlite 类的长相，提供 Mysql 的支持
// 设计 Mysql 数据库操作层的长相

// 分配器 for ByteBuffer ?
// Nullable 的序列化支持?
// Any 类型的引入? ( 可以是 c++ 内置数据类型 以及常用常见的集合啥的 )
// 简化各种 Size()
// 指针是否为空的 byte 的功能利用：比如可存序列化类型，
// 类型可能指的是 是否逐个序列化，还是说  index : value 部分列化， 或是 bit 标记
// 部分序列化，指成员变量的值与默认值不等的那些。
// 极端需求下，可对比两种方式的序列化后长度差，选出最省的方式

using namespace std;
using namespace xxx;

void Dump( Dict<int, int> const& d )
{
    for( int i = 0; i < d.Size(); ++i )
    {
        CoutLine( d.IndexAt( i )->value );
    }
}
int main()
{
    Dict<int, int> d;
    d.Insert( 123, 123 );
    d[ 456 ] = 456;
    Dump( d );
    return 0;
}
