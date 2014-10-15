#ifndef _SCOPEGUARD_H__
#define _SCOPEGUARD_H__

namespace xxx
{


    class ScopeGuard
    {
    public:
        typedef std::function<void()> FT;
        template<typename T>
        ScopeGuard( T&& f ) : _f( std::forward<T>( f ) ) {}
        ~ScopeGuard();
        void RunAndCancel();
        void Run();
        void Cancel();
        template<typename T>
        void Set( T&& f ) { _f = std::forward<T>( f ); }
    private:
        FT _f;
        ScopeGuard( const ScopeGuard & ) = delete;
        ScopeGuard &operator=( const ScopeGuard & ) = delete;
    };


}


#endif
