#ifndef __COR_H__
#define __COR_H__

#define COR_BEGIN       if( sleeps ) { return Sleeping(); }; \
                        switch( ln ) { case 0:
#define COR_YIELD       ln = __LINE__; return true; case __LINE__:
#define COR_END         } return false;
#define COR_SLEEP( N )  sleeps = N - 1; COR_YIELD;


#define REF_DECL( T, N ) T* N = nullptr; int N##Id = 0
#define REF_SET( N, P )  N = P; N##Id = P->id
#define REF_ENSURE( N )  if( N && N->id != N##Id ) N = nullptr
#define REF_CLEAR( N )   N = nullptr

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
        pool.resize( AutoID<T>::maxValue + 1 );
    }

    int aiid = 0;
    template<typename T, typename ...PTS>
    T* CreateItem( PTS ...parms )
    {
        static_assert( std::is_base_of<T, T>::value, "T must be inherit from FooBase*" );
        T* rtv;
        auto& objs = pool[ T::typeId.value ];
        if( objs.size() )
        {
            rtv = (T*)objs.top_pop();
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

    void DestroyItem( T* foo )
    {
        items.top()->idx = foo->idx;
        items.eraseFast( foo->idx );
        foo->Destroy();
        foo->id = 0;
        pool[ foo->GetTypeId() ].push( foo );
    }

    bool Process( int ticks = 0 )
    {
        if( !items.size() ) return false;
        for( int i = items.size() - 1; i >= 0; --i )
        {
            auto& foo = items[ i ];
            if( !foo->Process( ticks ) ) DestroyItem( foo );
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
        for( int i = 0; i < items.size(); ++i )
        {
            items[ i ]->EnsureRefs();
        }
        for( int i = pool.size() - 1; i >= 0; --i )
        {
            auto& objs = pool[ i ];
            for( int j = 0; j < pool.size(); ++j )
            {
                delete objs[ j ];
            }
            pool.clear();
        }
    }
};



struct Cor
{
    CorManager<Cor>* manager;

    // static const AutoID<Base1> typeId;
    // int GetTypeId() override { return typeId.value; }
    virtual int GetTypeId() = 0;

    // void Init( ... );
    inline virtual void EnsureRefs() {}
    inline virtual void Destroy() {}
    virtual ~Cor() {}

    int id = 0, idx = 0, ln = 0, sleeps = 0;
    virtual bool Process( int ticks ) = 0;
    inline virtual bool Sleeping() { --sleeps; return true; }
};



template<typename T>
struct CorBase : public Cor
{
    static const AutoID<Cor> typeId;
    int GetTypeId() override { return typeId.value; }
};
template<typename T>
const AutoID<Cor> CorBase<T>::typeId;


#endif
