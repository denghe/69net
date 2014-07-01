#ifndef _STACK_H__
#define _STACK_H__

template<typename T>
class List
{
public:
    List( int capacity = 8 );
    ~List();
    List( List && other );
    List( List const& other );
    List& operator=( List && other );
    List& operator=( List const& other );
    template<typename VT>
    void push( VT&& v );
    void pop();
    T& top();
    void clear();
    void reserve( int capacity );
    T* data() const;
    int size() const;
    int byteSize() const;
    T const & operator[]( int idx ) const;
    T& operator[]( int idx );
    T const & at( int idx ) const;
    template<typename VT>
    void set( int idx, VT&& v );
private:
    char*       _buf;
    int         _size;
    int         _maxSize;  // new, reserve refresh
};

#include "List.hpp"


template<>
class List<bool>
{
public:
    List( int capacity = 512 );
    ~List();
    List( List<bool> && other );
    List( List<bool> const& other );
    List& operator=( List<bool> && other );
    List& operator=( List<bool> const& other );
    void push( bool v );
    void pop();
    bool top() const;
    void clear();
    void reserve( int capacity );
    char* data() const;
    int size() const;
    int byteSize() const;
    bool operator[]( int idx ) const;
    bool at( int idx ) const;
    void set( int idx, bool v );
private:
    char*       _buf;
    int         _size;
    int         _maxSize;  // new, reserve refresh
};



#endif
