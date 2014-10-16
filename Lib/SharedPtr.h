#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

namespace xxx
{


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
        //typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type data;
        char data[ sizeof( T ) ];
        int copys;
        int weaks;
        typedef std::function<void()> DT;
        DT deleter;

        template<typename... PTS>
        SharedType( PTS&&... ps )
            : copys( 1 )
            , weaks( 0 )
        {
            auto p = new ( (void *)&data ) T( std::forward<PTS>( ps )... );
            SaveWeak( p );
        }
        T* Ptr()
        {
            return (T*)&data;
        }

        template<typename OT>
        typename std::enable_if<!std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type SaveWeak( OT* o ) {}
        template<typename OT>
        typename std::enable_if<std::is_base_of<EnableSharedFromThis<OT>, OT>::value, void>::type SaveWeak( OT* o )
        {
            o->weak = this;
        }
    };

    template<typename T>
    struct Shared
    {
        typedef Shared<T> MT;
        typedef typename SharedType<T>::DT DT;
        SharedType<T>* st;
        Shared()
            :st( nullptr )
        {
        }
        void Clear()
        {
            if( !st ) return;
            if( st->copys > 1 )
            {
                --st->copys;
            }
            else // copys <= 1
            {
                st->Ptr()->~T();
                st->copys = 0;
                if( !st->weaks )
                {
                    st->deleter();
                }
            }
            st = nullptr;
        }
        void Assign( SharedType<T>* _st, DT _deleter = nullptr )
        {
            assert( _st && _st->copys );
            Clear();
            ++_st->copys;
            st = _st;
            if( _deleter ) st->deleter = _deleter;
            else st->deleter = [ _st ] { delete _st; };
        }
        Shared( SharedType<T>* _st, DT _deleter = nullptr )
        {
            st = _st;
            if( _deleter ) st->deleter = _deleter;
            else st->deleter = [ _st ] { delete _st; };
        }
        Shared( MT const& o )
        {
            if( !o.st || !o.st->copys )
            {
                st = nullptr;
            }
            else
            {
                st = o.st;
                ++o.st->copys;
            }
        }
        Shared( MT&& o )
        {
            st = o.st;
            o.st = nullptr;
        }
        Shared& operator=( SharedType<T>* _st )
        {
            Clear();
            if( !_st || !_st->copys )
            {
                st = nullptr;
            }
            else
            {
                st = _st;
                ++_st->copys;
            }
        }
        Shared& operator=( MT const& o )
        {
            return operator=( o.st );
        }
        Shared& operator=( MT&& o )
        {
            Clear();
            st = o.st;
            o.st = nullptr;
        }

        ~Shared()
        {
            Clear();
        }
        T* Ptr()
        {
            if( st == nullptr || !st->copys ) return nullptr;
            return st->Ptr();
        }
        T* operator->( )
        {
            return Ptr();
        }
    };

    template<typename T>
    struct Weak
    {
        typedef Weak<T> MT;
        SharedType<T>* st;
        void Clear()
        {
            if( !st ) return;
            --st->weaks;
            if( !st->copys && !st->weaks )
            {
                st->deleter();
            }
            st = nullptr;
        }
        Weak& operator=( SharedType<T>* _st )
        {
            Clear();
            if( _st && _st->copys )
            {
                st = _st;
                ++_st->weaks;
            }
            else
            {
                st = nullptr;
            }
            return *this;
        }
        Weak& operator=( Shared<T> const& o )
        {
            return operator=( o.st );
        }
        Weak& operator=( MT const& o )
        {
            return operator=( o.st );
        }
        Weak& operator=( MT&& o )
        {
            Clear();
            st = o.st;
            o.st = nullptr;
        }
        Weak()
            : st( nullptr )
        {
        }
        Weak( Shared<T> const& o )
        {
            if( o.st && o.st->copys )
            {
                st = o.st;
                ++o.st->weaks;
            }
            else
            {
                st = nullptr;
            }
        }
        Weak( MT const& o )
        {
            if( o.st && o.st->copys )
            {
                st = o.st;
                ++o.st->weaks;
            }
            else
            {
                st = nullptr;
            }
        }
        Weak( MT&& o )
        {
            st = o.st;
            o.st = nullptr;
        }
        Weak( SharedType<T>* _st )
        {
            if( _st && _st->copys )
            {
                st = _st;
                ++_st->weaks;
            }
            else
            {
                st = nullptr;
            }
        }
        ~Weak()
        {
            Clear();
        }
        T* Ptr()
        {
            if( st == nullptr || !st->copys ) return nullptr;
            return st->Ptr();
        }
        Shared<T> Share()
        {
            Shared<T> rtv;
            if( !st ) return rtv;
            rtv.st = st;
            ++st->copys;
            return rtv;
        }
    };


    template<typename T>
    struct EnableSharedFromThis
    {
        Weak<T> weak;
        Shared<T> sharedFromThis()
        {
            return weak.Share();
        }
        friend SharedType < T > ;
    };

    template<typename T, typename... PTS>
    Shared<T> MakeShared( PTS&&... ps )
    {
        return new SharedType<T>( std::forward<PTS>( ps )... );
    }

    template<typename T, typename... PTS>
    Shared<T> MakeSharedEx( void* addr, typename SharedType<T>::DT _deleter, PTS&&... ps )
    {
        auto _st = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
        return Shared<T>( _st, _deleter );
    }

    template<typename T, typename... PTS>
    Shared<T> MakeSharedEx( Pool* pool, PTS&&... ps )
    {
        assert( pool->ItemBufLen() >= sizeof( SharedType<T> ) );
        auto addr = pool->Alloc();
        auto _st = new (addr)SharedType<T>( std::forward<PTS>( ps )... );
        return Shared<T>( _st, [ pool, addr ] { pool->Free( addr ); } );
    }

}

#endif
