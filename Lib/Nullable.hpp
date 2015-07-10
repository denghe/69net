#pragma once

namespace xxx
{

    template<typename T>
    Nullable<T>::Nullable()
        : hasValue( false )
    {
    }

    template<typename T>
    Nullable<T>::Nullable( T const& v )
        : hasValue( true )
    {
        new ( &value ) T( v );
    }

    template<typename T>
    Nullable<T>::Nullable( T&& v )
        : hasValue( true )
    {
        new ( &value ) T( std::move( v ) );
    }

    template<typename T>
    Nullable<T>::~Nullable()
    {
        if( hasValue )
        {
            ( (T*)( &value ) )->T::~T();
        }
    }

    template<typename T>
    Nullable<T>::Nullable( const Nullable& o )
        : hasValue( o.hasValue )
    {
        if( o.hasValue )
        {
            new ( &value ) T( *( (T*)( &o.value ) ) );
        }
    }

    template<typename T>
    Nullable<T>::Nullable( Nullable&& o )
        : hasValue( o.hasValue )
    {
        if( o.hasValue )
        {
            new ( &value ) T( std::move( *( (T*)( &o.value ) ) ) );
        }
    }

    template<typename T>
    Nullable<T>& Nullable<T>::operator=( Nullable&& o )
    {
        if( hasValue )
        {
            ( (T*)( &value ) )->T::~T();
        }
        hasValue = o.hasValue;
        if( o.hasValue )
        {
            new ( &value ) T( std::move( *( (T*)( &o.value ) ) ) );
        }
        return *this;
    }

    template<typename T>
    Nullable<T>& Nullable<T>::operator=( Nullable const& o )
    {
        if( hasValue )
        {
            ( (T*)( &value ) )->T::~T();
        }
        hasValue = o.hasValue;
        if( o.hasValue )
        {
            new ( &value ) T( *( (T*)( &o.value ) ) );
        }
        return *this;
    }

    template<typename T>
    template<typename ...PTS>
    void Nullable<T>::Emplace( PTS&& ...ps )
    {
        if( hasValue )
        {
            ( (T*)( &value ) )->T::~T();
        }
        else
        {
            hasValue = true;
        }
        new ( &value ) T( std::forward<PTS>( ps )... );
    }


    template<typename T>
    void Nullable<T>::Clear()
    {
        if( hasValue )
        {
            ( (T*)( &value ) )->T::~T();
            hasValue = false;
        }
    }

    template<typename T>
    Nullable<T>& Nullable<T>::operator=( T&& v )
    {
        Emplace( std::move( v ) );
        return *this;
    }

    template<typename T>
    Nullable<T>& Nullable<T>::operator=( T const& v )
    {
        Emplace( v );
        return *this;
    }

    template<typename T>
    bool Nullable<T>::operator!=( Nullable<T> const& o )
    {
        return !operator==( o );
    }

    template<typename T>
    bool Nullable<T>::operator<( Nullable<T> const& o ) const
    {
        if( hasValue )
        {
            if( o.hasValue )
            {
                return value < o.value;
            }
            return false;
        }
        return o.hasValue;
    }

    template<typename T>
    bool Nullable<T>::operator==( Nullable<T> const& o ) const
    {
        if( !hasValue && !o.hasValue )
        {
            return true;
        }
        if( hasValue && o.hasValue )
        {
            return value == o.value;
        }
        return false;
    }

    template<typename T>
    bool Nullable<T>::operator==( T const& v ) const
    {
        if( !hasValue )
        {
            return false;
        }
        return value == v;
    }

    template<typename T>
    bool Nullable<T>::HasValue() const
    {
        return hasValue;
    }

    //template<typename T>
    //Nullable<T>::operator bool() const
    //{
    //    return hasValue;
    //}

    template<typename T>
    T& Nullable<T>::Value()
    {
        assert( hasValue );
        return *( (T*)( &value ) );
    }

    template<typename T>
    T const& Nullable<T>::Value() const
    {
        assert( hasValue );
        return *( (T*)( &value ) );
    }


}
