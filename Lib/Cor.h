#ifndef __COR_H__
#define __COR_H__

namespace xxx
{

#define COR_BEGIN           if( corSleeps ) { return Sleeping(); }; \
                            switch( corLn ) { case 0:
#define COR_YIELD           corLn = __LINE__; return true; case __LINE__:
#define COR_END             } return false;
#define COR_SLEEP( N )      corSleeps = N - 1; COR_YIELD;


#define REF_DECL( T, N )    T* N = nullptr; int N##_corId = 0
#define REF_SET( N, P )     N = P; N##_corId = P->corId
#define REF_ENSURE( N )     if( N && N->corId != N##_corId ) N = nullptr
#define REF_CLEAR( N )      N = nullptr

    // todo: message ?

    template<typename T>
    struct AutoID
    {
        int value;
        static int maxValue;
        AutoID() { value = maxValue++; }
    };
    template<typename T>
    int AutoID<T>::maxValue = 0;


    template<typename T>
    struct CorManager
    {
        List<T*> items;
        List<List<T*>> pool;

        CorManager()
        {
            pool.Resize( AutoID<T>::maxValue + 1 );
        }

        int aiid = 0;
        template<typename CT, typename ...PTS>
        CT* CreateItem( PTS&& ..._parms )
        {
            CT* rtv;
            auto& objs = pool[ CT::typeId.value ];
            if( objs.Size() )
            {
                rtv = (CT*)objs.TopPop();
            }
            else
            {
                rtv = new CT();
                rtv->manager = this;
            }
            rtv->T::corId = ++aiid;
            rtv->Init( std::forward<PTS>( _parms )... );
            rtv->T::corIdx = items.Size();
            rtv->T::corLn = 0;
            rtv->T::corSleeps = 0;
            items.Push( rtv );
            return rtv;
        }

        void DestroyItem( T* _o )
        {
            items.Top()->corIdx = _o->corIdx;
            items.EraseFast( _o->corIdx );
            _o->Destroy();
            _o->corId = 0;
            pool[ _o->GetTypeId() ].Push( _o );
        }

        bool Process( int _ticks = 0 )
        {
            if( !items.Size() ) return false;
            for( int i = items.Size() - 1; i >= 0; --i )
            {
                auto& o = items[ i ];
                if( !o->Process( _ticks ) ) DestroyItem( o );
            }
            return true;
        }

        void Clear()
        {
            for( int i = items.Size() - 1; i >= 0; --i )
            {
                DestroyItem( items[ i ] );
            }
            aiid = 0;
        }

        void Compress()
        {
            for( int i = 0; i < items.Size(); ++i )
            {
                items[ i ]->EnsureRefs();
            }
            for( int i = pool.Size() - 1; i >= 0; --i )
            {
                auto& objs = pool[ i ];
                for( int j = 0; j < pool.Size(); ++j )
                {
                    delete objs[ j ];
                }
                pool.Clear();
            }
        }
    };

    struct CorCore
    {
        // void Init( ... );
        inline virtual void EnsureRefs() {}
        inline virtual void Destroy() {}
        virtual ~CorCore() {}

        int corId = 0, corIdx = 0, corLn = 0, corSleeps = 0;
        virtual bool Process( int ticks ) = 0;
        inline virtual bool Sleeping() { --corSleeps; return true; }
    };

    struct Cor : public CorCore
    {
        CorManager<Cor>* manager;

        // static const AutoID<Base1> typeId;
        // int GetTypeId() override { return typeId.value; }
        virtual int GetTypeId() = 0;
    };

    template<typename T>
    struct CorBase : public Cor
    {
        static const AutoID<Cor> typeId;
        int GetTypeId() override { return typeId.value; }
    };
    template<typename T>
    const AutoID<Cor> CorBase<T>::typeId;


}

#endif
