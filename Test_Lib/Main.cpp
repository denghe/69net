#include "Lib/All.h"
using namespace std;

#define COR_BEGIN       if( sleeps ) { return Sleeping(); }; \
                        switch( ln ) { case 0:
#define COR_YIELD       ln = __LINE__; return true; case __LINE__:
#define COR_END         } return false;
#define COR_SLEEP( N )  sleeps = N - 1; COR_YIELD;

// todo: message ?

struct FooBaseManager;
struct FooBase
{
    FooBaseManager* manager;

    //static const int typeId = xxx;
    virtual int GetTypeId() = 0;    // return typeId;

    // void Init( ... );
    virtual void Destroy() {}
    virtual ~FooBase() {}

    int id = 0, idx = 0, ln = 0, sleeps = 0;
    virtual bool Process() = 0;
    virtual bool Sleeping() { --sleeps; return true; }
};

struct FooBaseManager
{
    List<FooBase*> items;
    Dict<int, List<FooBase*>> avaliables;
    int aiid = 0;

    template<typename T, typename ...PTS>
    T* CreateItem( PTS ...parms )
    {
        static_assert( std::is_base_of<FooBase, T>::value, "T must be inherit from FooBase*" );
        T* rtv;
        auto& pool = avaliables[ T::typeId ];
        if( pool.size() )
        {
            rtv = (T*)avaliables[ T::typeId ].top_pop();
        }
        else
        {
            rtv = new T();
            rtv->manager = this;
        }
        rtv->id = ++aiid;
        rtv->Init( std::forward<PTS>( parms )... );
        rtv->idx = items.size();
        rtv->ln = 0;
        rtv->sleeps = 0;
        items.push( rtv );
        return rtv;
    }

    void DestroyItem( FooBase* foo )
    {
        items.top()->idx = foo->idx;
        items.eraseFast( foo->idx );
        foo->Destroy();
        foo->id = 0;
        avaliables[ foo->GetTypeId() ].push( foo );
    }

    bool Process()
    {
        if( !items.size() ) return false;
        for( int i = items.size() - 1; i >= 0; --i )
        {
            auto& foo = items[ i ];
            if( !foo->Process() ) DestroyItem( foo );
        }
        return true;
    }

    void Clear()
    {
        for( int i = items.size() - 1; i >= 0; --i )
        {
            DestroyItem( items[ i ] );
        }
        aiid = 0;
    }

    void Compress()
    {
        for( int i = avaliables.size() - 1; i >= 0; --i )
        {
            auto& pool = avaliables.data()[ i ]->value;
            for( int j = 0; j < pool.size(); ++j )
            {
                delete pool[ j ];
            }
            pool.clear();
        }
    }
};

struct Foo : public FooBase
{
    static const int typeId = 123;
    virtual int GetTypeId() { return typeId; }

    Foo() { Cout( "new Foo" ); }

    int i;
    String txt;

    void Init( String _txt )
    {
        txt = _txt;
    }

    virtual bool Process() override
    {
        COR_BEGIN;
        for( i = 0; i < 3; ++i )
        {
            Cout( txt, i );
            COR_YIELD;
            COR_SLEEP( 2 );
        }
        COR_END;
    }
};

int main()
{
    FooBaseManager fbm;
    fbm.CreateItem<Foo>( "foo1: " );
    fbm.CreateItem<Foo>( "foo2: " );
    while( fbm.Process() );
    fbm.CreateItem<Foo>( "foo1: " );
    fbm.CreateItem<Foo>( "foo2: " );
    while( fbm.Process() );

    //Foo f;
    //for( int i = 0; f.Process(); ++i ) Cout( "i = ", i );

    /*
        List<FooBase*> foos;
        for( int i = 0; i < 3; ++i )
        {
        auto foo = new Foo();
        foo->idx = foos.size();
        foos.push( foo );
        }
        int alive = foos.size();
        while( alive )
        {
        }*/

    system( "pause" );
    return 0;
}
