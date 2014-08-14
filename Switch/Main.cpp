#include "Lib/All.h"

template<typename T>
struct Share
{
    typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type _data;
    int _copys;
    int _weaks;
    typedef std::function<void()> DT;
    DT _deleter;

    template<typename... PTS>
    Share( PTS&&... ps )
        : _copys( 1 )
        , _weaks( 0 )
    {
        new ( (void *)&_data ) T( std::forward<PTS>( ps )... );
    }
    T* ptr()
    {
        return (T*)&_data;
    }
};

template<typename T>
struct Strong
{
    typedef typename Share<T>::DT DT;
    Share<T>* _p;
    Strong()
        :_p( nullptr )
    {
    }
    void clear()
    {
        if( !_p ) return;
        if( --_p->_copys == 0 )
        {
            _p->deleter();
        }
        _p = nullptr;
    }
    void assign( Share<T>* p, typename DT deleter = nullptr )
    {
        assert( p && !p->_copys );
        // todo: clear
        _p = p;
        if( deleter ) _p->_deleter = deleter;
        else _p->_deleter = [ p ] { delete p; };
    }
    Strong( Share<T>* p, typename DT deleter = nullptr )
    {
        assign( p, deleter );
    }
    Strong( Strong& other )
    {
        _p = other._p;
        ++other._p->_copys;
    }
    Strong( Strong&& other )
    {
        _p = other._p;
        other._p = nullptr;
    }
    Strong& operator=( Strong& other )
    {
        // todo: clear
    }
    Strong& operator=( Strong&& other )
    {
        // todo: clear
        _p = other._p;
        other._p = nullptr;
    }
    ~Strong()
    {
        if( !_p || --_p->_copys ) return;
        _p->ptr()->~T();
        if( !_p->_weaks )
        {
            _p->_deleter();
        }
        _p = nullptr;
    }
    T* ptr()
    {
        if( _p == nullptr || !_p->_copys ) return nullptr;
        return _p->ptr();
    }
};

template<typename T>
struct Weak
{
    Share<T>* _p;
    void clear()
    {
        if( !_p ) return;
        --_p->_weaks;
        _p = nullptr;
    }
    Weak& operator=( Share<T>* p )
    {
        // todo: clear();
        if( p && p->_copys )
        {
            _p = p;
            ++p->_weaks;
        }
        else
        {
            _p = nullptr;
        }
        return *this;
    }
    Weak& operator=( Strong<T>& p )
    {
        return operator=( p._p );
    }
    Weak& operator=( Weak<T>& p )
    {
        return operator=( p._p );
    }
    Weak& operator=( Weak<T>&& other )
    {
        _p = other._p;
        other._p = nullptr;
    }
    Weak( Strong<T>& s )
    {
        operator=( s );
    }
    Weak( Weak<T>& other )
    {
        operator=( other );
    }
    Weak( Weak<T>&& other )
    {
        _p = other._p;
        other._p = nullptr;
    }
    Weak( Share<T>* p )
    {
        operator=( p );
    }
    ~Weak()
    {
        if( !_p ) return;
        --_p->_weaks;
        if( !_p->_copys && !_p->_weaks )
        {
            _p->_deleter();
        }
        _p = nullptr;
    }
    T* ptr()
    {
        if( _p == nullptr || !_p->_copys ) return nullptr;
        return _p->ptr();
    }
};

template<typename T, typename... PTS>
Strong<T> makeStrong( PTS&&... ps )
{
    return Strong<T>( new Share<T>( std::forward<PTS>( ps )... ) );
}

template<typename T, typename... PTS>
Strong<T> makeStrongEx( void* addr, typename Share<T>::DT deleter, PTS&&... ps )
{
    auto p = new (addr)Share<T>( std::forward<PTS>( ps )... );
    return Strong<T>( p, deleter );
}

template<typename T, typename... PTS>
Strong<T> makeStrongEx( Pool* pool, PTS&&... ps )
{
    assert( pool->itemBufLen() >= sizeof( Share<T> ) );
    auto addr = pool->alloc();
    auto p = new (addr)Share<T>( std::forward<PTS>( ps )... );
    return Strong<T>( p, [ pool, addr ] { pool->free( addr ); } );
}

struct Foo
{
    Foo()
    {
        Cout( "Foo()" );
    }
    ~Foo()
    {
        Cout( "~Foo()" );
    }
};


int main()
{
    Pool fp( sizeof( Share<Foo> ) );
    Cout( p.size() );
    {
        auto addr = p.alloc();
        auto f = makeStrongEx<Foo>( &fp );
        Cout( p.size() );

        Weak<Foo> w( f );
        Cout( w._p->_weaks );
        auto w2 = w;
        Cout( w._p->_weaks );
    }
    Cout( p.size() );

    return 0;
}


//Strong<Foo> f( new SharedData<Foo>() );
//auto f2 = makeStrong<Foo>();
//ALIGN8( char buf[ sizeof( SharedData<Foo> ) ] );
//auto f3 = makeStrongEx<Foo>( buf, [] {} );


//struct ObjectContainer : public Dict < int, Object* >
//{
//    ~ObjectContainer()
//    {
//        if( empty() ) return;
//        List<int> keys( _nodes.size() );
//        for( auto i = 0; i < _nodes.size(); ++i )
//        {
//            keys.push( _nodes[ i ]->key );
//        }
//        for( auto i = 0; i < _nodes.size(); ++i )
//        {
//            at( keys[ i ] )->del();
//        }
//    }
//};
//
//int main()
//{
//    {
//        ObjectContainer oc;
//        auto f1 = Foo::create( oc );
//        f1->_friend = f1;
//        f1->dispose();
//    }
//
//    system( "pause" );
//    return 0;
//}
//
