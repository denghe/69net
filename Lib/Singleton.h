#ifndef __SINGLETON_H__
#define __SINGLETON_H__


namespace xxx
{
    //template<typename T>
    //struct Singleton
    //{
    //protected:
    //    static T* instance;

    //    Singleton() = default;
    //    Singleton( const Singleton & ) = delete;
    //    Singleton &operator =( const Singleton & ) = delete;

    //public:
    //    static T* GetInstance()
    //    {
    //        return instance;
    //    }
    //    template<typename ...PTS>
    //    static void InitInstance( PTS && ... ps )
    //    {
    //        assert( !instance );
    //        instance = new T( std::forward<PTS>( ps )... );
    //    }
    //    static void DestroyInstance()
    //    {
    //        assert( instance );
    //        delete instance;
    //        instance = nullptr;
    //    }
    //};
    //template<typename T>
    //T* Singleton<T>::instance = nullptr;


    // 这是会在程序退出时自动析构的实现( 用 unique_ptr 包了一下 instance )
    template<typename T>
    struct Singleton
    {
    protected:
        static std::unique_ptr<T>& GetInstanceCore()
        {
            static std::unique_ptr<T> instance;
            return instance;
        }

        Singleton() = default;
        Singleton( const Singleton& ) = delete;
        Singleton &operator =( const Singleton& ) = delete;

    public:
        static T* GetInstance()
        {
            assert( GetInstanceCore() != nullptr );
            return GetInstanceCore().get();
        }
        template<typename ...PTS>
        static void InitInstance( PTS&& ... ps )
        {
            assert( GetInstanceCore() == nullptr );
            GetInstanceCore().reset( new T( std::forward<PTS>( ps )... ) ); // todo: new T{} 这样写可兼容初始化列表 ?
        }
        static void DestroyInstance()
        {
            assert( GetInstanceCore() != nullptr );
            GetInstanceCore().reset();
        }
    };
}


#endif
