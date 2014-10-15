namespace xxx
{


    template<typename T>
    void SharedMemory::SetPrefixName( T&& s )
    {
        _prefixName = std::forward<T>( s );
    }


}