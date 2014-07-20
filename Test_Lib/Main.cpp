#include "Lib/All.h"
template<typename ...TS>
void cout( TS const& ...parms )
{
    String s;
    s.append( parms... );
    std::cout << s.c_str() << std::endl;
}



template <class T>
class FlatQueue
{
public:
    FlatQueue();
    ~FlatQueue();
    FlatQueue( FlatQueue const& other );
    FlatQueue( FlatQueue&& other );
    bool operator=( FlatQueue const& other );
    bool operator=( FlatQueue&& other );

    T const& operator[]( int idx ) const;
    T& operator[]( int idx );
    T const& at( int idx ) const;
    T& at( int idx );

    void insertAt( int idx, T const& v );
    void erase( int idx );
    int size() const;
    bool empty() const;
    void clear();
    void compress();
    bool find( T const& q );
    void reserve( int size );

    void push( T const& v );
    template<typename ...PTS>
    void emplace( PTS ...ps );
    void pop();
    T& top();
    T const& top() const;

private:
    List<char[ sizeof( T ) ]> _data;
    int _head, _tail;
};


template <class T>
int FlatQueue<T>::size() const
{
    if( _head <= _tail )
        return _tail - _head;
    else
        return _data.size() - _head + _tail;
}

template <class T>
bool FlatQueue<T>::empty() const
{
    return _head == _tail;
}

template <class T>
FlatQueue<T>::FlatQueue()
    : _head( 0 )
    , _tail( 0 )
{
}

template <class T>
FlatQueue<T>::~FlatQueue()
{
    while( !empty() ) pop();
}

template <class T>
void FlatQueue<T>::pop()
{
#ifdef __DEBUG
    assert( _head != _tail );
#endif
    if( ++_head == _data.size() )
    {
        (*(T*)_data[ _data.size() - 1 ]).~T();
        _head = 0;
    }
    (*(T*)_data[ _head - 1 ]).~T();
}

template <class T>
T const& FlatQueue<T>::top() const
{
#ifdef __DEBUG
    assert( _head != _tail );
#endif
    if( _head ) return *(T*)_data[ _head - 1 ];
    return *(T*)_data[ _data.size() - 1 ];
}

//
//template <class T>
//void FlatQueue<T>::insertAt( int idx, T const& v )
//{
//    assert( idx <= size() );
//
//    // Just force a reallocation, will be overwritten
//    push( v );
//
//    if( size() == 1 )
//        return;
//
//    int writeIndex, readIndex, trueWriteIndex, trueReadIndex;
//    writeIndex = size() - 1;
//    readIndex = writeIndex - 1;
//    while( readIndex >= idx )
//    {
//        if( _head + writeIndex >= _data.size() )
//            trueWriteIndex = _head + writeIndex - _data.size();
//        else
//            trueWriteIndex = _head + writeIndex;
//
//        if( _head + readIndex >= _data.size() )
//            trueReadIndex = _head + readIndex - _data.size();
//        else
//            trueReadIndex = _head + readIndex;
//
//        array[ trueWriteIndex ] = array[ trueReadIndex ];
//
//        if( readIndex == 0 )
//            break;
//        writeIndex--;
//        readIndex--;
//    }
//
//    if( _head + idx >= _data.size() )
//        trueWriteIndex = _head + idx - _data.size();
//    else
//        trueWriteIndex = _head + idx;
//
//    array[ trueWriteIndex ] = v;
//}

//
//template <class T>
//T FlatQueue<T>::Peek() const
//{
//#ifdef _DEBUG
//    assert( _head != _tail );
//#endif
//
//    return *(T*)_data[ _head ];
//}
//
//template <class T>
//T FlatQueue<T>::PeekTail() const
//{
//#ifdef _DEBUG
//    assert( _head != _tail );
//#endif
//    if( _tail != 0 )
//        return *(T*)_data[ _tail - 1 ];
//    else
//        return *(T*)_data[ _data.size() - 1 ];
//}

template <class T>
void FlatQueue<T>::push( T const& v )
{
    if( _data.size() == 0 )
    {
        array = RakNet::OP_NEW_ARRAY<T>( 16 );
        _head = 0;
        _tail = 1;
        array[ 0 ] = v;
        _data.size() = 16;
        return;
    }

    array[ _tail++ ] = v;

    if( _tail == _data.size() )
        _tail = 0;

    if( _tail == _head )
    {
        //  int idx=tail;

        // Need to allocate more memory.
        T * new_array;
        new_array = RakNet::OP_NEW_ARRAY<T>( (int)_data.size() * 2 );
#ifdef _DEBUG
        assert( new_array );
#endif
        if( new_array == 0 )
            return;

        for( int counter = 0; counter < _data.size(); ++counter )
            new_array[ counter ] = array[ ( _head + counter ) % ( _data.size() ) ];

        _head = 0;

        _tail = _data.size();

        _data.size() *= 2;

        // Delete the old array and move the pointer to the new array
        RakNet::OP_DELETE_ARRAY( array );

        array = new_array;
    }

}

