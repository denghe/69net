#ifndef __COR_H__
#define __COR_H__

namespace xxx
{
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
        inline virtual bool Sleeping() { --corSleeps; return true; }
    };



    // coroutine 对象池 / 管理器
    // sample:
    /*
        CorManager<CorBase> cm;
        auto foo = cm.CreateItem<Foo>( ...... );
        ...
        while( cm.Process( i ) {}
        ...
    */
    struct CorManager
    {
        List<CorBase*> items;
        AutoIDPool<CorBase> pool;
        int versionNumber = 0;

        ~CorManager()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* CreateItem( PTS&& ...ps )
        {
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
            items.Top()->corIdx = o->corIdx;
            items.EraseFast( o->corIdx );
            pool.Free( o );
        }

        bool Process( int ticks = 0 )
        {
            if( !items.Size() ) return false;
            for( int i = items.Size() - 1; i >= 0; --i )
            {
                auto& o = items[ i ];
                if( !o->Process( ticks ) ) DestroyItem( o );
            }
            return true;
        }

        void Clear()
        {
            while( items.Size() )
            {
                for( int i = items.Size() - 1; i >= 0; --i )
                {
                    DestroyItem( items[ i ] );
                }
            }
            pool.versionNumber = 0;
        }

        void Compress()
        {
            for( int i = 0; i < items.Size(); ++i )
            {
                items[ i ]->EnsureRefs();
            }
            pool.Clear();
        }
    };
}

#endif
