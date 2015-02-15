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
