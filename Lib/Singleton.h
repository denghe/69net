#pragma once

namespace xxx
{
#ifdef SINGLETON_DISABLE_AUTO_DESTROY
    template<typename T>
    class Singleton
    {
    protected:
        static T* instance;
        Singleton() = default;
        Singleton( const Singleton & ) = delete;
        Singleton &operator =( const Singleton & ) = delete;
    public:
        static T* GetInstance()
        {
            assert( instance );
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
#else
    template<typename T>
    class Singleton
    {
    protected:
        static std::unique_ptr<T> instance;
        Singleton() = default;
        Singleton( const Singleton& ) = delete;
        Singleton &operator =( const Singleton& ) = delete;
    public:
        static T* GetInstance()
        {
            assert( instance );
            return instance.get();
        }
        template<typename ...PTS>
        static void InitInstance( PTS&& ... ps )
        {
            assert( !instance );
            instance.reset( new T( std::forward<PTS>( ps )... ) );
        }
        static void DestroyInstance()
        {
            assert( instance );
            instance.reset();
        }
    };
    template<typename T>
    std::unique_ptr<T> Singleton<T>::instance;
#endif
}
