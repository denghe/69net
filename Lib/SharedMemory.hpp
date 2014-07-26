template<typename T>
void SharedMemory::setPrefixName( T&& s )
{
    _prefixName = std::forward<T>( s );
}
