#ifndef __REF_H__
#define __REF_H__

namespace xxx
{
    class Ref
    {
    public:
        void Retain();
        void Release();
        int refCount = 1;

        void AutoRelease();
        static void ReleasePool();
#ifdef USE_STL
        static vector<Ref*> autoReleasePool;
#else
        static List<Ref*> autoReleasePool;
#endif
    protected:
        Ref();
        virtual ~Ref();

    };

    template<typename T, typename ...PTS>
    inline static T* Create( PTS&& ...ps )
    {
        static_assert( is_base_of<Ref, T>::value, "the T must inherit from Ref." );
        auto rtv = new T( std::forward<PTS>( ps )... );
        rtv->AutoRelease();
        return rtv;
    }

}

#endif
