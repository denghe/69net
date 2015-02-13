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
        // �����Լ���û�б����л���
        auto rtv = bb.ptrStore->Insert( ( void* )this, bb.offset );

        // д�뵱ǰ��鵽�� offset( ��û�����л�, ������Ҫд�� )
        bb.Write( rtv.first->value );

        // ������״����л����Ͳ����л����Ա��
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
        // �Խ���ǰ this ָ��� offset ���� ptr ����
        auto rtv = bb.ptrStore->Insert( ( void* )this, bb.offset );

        // д�뵱ǰ��鵽�� offset
        bb.Write( rtv.first->value );       

        // ������״����л�����Ҫ�������л�����
        if( rtv.second )
        {
            // ��ʼ���л�����
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
