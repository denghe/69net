#ifndef __COR_H__
#define __COR_H__

namespace xxx
{
    // 最下面有较为全面的示例

    // 下面是可用于 coroutine 编程的实用宏
    // sample:
    /*
    bool FooState1::Process( int _ticks )
    {
        COR_BEGIN;
        Cout( name, " Process: before sleep( 2 )" );
        COR_SLEEP( 2 );
        Cout( name, " Process: sleeped" );
        for( i = 0; i < 9; ++i )
        {
            Cout( "Foo's i = ", i );
            if( bar )
            {
                Cout( "Foo's Bar alive!" );
            }
            COR_YIELD;
        }
        COR_END;
    }
    */

#define COR_BEGIN           if( corSleeps ) { return Sleeping(); }; \
                            switch( corLn ) { case 0:
#define COR_YIELD           corLn = __LINE__; return true; case __LINE__:
#define COR_END             } return false;
#define COR_SLEEP( N )      corSleeps = N - 1; COR_YIELD;


    struct CorManager;

    // coroutine 基类, 派生类须实现自己的 Init( .... ) 以及 Process 函数
    // sample:
    /*
    struct Foo : public CorBase, public AutoIDAttacher<Foo, CorBase>
    {
        String name;
        void Init( String const& _name );
        bool Process( int _ticks ) override;
    };
    struct FooChild1 : public Foo, public AutoIDAttacher<FooChild1, CorBase>
    struct FooChild1_1 : public FooChild1_1, public AutoIDAttacher<FooChild1_1, CorBase>
    */
    struct CorBase
    {
        CorBase() = default;                            // 默认的构造和析构, 派生类通常不需要实现。除非用于生命周期内不变的初始化传值
        virtual ~CorBase() {}

        // void Init( ... );                            // 相当于 new 
        inline virtual void EnsureRefs() {}             // for manager compress( 调所有 PoolPtr 类成员 Ensure() )
        inline virtual void Destroy() {}                // 相当于 析构

        int typeId = 0, versionNumber = 0;              // for pool
        int corIdx = 0, corLn = 0, corSleeps = 0;       // for coroutine
        virtual bool Process( int ticks ) = 0;
        inline virtual bool Sleeping() { --corSleeps; return true; }
        CorManager* manager = nullptr;
    };


    // 用于状态类的协程基类( 上面的简化版, 自己处理 Process )
    struct CorStateBase
    {
        virtual ~CorStateBase() {}
        int corIdx = 0, corLn = 0, corSleeps = 0;       // for coroutine
        virtual bool Process( int ticks ) = 0;
        virtual void Destroy() = 0;
        inline virtual bool Sleeping() { --corSleeps; return true; }
    };



    // coroutine 对象池 / 管理器
    // sample:
    /*
        CorManager cm;
        auto foo = cm.CreateItem<Foo>( ...... );
        ...
        foreach( o in RefItems<Xxxx>() ) ((Xxxx)o)->Xxxxxx;
        ...
        while( cm.Process( i ) {}
        ...
    */
    struct CorManager
    {
        List<List<CorBase*>> itemss;
        AutoIDPool<CorBase> pool;

        template<typename CT>
        List<CorBase*>& RefItems()
        {
#if __WIN
            auto& tid = CT::AutoIDAttacher<CT, CorBase>::autoTypeId.value;
#else
            auto& tid = CT::template AutoIDAttacher<CT, CorBase>::autoTypeId.value;
#endif
            return itemss[ tid ];
        }
        List<CorBase*>& RefItems( CorBase* o )
        {
            return itemss[ o->typeId ];
        }
        List<CorBase*>& RefItems( int typeId )
        {
            return itemss[ typeId ];
        }

        CorManager()
        {
            itemss.Resize( AutoID<CorBase>::maxValue );
        }
        ~CorManager()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* CreateItem( PTS&& ...ps )
        {
            auto& items = RefItems<CT>();
            auto rtv = pool.Alloc<CT>( std::forward<PTS>( ps )... );
            rtv->CorBase::corIdx = items.Size();
            rtv->CorBase::corLn = 0;
            rtv->CorBase::corSleeps = 0;
            rtv->CorBase::manager = this;
            items.Push( rtv );
            return rtv;
        }

        void DestroyItem( CorBase* o )
        {
            auto& items = RefItems( o );
            items.Top()->corIdx = o->corIdx;
            items.EraseFast( o->corIdx );
            pool.Free( o );
        }

        bool Process( int ticks = 0 )
        {
            for( int j = 0; j < itemss.Size(); ++j )
            {
                auto& items = itemss[ j ];
                for( int i = items.Size() - 1; i >= 0; --i )
                {
                    auto& o = items[ i ];
                    if( !o->Process( ticks ) ) DestroyItem( o );
                }
            }
            return true;
        }


        void Clear()
        {
            for( int j = 0; j < itemss.Size(); ++j )
            {
                auto& items = itemss[ j ];
                while( items.Size() )
                {
                    for( int i = items.Size() - 1; i >= 0; --i )
                    {
                        DestroyItem( items[ i ] );
                    }
                }
            }
            pool.versionNumber = 0;
        }

        void Compress()
        {
            for( int j = 0; j < itemss.Size(); ++j )
            {
                auto& items = itemss[ j ];
                for( int i = 0; i < items.Size(); ++i )
                {
                    items[ i ]->EnsureRefs();
                }
            }
            pool.Clear();
        }
    };
}

#endif


/*


struct Foo;
struct FooState1 : public CorStateBase
{
    Foo* owner;
    String name;
    int sleepTicks = 0;
    FooState1( Foo* owner );
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};
struct FooState2 : public CorStateBase
{
    Foo* owner;
    String name;
    int sleepTicks = 0;
    FooState2( Foo* owner );
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};
struct Foo : public CorBase, public AutoIDAttacher < Foo, CorBase >
{
    CorStateBase* s = nullptr;
    String name;
    FooState1 s1;
    FooState2 s2;
    Foo();
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};

struct Bar : public CorBase, public AutoIDAttacher < Bar, CorBase >
{
    void Init( String const& name );
    bool Process( int ticks ) override;
    String name;
    List<PoolPtr<Foo>> childs;
    void AddChild( Foo* foo );
};




Foo::Foo()
    : s1( this )
    , s2( this )
{
}
void Foo::Init( String const& name, int sleepTicks )
{
    this->name = name;
    s1.Init( String::Make( name, "_state_1" ), sleepTicks );
}
bool Foo::Process( int ticks )
{
    CoutLine( name, " Process" );
    s->Process( ticks );
    return s != nullptr;
}




FooState1::FooState1( Foo* owner )
{
    this->owner = owner;
}
void FooState1::Init( String const& name, int sleepTicks )
{
    this->name = name;
    this->sleepTicks = sleepTicks;
    owner->s = this;
}
bool FooState1::Process( int ticks )
{
    COR_BEGIN;
    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
    COR_SLEEP( sleepTicks );
    CoutLine( name, " Process: sleeped" );
    owner->s2.Init( String::Make( name, "_state_2" ), sleepTicks );
    COR_END;
}




FooState2::FooState2( Foo* owner )
{
    this->owner = owner;
}
void FooState2::Init( String const& name, int sleepTicks )
{
    this->name = name;
    this->sleepTicks = sleepTicks;
    owner->s = this;
}
bool FooState2::Process( int ticks )
{
    COR_BEGIN;
    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
    COR_SLEEP( sleepTicks );
    CoutLine( name, " Process: sleeped" );
    owner->s = nullptr;
    COR_END;
}






void Bar::Init( String const& name )
{
    this->name = name;
}

bool Bar::Process( int ticks )
{
    CoutLine( name, " Process begin" );
    Cout( "alive childs: {" );
    for( int i = childs.Size() - 1; i >= 0; --i )
    {
        auto& c = childs[ i ];
        if( auto p = c.Ptr() )
        {
            Cout( p->name, ", " );
        }
        else
        {
            childs.EraseFast( i );
        }
    }
    CoutLine( "}" );
    CoutLine( name, " Process end" );
    return true;
}

void Bar::AddChild( Foo* foo )
{
    childs.Push( foo );
}




int main()
{
    CorManager cm;
    auto b = cm.CreateItem<Bar>( "bar" );
    b->AddChild( cm.CreateItem<Foo>( "foo1", 1 ) );
    b->AddChild( cm.CreateItem<Foo>( "foo2", 2 ) );
    b->AddChild( cm.CreateItem<Foo>( "foo3", 3 ) );
    while( cm.Process() )
    {
        _getch();
        CoutLine();
    };
    system( "pause" );
    return 0;
}


*/