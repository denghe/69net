namespace xxx
{


    template<typename T>
    void SharedMemory::SetPrefixName( T&& s )
    {
        prefixName = std::forward<T>( s );
    }


}