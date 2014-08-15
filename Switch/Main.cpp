#include "Lib/All.h"

//int main()
//{
//    static const int tc = 100;
//    std::vector<Pool> pools;
//    for( int i = 1; i < tc / 8 + 2; ++i )
//    {
//        pools.emplace_back( i * 8 );
//    }
//
//    std::cout << "Time: ";
//    Stopwatch sw;
//
//    for( int i = 1; i < tc; ++i )
//    {
//        void * p = pools[ i / 8 ].alloc();
//        //pool.free(p);
//    }
//    std::cout << sw.elapsedMillseconds() << "ms" << std::endl;
//    system( "pause" );
//    return 0;
//}

struct Foo : EnableSharedFromThis<Foo>
{
    Weak<Foo> parent;
    List<Weak<Foo>> childs;

    void addChild( Shared<Foo>& c )
    {
        c->parent = sharedFromThis();
        childs.push( c );
    }
    Foo()
    {
        Cout( "Foo()" );
    }
    ~Foo()
    {
        Cout( "~Foo()" );
    }
};


int main()
{
    {
        std::aligned_storage<sizeof( SharedType<Foo> ), std::alignment_of<SharedType<Foo>>::value>::type buf;

        auto f = makeSharedEx<Foo>( &buf, []
        {
            std::cout << "deleted" << std::endl;
        } );




        std::cout << " f._copys = " << f._st->_copys << " f._weaks = " << f._st->_weaks << std::endl;

        f->addChild( f );
        std::cout << " f._copys = " << f._st->_copys << " f._weaks = " << f._st->_weaks << std::endl;

        f->addChild( f );
        std::cout << " f._copys = " << f._st->_copys << " f._weaks = " << f._st->_weaks << std::endl;

        f->addChild( f );
        std::cout << " f._copys = " << f._st->_copys << " f._weaks = " << f._st->_weaks << std::endl;

    }

    return 0;
}


//
//int main()
//{
//    Pool pool( 4 );
//    static const int tc = 10000000;
//    std::cout << "Time: ";
//    Stopwatch sw;
//
//    for( int i = 0; i < tc; ++i )
//    {
//        // void * p = malloc( ( i & 0xfff ) );
//        //free(p);
//
//        // void * p = ::operator new( i & 0xfff );
//        //delete[] p;
//
//        void * p = malloc( 4 );
//        free(p);
//
//        //void * p = pool.alloc();
//        //pool.free(p);
//    }
//    std::cout << sw.elapsedMillseconds() << "s" << std::endl;
//    system( "pause" );
//    return 0;
//}
