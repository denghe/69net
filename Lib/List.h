#ifndef _LIST_H__
#define _LIST_H__

class FlatBuffer;

template<typename T>
class List
{
public:
    List( int capacity = 8 );
    ~List();
    List( List&& other );
    List( List const& other );
    List& operator=( List&& other );
    List& operator=( List const& other );
    template<typename VT>
    void push( VT&& v );
    void pop();
    T& top();
    void clear();
    void reserve( int capacity );
    void resize( int capacity, bool init = true );
    T* data() const;
    int size() const;
    int& size();
    int byteSize() const;
    int byteMaxSize() const;
    T const & operator[]( int idx ) const;
    T& operator[]( int idx );
    T const & at( int idx ) const;
    T& at( int idx );
    template<typename VT>
    void set( int idx, VT&& v );

    // for FlatBuffer write
    int getBufferSize() const;
    void writeBuffer( FlatBuffer& fb ) const;
    void writeBufferDirect( FlatBuffer& fb ) const;

private:
    T*          _buf;
    int         _size;
    int         _maxSize;  // new, reserve refresh
};



template<>
class List < bool >
{
public:
    List( int capacity = 512 );
    ~List();
    List( List<bool>&& other );
    List( List<bool> const& other );
    List& operator=( List<bool>&& other );
    List& operator=( List<bool> const& other );
    void push( bool v );
    void pop();
    bool top() const;
    void clear();
    void reserve( int capacity );
    // todo: void resize( int capacity, bool init = true );
    char* data() const;
    int size() const;
    int byteSize() const;
    // todo: int byteMaxSize() const;
    bool operator[]( int idx ) const;
    bool at( int idx ) const;
    void set( int idx, bool v );

    // todo: for FlatBuffer write
private:
    char*       _buf;
    int         _size;
    int         _maxSize;  // new, reserve refresh
};



#endif
