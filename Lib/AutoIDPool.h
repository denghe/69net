#ifndef __AUTOIDPOOL_H__
#define __AUTOIDPOOL_H__

namespace xxx
{
    // 作为静态成员置入 class, 提供自增 type id 的生成功能
    template<typename T>
    struct AutoID
    {
        int value;
        static int maxValue;
        AutoID() { value = maxValue++; }
    };
    template<typename T>
    int AutoID<T>::maxValue = 0;




    // 为类附加自增ID
    // sample:
    /*
    struct Foo : public XxxBase, public AutoIDAttacher<Foo, XxxBase>
    struct Foo_Child : public Foo, public AutoIDAttacher<Foo_Child, XxxBase>
    */
    template<typename T, typename BT>
    struct AutoIDAttacher
    {
        static const AutoID<BT> autoTypeId;
        AutoIDAttacher()
        {
            ( ( T* )this )->BT::typeId = AutoIDAttacher<T, BT>::autoTypeId.value;
        }
    };
    template<typename T, typename BT>
    const AutoID<BT> AutoIDAttacher<T, BT>::autoTypeId;




    // 池对象的 带版本控制需求的指针 容器
    // sample: 
    /*
    PoolPtr<Foo> foo;
    foo = xxxx;
    if( auto o = foo.Ptr() ) ...
    */
    template<typename T>
    struct PoolPtr
    {
        T* pointer = nullptr;
        int versionNumber = 0;

        PoolPtr() = default;
        PoolPtr( T const* const p )
            : pointer( p )
            , versionNumber( p->versionNumber )
        {
        }
        PoolPtr( PoolPtr const& o )
            : pointer( o.pointer )
            , versionNumber( o.versionNumber )
        {
        }
        inline PoolPtr& operator=( T const* const o )
        {
            pointer = o.pointer;
            versionNumber = o.versionNumber;
            return *this;
        }
        inline PoolPtr& operator=( PoolPtr const& o )
        {
            pointer = o.pointer;
            versionNumber = o.versionNumber;
            return *this;
        }
        // 没有数据管理需求, 不需要实现右值版复制构造

        inline void Ensure()
        {
            if( pointer->versionNumber != versionNumber )
                pointer = nullptr;
        }
        inline T* Ptr()
        {
            if( !pointer ) return nullptr;
            Ensure();
            return pointer;
        }
    };


    // 对象池，提供创建 / 分配 / 回收, 版本管理
    // T 的接口需求参看 Buf.h
    template<typename T>
    struct AutoIDPool
    {
        List<List<T*>> pool;
        int poolVersion = 0;

        AutoIDPool()
        {
            pool.Resize( AutoID<T>::maxValue + 1 );
        }
        ~AutoIDPool()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* Alloc( PTS&& ...ps )
        {
            CT* rtv;
#if __WIN
            auto& tid = CT::AutoIDAttacher<CT, T>::autoTypeId.value;
#else
            auto& tid = CT::template AutoIDAttacher<CT, T>::autoTypeId.value;
#endif
            auto& os = pool[ tid ];
            if( os.Size() )
                rtv = (CT*)os.TopPop();
            else
                rtv = new CT();
            rtv->T::poolVersion = ++poolVersion;
            rtv->Init( std::forward<PTS>( ps )... );
            return rtv;
        }

        void Free( T* o )
        {
            o->Destroy();
            o->poolVersion = 0;
            pool[ o->typeId ].Push( o );
        }

        template<typename CT>
        void Prepare( int count )
        {
#if __WIN
            auto& tid = CT::AutoIDAttacher<CT, T>::autoTypeId.value;
#else
            auto& tid = CT::template AutoIDAttacher<CT, T>::autoTypeId.value;
#endif
            auto& os = pool[ tid ];
            for( int i = 0; i < pool.Size(); ++i )
            {
                os.Push( new CT() );
            }
        }

        void Clear()
        {
            for( int i = 0; i < pool.Size(); ++i )
            {
                auto& os = pool[ i ];
                for( int j = 0; j < os.Size(); ++j )
                {
                    delete os[ j ];
                }
                os.Clear();
            }
        }
    };


}

#endif
