#include "Lib/All.h"
using namespace xxx;


// todo: ByteBuffer ptrStore idxStore 改数据类型
// 各种容器的支持
// 指针是否为空的 byte 的功能利用：比如可存序列化类型，
// 类型可能指的是 是否逐个序列化，还是说  index : value 部分列化， 或是 bit 标记
// 部分序列化，指成员变量的值与默认值不等的那些。
// 极端需求下，可对比两种方式的序列化后长度差，选出最省的方式

class Foo
{
public:
    Foo* parent = nullptr;
    List<Foo*> childs;

    // ByteBuffer interface
    inline void WriteTo( ByteBuffer& bb ) const
    {
        bb.WriteMulti( this->parent, this->childs );
    }
    inline bool ReadFrom( ByteBuffer& bb )
    {
        return bb.ReadMulti( this->parent, this->childs );
    }
};

int main()
{
    ByteBuffer bb;

    {
        Foo f;
        f.parent = &f;
        f.childs.Push( &f );
        f.childs.Push( &f );
        f.childs.Push( &f );

        bb.RootWrite( f );
        CoutLine( bb.Dump() );
    }

    {
        Foo f;
        if( bb.RootRead( f ) )
        {
            CoutLine( (uint64)f.parent );
            for( auto i = 0; i < f.childs.Size(); ++i )
            {
                CoutLine( (uint64)f.childs[ i ] );
            }
        }
    }

    return 0;
}



//
//
//class Bar
//{
//public:
//    int a = 1;
//    int b = 2;
//
//    // ByteBuffer interface
//    inline void WriteTo( ByteBuffer& bb ) const
//    {
//        bb.VarWrite( a );
//        bb.VarWrite( b );
//    }
//
//    inline bool ReadFrom( ByteBuffer& bb )
//    {
//        if( !bb.VarRead( a ) ) return false;
//        if( !bb.VarRead( b ) ) return false;
//        return true;
//    }
//};
//
//
//int main()
//{
//    ByteBuffer bb;
//    {
//        Bar b;
//        b.a = 3;
//        b.b = 4;
//        bb.Write( b );
//        CoutLine( bb.Dump() );
//    }
//    {
//        Bar b;
//        if( bb.Read( b ) )
//        {
//            CoutLine( "b.a = ", b.a, ", b.b = ", b.b );
//        }
//    }
//    
//    return 0;
//}
