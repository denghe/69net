#ifndef __AUTOIDPOOL_H__
#define __AUTOIDPOOL_H__

namespace xxx
{
    /*
示例：


// 基类，必须具备下列接口要素
struct FooBase
{
    int typeId = 0;                     // 类型自增id. 将于派生类 new 时填充
    CorCore() = default;                // 默认构造函数
    virtual ~FooBase();                 // 虚析构，必须的
    // void Init( ... );                // 在派生类中实现. 想象为构造函数
    inline virtual void Destroy() {}    // 想象为析构函数
}


// 为了给派生类型附加上自增id, 实现一个模板以继承. 也可以复制代码或用宏啥的
template<typename T>
struct Foo : public FooBase
{
    static const AutoID<FooBase> typeId;
    Foo()
    {
        FooBase::typeId = this->typeId.value;
    }
};
template<typename T>
const AutoID<FooBase> Foo<T>::typeId;


// 这里约定，池对象每次初始化用 Init 函数，参数可变
struct Foo1 : Foo<Foo1> 
{
    void Init( ... );
}
struct Foo2 : Foo<Foo2>
{
    void Init( ... );
    void Destroy() override {}
}
...


BufPool<Foo> p;
auto f1 = p.Alloc<Foo1>( ... );
auto f2 = p.Alloc<Foo2>( ... );
...
p.Free( f1 );
p.Free( f2 );
    */


    // 作为静态成员置入 class, 提供自增 type id 的生成功能
    template<typename T>
    struct AutoID
    {
        int value;
        static int maxValue;
        AutoID() { value = maxValue++; }
    };
    template<typename T>
    int AutoID<T>::maxValue = 0;


    // T 的接口需求参看示例
    template<typename T>
    struct BufPool
    {
        List<List<T*>> data;

        BufPool()
        {
            data.Resize( AutoID<T>::maxValue + 1 );
        }
        ~BufPool()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* Alloc( PTS&& ...ps )
        {
            CT* rtv;
            auto& os = data[ CT::typeId.value ];
            if( os.Size() )
                rtv = (CT*)os.TopPop();
            else
                rtv = new CT();
            rtv->Init( std::forward<PTS>( ps )... );
            return rtv;
        }

        void Free( T* o )
        {
            o->Destroy();
            data[ o->typeId ].Push( o );
        }

        template<typename CT>
        void Prepare( int count )
        {
            auto& os = data[ CT::typeId.value ];
            for( int i = 0; i < data.Size(); ++i )
            {
                os.Push( new CT() );
            }
        }

        void Clear()
        {
            for( int i = 0; i < data.Size(); ++i )
            {
                auto& os = data[ i ];
                for( int j = 0; j < os.Size(); ++j )
                {
                    delete os[ j ];
                }
                os.Clear();
            }
        }
    };

}

#endif
