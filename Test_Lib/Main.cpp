#include "Lib/All.h"
using namespace xxx;

class Foo
{
public:
    Foo* parent = nullptr;
    List<Foo*> childs;

    // ByteBuffer interface
    inline void WriteTo( ByteBuffer& bb ) const
    {
        // 看看自己有没有被序列化过
        auto rtv = bb.ptrStore->Insert( ( void* )this, bb.offset );

        // 写入当前或查到的 offset( 有没有序列化, 反正都要写的 )
        bb.Write( rtv.first->value );

        // 如果非首次序列化，就不序列化类成员了
        if( !rtv.second ) return;

        bb.WriteMulti( parent, childs );
    }
    inline bool ReadFrom( ByteBuffer& bb )
    {
        return true;
    }
};

class Bar
{
public:
    Bar* parent = nullptr;
    List<Bar*> childs;
    int xxx = 0;
    int xxxx = 0;

    // ByteBuffer interface
    inline void WriteTo( ByteBuffer& bb ) const
    {
        // 试将当前 this 指针和 offset 放入 ptr 容器
        auto rtv = bb.ptrStore->Insert( ( void* )this, bb.offset );

        // 写入当前或查到的 offset
        bb.Write( rtv.first->value );       

        // 如果是首次序列化，需要接着序列化内容
        if( rtv.second )
        {
            // 开始序列化内容
            bb.Write( parent );
            bb.Write( childs );
            bb.Write( xxx );
            bb.Write( xxxx );
        }
    }

    inline bool ReadFrom( ByteBuffer& bb )
    {
        return true;
    }
};


int main()
{
    Foo f;
    f.parent = &f;
    f.childs.Push( &f );
    f.childs.Push( &f );
    f.childs.Push( &f );

    ByteBuffer bb;
    bb.RootWrite( f );
    CoutLine( bb.Dump() );

    return 0;
}
