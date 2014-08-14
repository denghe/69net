#include "Lib/All.h"

template<typename T>
struct SharedType
{
    typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type _data;
    int _copys;
    int _weaks;
    typedef std::function<void()> DT;
    DT _deleter;

    template<typename... PTS>
    SharedType( PTS&&... ps )
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
struct Shared
{
    typedef typename SharedType<T>::DT DT;
    SharedType<T>* _p;
    Shared()
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
    void assign( SharedType<T>* p, typename DT deleter = nullptr )
    {
        assert( p && p->_copys );
        clear();
        ++p->_copys;
        _p = p;
        if( deleter ) _p->_deleter = deleter;
        else _p->_deleter = [ p ] { delete p; };
    }
    Shared( SharedType<T>* p, typename DT deleter = nullptr )
    {
        _p = p;
        if( deleter ) _p->_deleter = deleter;
        else _p->_deleter = [ p ] { delete p; };
    }
    Shared( Shared& other )
    {
        if( !other._p || !other._p->_copys )
        {
            _p = nullptr;
        }
        else
        {
            _p = other._p;
            ++other._p->_copys;
        }
    }
    Shared( Shared&& other )
    {
        _p = other._p;
        other._p = nullptr;
    }
    Shared& operator=( Shared& other )
    {
        clear();
        if( !other._p || !other._p->_copys )
        {
            _p = nullptr;
        }
        else
        {
            _p = other._p;
            ++other._p->_copys;
        }
    }
    Shared& operator=( Shared&& other )
    {
        clear();
        _p = other._p;
        other._p = nullptr;
    }
    ~Shared()
    {
        if( !_p ) return;
        if( _p->_copys > 1 )
        {
            --_p->_copys;
            return;
        }
        else
        {
            _p->ptr()->~T();
            if( !_p->_weaks )
            {
                _p->_deleter();
            }
            _p = nullptr;
        }
    }
    T* ptr()
    {
        if( _p == nullptr || !_p->_copys ) return nullptr;
        return _p->ptr();
    }
    T* operator->( )
    {
        return ptr();
    }
};

template<typename T>
struct Weak
{
    SharedType<T>* _p;
    void clear()
    {
        if( !_p ) return;
        --_p->_weaks;
        _p = nullptr;
    }
    Weak& operator=( SharedType<T>* p )
    {
        clear();
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
    Weak& operator=( Shared<T>& p )
    {
        return operator=( p._p );
    }
    Weak& operator=( Weak<T>& p )
    {
        return operator=( p._p );
    }
    Weak& operator=( Weak<T>&& other )
    {
        clear();
        _p = other._p;
        other._p = nullptr;
    }
    Weak()
        : _p( nullptr )
    {
    }
    Weak( Shared<T>& other )
    {
        if( other._p && other._p->_copys )
        {
            _p = other._p;
            ++other._p->_weaks;
        }
        else
        {
            _p = nullptr;
        }
    }
    Weak( Weak<T>& other )
    {
        if( other._p && other._p->_copys )
        {
            _p = other._p;
            ++other._p->_weaks;
        }
        else
        {
            _p = nullptr;
        }
    }
    Weak( Weak<T>&& other )
    {
        _p = other._p;
        other._p = nullptr;
    }
    Weak( SharedType<T>* p )
    {
        if( p && p->_copys )
        {
            _p = p;
            ++p->_weaks;
        }
        else
        {
            _p = nullptr;
        }
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
Shared<T> makeShared( PTS&&... ps )
{
    return Shared<T>( new SharedType<T>( std::forward<PTS>( ps )... ) );
}

template<typename T, typename... PTS>
Shared<T> makeSharedEx( void* addr, typename SharedType<T>::DT deleter, PTS&&... ps )
{
    auto p = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
    return Shared<T>( p, deleter );
}

template<typename T, typename... PTS>
Shared<T> makeSharedEx( Pool* pool, PTS&&... ps )
{
    assert( pool->itemBufLen() >= sizeof( SharedType<T> ) );
    auto addr = pool->alloc();
    auto p = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
    return Shared<T>( p, [ pool, addr ] { pool->free( addr ); } );
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
    void xxx()
    {
        Cout( "xxx" );
    }

    Weak<Foo> parent;
};


int main()
{
    Pool fp( sizeof( SharedType<Foo> ) );
    Cout( fp.size() );
    {
        auto f = makeSharedEx<Foo>( &fp );
        //f->xxx();
        //Cout( fp.size() );
        //Cout( f._p->_copys );
        //{
        //    auto f2 = f;
        //    Cout( f._p->_copys );
        //}
        //Cout( f._p->_copys );

        Weak<Foo> w( f );
        //Cout( w._p->_weaks );
        //{
        //    auto w2 = w;
        //    Cout( w._p->_weaks );
        //}
        Cout( w._p->_weaks );

        f->parent = w;
        Cout( w._p->_weaks );
    }
    Cout( fp.size() );

    return 0;
}
