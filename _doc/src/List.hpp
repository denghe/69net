#ifndef _LIST_HPP_
#define _LIST_HPP_

template<typename T>
List<T>::List()
{
    this->_len = 0;
    this->_bufSize = ListDefaultBufferSize;
    this->_buf = (T*)malloc( ListDefaultBufferSize );
}

template<typename T>
List<T>::List( int capacity )
{
    assert( capacity >= 1 );
    this->_len = 0;
    this->_bufSize = round2n( capacity * sizeof(T) );
    this->_buf = (T*)malloc( this->_bufSize );
}

template<typename T>
List<T>::List( List const & o )
{
    this->_bufSize = o._bufSize;
    this->_len = o._len;
    this->_buf = (T*)malloc( this->_bufSize );
    if( isValueType<T>() )
    {
        memcpy( this->_buf, o._buf, o._len * sizeof(T) );
    }
    else
    {
        for( int i = 0; i < o._len; ++i )
        {
            new ( &this->_buf[ i ] ) T( o._buf[ i ] );
        }
    }
}

template<typename T>
List<T>::List( List && o )
{
    this->_buf = o._buf;
    this->_bufSize = o._bufSize;
    this->_len = o._len;
    o._buf = nullptr;
}

template<typename T>
List<T>::~List()
{
    if( this->_buf )
    {
        if( !isValueType<T>() )
        {
            for( int i = 0; i < this->_len; ++i )
            {
                this->_buf[ i ].~T();
            }
        }
        free( this->_buf );
    }
}


template<typename T>
void List<T>::clear()
{
    if( !isValueType<T>() )
    {
        for( int i = 0; i < this->_len; ++i )
        {
            this->_buf[ i ].~T();
        }
    }
    this->_len = 0;
}

template<typename T>
int List<T>::getFreeSpace()
{
    return this->_bufSize / sizeof(T) - this->_len;
}


template<typename T>
void List<T>::ensure( int len )
{
    if( getFreeSpace() < len )
    {
        this->_bufSize = (int)round2n( ( this->_len + len ) * sizeof(T) );
        if( isValueType<T>() )
        {
            this->_buf = (decltype(this->_buf))realloc( this->_buf, this->_bufSize );
        }
        else
        {
            auto bak = this->_buf;
            this->_buf = (T*)malloc( this->_bufSize );
            for( int i = 0; i < this->_len; ++i )
            {
                new ( &this->_buf[ i ] ) T( (T&&)bak[ i ] );
                bak[ i ].~T();
            }
            free( bak );
        }
    }
}

template<typename T>
template<typename TV>
void List<T>::add( TV && v )
{
    ensure( 1 );
    addDirect( std::forward<TV>(v) );
}

template<typename T>
template<typename TV>
void List<T>::addDirect( TV && v )
{
    if( isValueType<TV>() )
    {
        this->_buf[ this->_len++ ] = std::forward<TV>(v); 
    }
    else
    {
        new ( &this->_buf[ this->_len++ ] ) T( std::forward<TV>(v) );
    }
}

template<typename T>
template<typename TV>
List<T> & List<T>::operator<<( TV && v )
{
    add( std::forward<TV>(v) );
    return *this;
}

template<typename T>
T const & List<T>::get( int idx ) const
{
    return this->_buf[ idx ];
}

template<typename T>
void List<T>::set( int idx, T const & v )
{
    this->_buf[ idx ] = v;
}

template<typename T>
T & List<T>::operator[]( int idx )
{
    return this->_buf[ idx ];
}

template<typename T>
T const & List<T>::operator[]( int idx ) const
{
    return this->_buf[ idx ];
}

template<typename T>
void List<T>::toString( String & s ) const
{
    s << "{ ";
    for( int i = 0; i < this->_len; ++i )
    {
        if( i ) s << ", ";
        s << (*this)[ i ];
    }
    s << " }";
}





#endif



