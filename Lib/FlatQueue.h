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
        FlatQueue( FlatQueue const& other );
        FlatQueue( FlatQueue&& other );
        FlatQueue& operator=( FlatQueue const& other );
        FlatQueue& operator=( FlatQueue&& other );

        T const& operator[]( int idx ) const;
        T& operator[]( int idx );
        T const& At( int idx ) const;
        T& At( int idx );

        int Size() const;
        bool Empty() const;
        void Clear();
        void Reserve( int capacity, bool afterPush = false );

        void Push( T const& v );
        template<typename ...PTS>
        void Emplace( PTS&& ...ps );
        void Pop();
        T& Top();
        T const& Top() const;
        bool Pop( T& outVal );

    private:
        T* _buf;
        int _maxSize, _head, _tail;
    };


}

#endif
