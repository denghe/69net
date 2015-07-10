#pragma once

namespace xxx
{
    template<typename T>
    struct Nullable
    {
        Nullable();
        ~Nullable();

        Nullable( T const& v );
        Nullable( T&& v );
        Nullable( const Nullable& o );
        Nullable( Nullable&& o );

        Nullable& operator=( Nullable&& o );
        Nullable& operator=( Nullable const& o );

        template<typename ...PTS>
        void Emplace( PTS&& ...ps );

        void Clear();

        Nullable& operator=( T&& v );
        Nullable& operator=( T const& v );

        bool operator==( Nullable<T> const& o ) const;
        bool operator<( Nullable<T> const& o ) const;
        bool operator!=( Nullable<T> const& o );

        bool operator==( T const& v ) const;

        bool HasValue() const;
        // explicit operator bool() const;
        T& Value();
        T const& Value() const;

    protected:
        typename std::aligned_storage<sizeof( T ), std::alignment_of<T>::value>::type value;
        bool hasValue;
    };

}
