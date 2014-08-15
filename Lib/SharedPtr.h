#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

template<typename T>
struct Weak;

template<typename T>
struct Shared;

template<typename T>
struct SharedType;

template<typename T>
struct EnableSharedFromThis;

template<typename T>
struct SharedType
{
    //typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type _data;
    char _data[ sizeof( T ) ];
    int _copys;
    int _weaks;
    typedef std::function<void()> DT;
    DT _deleter;

    template<typename... PTS>
    SharedType( PTS&&... ps )
        : _copys( 1 )
        , _weaks( 0 )
    {
        auto p = new ( (void *)&_data ) T( std::forward<PTS>( ps )... );
        saveWeak( p );
    }
    T* ptr()
    {
        return (T*)&_data;
    }

    template<typename OT>
    typename std::enable_if<!std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type saveWeak( OT* o ) {}
    template<typename OT>
    typename std::enable_if<std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type saveWeak( OT* o )
    {
        o->_weak = this;
    }
};

template<typename T>
struct Shared
{
    typedef Shared<T> MT;
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
        else // _copys <= 1
        {
            _st->ptr()->~T();
            _st->_copys = 0;
            if( !_st->_weaks )
            {
                _st->_deleter();
            }
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
    Shared( MT const& other )
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
    Shared( MT&& other )
    {
        _st = other._st;
        other._st = nullptr;
    }
    Shared& operator=( SharedType<T>* st )
    {
        clear();
        if( !st || !st->_copys )
        {
            _st = nullptr;
        }
        else
        {
            _st = st;
            ++st->_copys;
        }
    }
    Shared& operator=( MT const& other )
    {
        return operator=( other._st );
    }
    Shared& operator=( MT&& other )
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
    typedef Weak<T> MT;
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
    Weak& operator=( Shared<T> const& other )
    {
        return operator=( other._st );
    }
    Weak& operator=( MT const& other )
    {
        return operator=( other._st );
    }
    Weak& operator=( MT&& other )
    {
        clear();
        _st = other._st;
        other._st = nullptr;
    }
    Weak()
        : _st( nullptr )
    {
    }
    Weak( Shared<T> const& other )
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
    Weak( MT const& other )
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
    Weak( MT&& other )
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
    Shared<T> share()
    {
        Shared<T> rtv;
        if( !_st ) return rtv;
        rtv._st = _st;
        ++_st->_copys;
        return rtv;
    }
};


template<typename T>
struct EnableSharedFromThis
{
    Weak<T> _weak;
    Shared<T> sharedFromThis()
    {
        return _weak.share();
    }
    friend SharedType<T>;
};

template<typename T, typename... PTS>
Shared<T> makeShared( PTS&&... ps )
{
    return new SharedType<T>( std::forward<PTS>( ps )... );
}

template<typename T, typename... PTS>
Shared<T> makeSharedEx( void* addr, typename SharedType<T>::DT deleter, PTS&&... ps )
{
    auto st = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
    return Shared<T>( st, deleter );
}

template<typename T, typename... PTS>
Shared<T> makeSharedEx( Pool* pool, PTS&&... ps )
{
    assert( pool->itemBufLen() >= sizeof( SharedType<T> ) );
    auto addr = pool->alloc();
    auto st = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
    return Shared<T>( st, [ pool, addr ] { pool->free( addr ); } );
}

#endif
