#ifndef _FLATQUEUE_H__
#define _FLATQUEUE_H__

namespace xxx
{

    template <class T>
    class FlatQueue
    {
    public:
        FlatQueue( int capacity = 8 );
        ~FlatQueue();
        FlatQueue( FlatQueue const& o );
        FlatQueue( FlatQueue&& o );
        FlatQueue& operator=( FlatQueue const& o );
        FlatQueue& operator=( FlatQueue&& o );

        T const& operator[]( int idx ) const;
        T& operator[]( int idx );
        T const& At( int idx ) const;
        T& At( int idx );

        int Size() const;
        bool Empty() const;
        void Clear();
        void Reserve( int capacity, bool afterPush = false );

        template<typename VT>
        void Push( VT&& v );
        template<typename ...PTS>
        void Emplace( PTS&& ...ps );
        void Pop();
        T& Top();
        T const& Top() const;
        bool Pop( T& outVal );

    private:
        T* buf;
        int maxSize, head, tail;
    };


}

#endif
