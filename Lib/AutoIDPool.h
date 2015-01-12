#ifndef __AUTOIDPOOL_H__
#define __AUTOIDPOOL_H__

namespace xxx
{
    // 示例参看 Buf.h

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


    // sample: struct Foo : public XxxBase, public AutoIDAttacher<Foo, XxxBase>
    // struct Foo_Child : public Foo, public AutoIDAttacher<Foo_Child, XxxBase>
    template<typename T, typename BT>
    struct AutoIDAttacher
    {
        static const AutoID<BT> autoTypeId;
        AutoIDAttacher()
        {
            ( ( T* )this )->BT::typeId = AutoIDAttacher<T, BT>::autoTypeId.value;
        }
    };
    template<typename T, typename BT>
    const AutoID<BT> AutoIDAttacher<T, BT>::autoTypeId;



    // T 的接口需求参看示例
    template<typename T>
    struct AutoIDPool
    {
        List<List<T*>> data;

        AutoIDPool()
        {
            data.Resize( AutoID<T>::maxValue + 1 );
        }
        ~AutoIDPool()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* Alloc( PTS&& ...ps )
        {
            CT* rtv;
            auto& os = data[ CT::AutoIDAttacher<CT, T>::autoTypeId.value ];
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
            auto& os = data[ CT::::AutoIDAttacher<CT, T>autoTypeId.value ];
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
