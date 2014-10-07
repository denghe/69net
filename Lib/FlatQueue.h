#ifndef _FLATQUEUE_H__
#define _FLATQUEUE_H__

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
    T const& at( int idx ) const;
    T& at( int idx );

    int size() const;
    bool empty() const;
    void clear();
    void reserve( int capacity, bool afterPush = false );

    void push( T const& v );
    template<typename ...PTS>
    void emplace( PTS&& ...ps );
    void pop();
    T& top();
    T const& top() const;
    bool pop( T& outVal );

private:
    T* _buf;
    int _maxSize, _head, _tail;
};

#endif
