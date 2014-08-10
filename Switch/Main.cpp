#include "Lib/All.h"

struct Object
{
    void retain() { ++_refs; }
    void release() { --_refs; }
    virtual void dispose() 
    {
        if( !_refs )
        {
            del();
            return;
        }
        _disposed = true;
    }
    virtual void del() = 0;                         // free memory
protected:
    int _id;                                        // auto increase identity
    int _refs;                                      // weak reference counter
    bool _disposed;                                 // dead flag
    Object() : _refs( 0 ), _disposed( false )
    {
        static int id = 0;
        _id = id++;
    }
    virtual ~Object() {};
};

template<typename T>
struct Ref
{
    Ref( T* p = nullptr )
    {
        if( !p || p->_disposed )
        {
            _p = nullptr;
            return;
        }
        _p = p;
        p->retain();
    }
    ~Ref()
    {
        if( !_p ) return;
        _p->release();
        if( _p->_disposed && _p->_refs == 0 )
        {
            auto tmp = _p;
            _p = nullptr;
            tmp->del();     // 是否不应该调用这句？
        }
    }
    Ref( Ref&& other )
        : _p( other._p )
    {
        other._p = nullptr;
    }
    Ref( Ref const& other )
        : _p( other._p )
    {
        if( _p ) _p->retain();
    }
    Ref& operator=( T* p )
    {
        clear();
        _p = p;
        if( p ) p->retain();
        return *this;
    }
    Ref& operator=( Ref&& other )
    {
        clear();
        _p = other._p;
        other._p = nullptr;
    }
    Ref& operator=( Ref const& other )
    {
        clear();
        _p = other._p;
        if( _p ) _p->retain();
    }
    void clear()
    {
        if( !_p ) return;
        _p->release();
        if( _p->_disposed && _p->_refs == 0 )
        {
            _p->del();
        }
        _p = nullptr;
    }
    T* reset( T* p = nullptr )
    {
        clear();
        if( !p || p->_disposed ) return;
        _p = p;
        p->retain();
    }
    T* lock()
    {
        if( !_p ) return nullptr;
        if( _p->_disposed && _p->_refs == 0 )
        {
            _p->del();
            _p = nullptr;
            return nullptr;
        }
        return _p;
    }
    T* get()
    {
        return _p;
    }

protected:
    T* _p;
};


struct ObjectContainer : public Dict < int, Object* >
{
    ~ObjectContainer()
    {
        if( empty() ) return;
        List<int> keys( _nodes.size() );
        for( auto i = 0; i < _nodes.size(); ++i )
        {
            keys.push( _nodes[ i ]->key );
        }
        for( auto i = 0; i < _nodes.size(); ++i )
        {
            at( keys[ i ] )->del();
        }
    }
};

struct Foo : public Object
{
    Ref<Foo> _friend;
    virtual void dispose() override
    {
        _friend.clear();
        Object::dispose();
    }
    // blah blah blah

public:
    template<typename... PTS>
    static Foo* create( PTS&& ... ps )
    {
        return new Foo( std::forward<PTS>( ps )... );
    }
    friend Ref < Foo > ;
    friend ObjectContainer;
private:
    Dict<int, Object*>* _oc;
    virtual void del() override
    {
        _oc->erase( _id );
        delete this;
    }
    Foo( ObjectContainer& c )
    {
        _oc = &c;
        c.insert( _id, this );
        // blah blah blah
    }
    ~Foo()
    {
    }
};

int main()
{
    {
        ObjectContainer oc;
        auto f1 = Foo::create( oc );
        f1->_friend = f1;
        //f1->dispose();
    }

    //auto f1 = new ( p.alloc() ) Foo();
    //objs.insert( f1->_id, f1 );

    //auto f2 = new ( p.alloc() ) Foo();
    //objs.insert( f2->_id, f2 );

    //f2->retain();
    //f1->_friend = f2;

    //f1->retain();
    //f2->_friend = f1;

    //f2->dispose();


    system( "pause" );
    return 0;
}

