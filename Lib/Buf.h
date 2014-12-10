#ifndef __BUF_H__
#define __BUF_H__

namespace xxx
{
    struct BufBase
    {
        // bufs: 到达这个 ticks 将 Destroy
        // dots: 到达这个 ticks 将 Process
        int activeTicks = 0;

        // 类型自增编号 for 定位pool ( 于 new 时填充 )
        int typeId = 0;

        // 位于容器中时的位置 for 速删
        int bufIdx = 0;

        // 不应放置逻辑相关代码
        BufBase() = default;

        // 生命周期 -- 初始化/ 创建( 派生类自己实现 )
        // void Init( ... );

        // 生命周期 -- 执行
        inline virtual bool Process( int ticks ) = 0;

        // 生命周期 -- 销毁
        inline virtual void Destroy() {}

        // 不应放置逻辑相关代码
        virtual ~BufBase() {};
    };

    template<typename T>
    struct Buf : public BufBase
    {
        static const AutoID<BufBase> typeId;
        Buf()
        {
            this->BufBase::typeId = Buf::typeId.value;
        }
    };
    template<typename T>
    const AutoID<BufBase> Buf<T>::typeId;


    struct BufContainer
    {
        // 指向公用的 buf 池
        BufPool<BufBase>* pool;

        // 存 buf/dot 最小的死亡 ticks for 分时段批量跳过 Process 的过程
        int bufActiveTicks;
        int dotActiveTicks;

        // bufsDirty 为 true 时将 call 它再 遍历处理 bufs
        std::function<void()> BeforeBufProcess;

        // bufsDirty 为 true 时 遍历处理 bufs 后将 call 它
        std::function<void()> AfterBufProcess;

        // 持续生效的，以改 Properties buf 系数为主的 buf
        // 当成员构成( 有增删 )变化( lazyDirty 为 true )时，遍历处理
        List<BufBase*> bufs;

        // 周期性生效的，以 call 宿主函数( 形成指令流, Process 结束后依次执行 ) 为主的( dot 系 )
        // 每次 Process 都遍历处理
        List<BufBase*> dots;

        BufContainer( BufPool<BufBase>* pool )
            : pool( pool )
            , bufActiveTicks( INT_MAX )
            , dotActiveTicks( INT_MAX )
        {
        }
        ~BufContainer()
        {
            Clear();
        }

        template<typename T, typename ...PTS>
        inline T* CreateBuf( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufActiveTicks = 0;     // 相当于标脏，全面重算
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = dots.Size();
            dots.Push( rtv );
            if( rtv->activeTicks < dotActiveTicks )
            {
                dotActiveTicks = rtv->activeTicks;
            }
            return rtv;
        }

        inline void DestroyBuf( BufBase* buf )
        {
            bufs.Top()->bufIdx = buf->bufIdx;
            bufs.EraseFast( buf->bufIdx );
            pool->Free( buf );
            bufActiveTicks = 0;     // 相当于标脏，全面重算
        }

        inline void DestroyDot( BufBase* buf )
        {
            dots.Top()->bufIdx = buf->bufIdx;
            dots.EraseFast( buf->bufIdx );
            pool->Free( buf );
        }

        inline void Clear()
        {
            // 有可能存在清不完的情况，比如 Destroy 的时候又创建了新的
            while( bufs.Size() )
            {
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    DestroyBuf( bufs[ i ] );
                }
            }
            while( dots.Size() )
            {
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    DestroyDot( dots[ i ] );
                }
            }
            bufActiveTicks = INT_MAX;
            dotActiveTicks = INT_MAX;
        }

        inline bool Process( int ticks = 0 )
        {
            if( bufActiveTicks <= ticks )
            {
                bufActiveTicks = INT_MAX;
                if( BeforeBufProcess )
                {
                    BeforeBufProcess();
                }
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    auto& o = bufs[ i ];
                    if( !o->Process( ticks ) || o->activeTicks <= ticks )
                    {
                        DestroyBuf( o );
                    }
                    else
                    {
                        // 只扫出还活着的 buf 的下次执行 ticks
                        if( o->activeTicks < bufActiveTicks )
                        {
                            bufActiveTicks = o->activeTicks;
                        }
                    }
                }
                if( AfterBufProcess )
                {
                    AfterBufProcess();
                }
            }

            if( dotActiveTicks <= ticks )
            {
                dotActiveTicks = INT_MAX;
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    auto& o = dots[ i ];
                    // 试扫出下次执行的最小间隔
                    // 如果被杀掉的刚好是最小间隔 也没关系 不严格, 到那个 ticks 会再扫出正确值
                    // 感觉略冗余，但省掉了 存放 或立即扫出 最小间隔的消耗
                    if( o->activeTicks < dotActiveTicks )
                    {
                        dotActiveTicks = o->activeTicks;
                    }
                    if( o->activeTicks <= ticks )
                    {
                        if( !o->Process( ticks ) )
                        {
                            DestroyDot( o );
                        }
                    }
                }
                // 当前算法：每到一个执行点，就硬扫，适合对象数较少的情况( 阈值不明确，猜测是 10 个以内 )。
                // 如果元素再多点，则需要在扫的同时将下次执行点的 dot 放队列了
                // 如果又继续扫到更小的间隔，则将先前的队列中的元素清了，再放入队列
            }

            return true;
        }
    };

}

#endif