template <class T>
FlatQueue<T>::FlatQueue( FlatQueue const& other )
{
    // Allocate memory for copy

    if( other.Size() == 0 )
    {
        _data.size() = 0;
    }

    else
    {
        array = RakNet::OP_NEW_ARRAY<T >( other.Size() + 1, _FILE_AND_LINE_ );

        for( int counter = 0; counter < other.Size(); ++counter )
            array[ counter ] = other.array[ ( other.head + counter ) % ( other._data.size() ) ];

        _head = 0;

        _tail = other.Size();

        _data.size() = other.Size() + 1;
    }
}

template <class T>
bool FlatQueue<T>::operator= ( FlatQueue const& other )
{
    if( ( &other ) == this )
        return false;

    clear( _FILE_AND_LINE_ );

    // Allocate memory for copy
    if( other.Size() == 0 )
    {
        _data.size() = 0;
    }

    else
    {
        array = RakNet::OP_NEW_ARRAY<T >( other.Size() + 1, _FILE_AND_LINE_ );

        for( int counter = 0; counter < other.Size(); ++counter )
            array[ counter ] = other.array[ ( other.head + counter ) % ( other._data.size() ) ];

        _head = 0;

        _tail = other.Size();

        _data.size() = other.Size() + 1;
    }

    return true;
}

template <class T>
void FlatQueue<T>::clear()
{
    if( _data.size() == 0 )
        return;

    if( _data.size() > 32 )
    {
        RakNet::OP_DELETE_ARRAY( array );
        _data.size() = 0;
    }

    _head = 0;
    _tail = 0;
}

template <class T>
void FlatQueue<T>::compress()
{
    T* new_array;
    int newAllocationSize;
    if( _data.size() == 0 )
        return;

    newAllocationSize = 1;
    while( newAllocationSize <= size() )
        newAllocationSize <<= 1; // Must be a better way to do this but I'm too dumb to figure it out quickly :)

    new_array = RakNet::OP_NEW_ARRAY<T >( newAllocationSize );

    for( int counter = 0; counter < size(); ++counter )
        new_array[ counter ] = array[ ( _head + counter ) % ( _data.size() ) ];

    _tail = size();
    _data.size() = newAllocationSize;
    _head = 0;

    // Delete the old array and move the pointer to the new array
    RakNet::OP_DELETE_ARRAY( array );
    array = new_array;
}

template <class T>
bool FlatQueue<T>::find( const T &q )
{
    if( _data.size() == 0 )
        return false;

    int counter = _head;

    while( counter != _tail )
    {
        if( array[ counter ] == q )
            return true;

        counter = ( counter + 1 ) % _data.size();
    }

    return false;
}

template <class T>
void FlatQueue<T>::reserve( int size )
{
    RakNet::OP_DELETE_ARRAY( array );
    if( size > 0 )
        array = RakNet::OP_NEW_ARRAY<T>( size );
    else
        array = 0;
    _data.size() = size;
    _head = 0;
    _tail = 0;
}

template <class T>
T const& FlatQueue<T>::operator[] ( int idx ) const
{
#ifdef _DEBUG
    assert( idx < size() );
#endif
    if( _head + idx >= _data.size() )
        return array[ _head + idx - _data.size() ];
    else
        return array[ _head + idx ];
}

template <class T>
void FlatQueue<T>::erase( int idx )
{
#ifdef _DEBUG
    assert( idx < size() );
    assert( _head != _tail );
#endif

    if( _head == _tail || idx >= size() )
        return;

    int idx;

    int next;

    if( _head + idx >= _data.size() )
        idx = _head + idx - _data.size();
    else
        idx = _head + idx;

    next = idx + 1;

    if( next == _data.size() )
        next = 0;

    while( next != _tail )
    {
        // Overwrite the previous element
        array[ idx ] = array[ next ];
        idx = next;

        if( ++next == _data.size() )
            next = 0;
    }

    // Move the tail back
    if( _tail == 0 )
        _tail = _data.size() - 1;
    else
        --_tail;
}



int main()
{
    return 0;
}
