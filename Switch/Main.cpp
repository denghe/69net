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
    SharedType<T>* _st;
    Shared()
        :_st( nullptr )
    {
    }
    void clear()
    {
        if( !_st ) return;
        if( _st->_copys > 1 )
        {
            --_st->_copys;
        }
        else
        {
            _st->ptr()->~T();
            if( !_st->_weaks )
            {
                _st->_deleter();
            }
            _st->_copys = 0;
        }
        _st = nullptr;
    }
    void assign( SharedType<T>* st, typename DT deleter = nullptr )
    {
        assert( st && st->_copys );
        clear();
        ++st->_copys;
        _st = st;
        if( deleter ) _st->_deleter = deleter;
        else _st->_deleter = [ st ] { delete st; };
    }
    Shared( SharedType<T>* st, typename DT deleter = nullptr )
    {
        _st = st;
        if( deleter ) _st->_deleter = deleter;
        else _st->_deleter = [ st ] { delete st; };
    }
    Shared( Shared& other )
    {
        if( !other._st || !other._st->_copys )
        {
            _st = nullptr;
        }
        else
        {
            _st = other._st;
            ++other._st->_copys;
        }
    }
    Shared( Shared&& other )
    {
        _st = other._st;
        other._st = nullptr;
    }
    Shared& operator=( Shared& other )
    {
        clear();
        if( !other._st || !other._st->_copys )
        {
            _st = nullptr;
        }
        else
        {
            _st = other._st;
            ++other._st->_copys;
        }
    }
    Shared& operator=( Shared&& other )
    {
        clear();
        _st = other._st;
        other._st = nullptr;
    }
    ~Shared()
    {
        clear();
    }
    T* ptr()
    {
        if( _st == nullptr || !_st->_copys ) return nullptr;
        return _st->ptr();
    }
    T* operator->( )
    {
        return ptr();
    }
};

template<typename T>
struct Weak
{
    SharedType<T>* _st;
    void clear()
    {
        if( !_st ) return;
        --_st->_weaks;
        if( !_st->_copys && !_st->_weaks )
        {
            _st->_deleter();
        }
        _st = nullptr;
    }
    Weak& operator=( SharedType<T>* st )
    {
        clear();
        if( st && st->_copys )
        {
            _st = st;
            ++st->_weaks;
        }
        else
        {
            _st = nullptr;
        }
        return *this;
    }
    Weak& operator=( Shared<T>& other )
    {
        return operator=( other._st );
    }
    Weak& operator=( Weak<T>& other )
    {
        return operator=( other._st );
    }
    Weak& operator=( Weak<T>&& other )
    {
        clear();
        _st = other._st;
        other._st = nullptr;
    }
    Weak()
        : _st( nullptr )
    {
    }
    Weak( Shared<T>& other )
    {
        if( other._st && other._st->_copys )
        {
            _st = other._st;
            ++other._st->_weaks;
        }
        else
        {
            _st = nullptr;
        }
    }
    Weak( Weak<T>& other )
    {
        if( other._st && other._st->_copys )
        {
            _st = other._st;
            ++other._st->_weaks;
        }
        else
        {
            _st = nullptr;
        }
    }
    Weak( Weak<T>&& other )
    {
        _st = other._st;
        other._st = nullptr;
    }
    Weak( SharedType<T>* st )
    {
        if( st && st->_copys )
        {
            _st = st;
            ++st->_weaks;
        }
        else
        {
            _st = nullptr;
        }
    }
    ~Weak()
    {
        clear();
    }
    T* ptr()
    {
        if( _st == nullptr || !_st->_copys ) return nullptr;
        return _st->ptr();
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
    std::vector<Shared<Foo>> childs;
};


int main()
{
    Pool fp( sizeof( SharedType<Foo> ) );
    Cout( fp.size() );
    {
        auto f = makeSharedEx<Foo>( &fp );
        f->xxx();
        Cout( fp.size() );
        Cout( f._st->_copys );
        {
            auto f2 = f;
            Cout( f._st->_copys );
        }
        Cout( f._st->_copys );

        Weak<Foo> w( f );
        Cout( w._st->_weaks );
        {
            auto w2 = w;
            Cout( w._st->_weaks );
        }
        Cout( w._st->_weaks );

        f->parent = w;
        f->parent = w;
        f->parent = w;
        Cout( w._st->_weaks );
        f->parent = nullptr;
        Cout( w._st->_weaks );

        f->childs.push_back( makeSharedEx<Foo>( &fp ) );
        f->childs.push_back( makeSharedEx<Foo>( &fp ) );
        f->childs.push_back( makeSharedEx<Foo>( &fp ) );
        f->childs.push_back( makeSharedEx<Foo>( &fp ) );

        f->childs[ 0 ]->parent = f->childs[ 1 ];
        f->childs[ 1 ]->parent = f->childs[ 0 ];
        f->childs[ 2 ]->parent = f->childs[ 1 ];
        Cout( fp.size() );
    }
    Cout( fp.size() );

    return 0;
}
