#ifndef _LIST_H__
#define _LIST_H__

class FlatBuffer;

template<typename T>
class List : Memmoveable
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
    template<typename ...PTS>
    T& emplace( PTS&& ...ps );
    template<typename ...PTS>
    T& emplaceAt( int idx, PTS&& ...ps );
    template<typename VT>
    void insertAt( int idx, VT&& v );
    int find( T const& v );
    void erase( int idx );
    void eraseFast( int idx );
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
    bool readBuffer( FlatBuffer& fb );


private:
    T*          _buf;
    int         _size;
    int         _maxSize;
};




template<>
class List<bool> : Memmoveable
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
    char* data() const;
    int size() const;
    int byteSize() const;
    bool operator[]( int idx ) const;
    bool at( int idx ) const;
    void set( int idx, bool v );
    // todo: more functions
private:
    char*       _buf;
    int         _size;
    int         _maxSize;
};



#endif
