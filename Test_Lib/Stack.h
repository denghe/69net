#ifndef _STACK_H__
#define _STACK_H__

template<typename T>
class Stack
{
public:
    Stack( int capacity = 8 ); // count
    ~Stack();
    Stack( Stack const& other ) = delete;
    Stack& operator=( Stack const& other ) = delete;
    template<typename VT>
    void push( VT&& v );
    void pop();
    T& top();
    void clear();
    void reserve( int capacity );
    T* items() const;
    int count() const;
    T& operator[]( int idx ) const;
private:
    char*       _buf;
    int         _bufLen;
    int         _dataCount;
    // todo: disposer, incoming buffer
};

#include "Stack.hpp"

#endif
