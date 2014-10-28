#ifndef __SINGLETON_H__
#define __SINGLETON_H__


namespace xxx
{

    template<typename T>
    struct Singleton
    {
    protected:
        static T* instance;

        Singleton() = default;
        Singleton( const Singleton & ) = delete;
        Singleton &operator =( const Singleton & ) = delete;

    public:
        static T* GetInstance()
        {
            return instance;
        }
        template<typename ...PTS>
        static void InitInstance( PTS && ... ps )
        {
            assert( !instance );
            instance = new T( std::forward<PTS>( ps )... );
        }
        static void DestroyInstance()
        {
            assert( instance );
            delete instance;
            instance = nullptr;
        }
    };
    template<typename T>
    T* Singleton<T>::instance = nullptr;
}


#endif
