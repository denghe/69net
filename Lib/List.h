#ifndef _LIST_H__
#define _LIST_H__

namespace xxx
{
    class ByteBuffer;

    template<typename T>
    class List : Memmoveable
    {
    public:
        List( std::initializer_list<T> items );
        List( int capacity = 8 );
        ~List();
        List( List&& o );
        List( List const& o );
        List& operator=( List&& o );
        List& operator=( List const& o );
        template<typename VT>
        void Push( VT&& v );
        template<typename VT>
        void PushFast( VT&& v );        // unsafe: do not check space enough
        template<typename ...PTS>
        T& Emplace( PTS&& ...ps );
        template<typename ...PTS>
        T& EmplaceAt( int idx, PTS&& ...ps );
        template<typename VT>
        void InsertAt( int idx, VT&& v );
        int Find( T const& v ) const;
        void Erase( int idx );
        void EraseFast( int idx );
        void Pop();
        T& Top();
        T const& Top() const;
        T& TopPop();
        void Clear();
        void Reserve( int capacity );
        void Resize( int capacity, bool init = true );
        T* Data() const;
        int Size() const;
        int& Size();
        int ByteSize() const;
        int ByteMaxSize() const;
        T const & operator[]( int idx ) const;
        T& operator[]( int idx );
        T const & At( int idx ) const;
        T& At( int idx );
        template<typename VT>
        void Set( int idx, VT&& v );

        // ByteBuffer interface
        void WriteTo( ByteBuffer& bb ) const;
        void FastWriteTo( ByteBuffer& bb ) const;
        bool ReadFrom( ByteBuffer& bb );
    private:
        T*          buf;
        int         size;
        int         maxSize;
    };



    // 2进制排列为 从右到左，由低到高位，即小尾机可 memcpy
    template<>
    class List<bool> : Memmoveable
    {
    public:
        List( int capacity = 512 );
        ~List();
        List( List<bool>&& o );
        List( List<bool> const& o );
        List& operator=( List<bool>&& o );
        List& operator=( List<bool> const& o );
        void Push( bool bit );
        void Pop();
        bool Top() const;
        void Clear();
        void Reserve( int capacity );
        char* Data() const;
        int Size() const;
        int ByteSize() const;
        bool operator[]( int idx ) const;
        bool At( int idx ) const;
        void Set( int idx, bool bit );
        void SetTrue( int idx );
        void SetFalse( int idx );
        void FillTrue();
        void FillFalse();
        void Fill( bool bit, int idxFrom = 0, int idxTo = 0 );
        void Resize( int capacity, bool init = true );
        void CleanUp() const;   // clear last byte's unused bits

        // ByteBuffer interface
        void WriteTo( ByteBuffer& bb ) const;
        void FastWriteTo( ByteBuffer& bb ) const;
        bool ReadFrom( ByteBuffer& bb );
    private:
        char*       buf;
        int         size;
        int         maxSize;
    };


}

#endif
