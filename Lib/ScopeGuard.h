#ifndef _SCOPEGUARD_H__
#define _SCOPEGUARD_H__

class ScopeGuard
{
public:
    typedef std::function<void()> FT;
    template<typename T>
    ScopeGuard( T&& f ) : _f( std::forward<T>( f ) ) {}
    ~ScopeGuard();
    void runAndCancel();
    void run();
    void cancel();
    template<typename T>
    void set( T&& f ) { _f = std::forward<T>( f ); }
private:
    FT _f;
    ScopeGuard( const ScopeGuard & ) = delete;
    ScopeGuard &operator=( const ScopeGuard & ) = delete;
};

#endif
