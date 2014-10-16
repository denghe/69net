#ifndef _LIST_H__
#define _LIST_H__

namespace xxx
{


    class FlatBuffer;

    template<typename T>
    class List : Memmoveable
    {
    public:
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
        int Find( T const& v );
        void Erase( int idx );
        void EraseFast( int idx );
        void Pop();
        T& Top();
        T const& Top() const;
        T& TopPop();
        T const& TopPop() const;
        void Clear();
        void Reserve( int capacity );
        void Resize( int capacity, bool Init = true );
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

        // for FlatBuffer Write
        int GetWriteBufferSize() const;
        void WriteBuffer( FlatBuffer& fb ) const;
        void WriteBufferDirect( FlatBuffer& fb ) const;
        bool ReadBuffer( FlatBuffer& fb );


    private:
        T*          buf;
        int         size;
        int         maxSize;
    };




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
        void Push( bool v );
        void Pop();
        bool Top() const;
        void Clear();
        void Reserve( int capacity );
        char* Data() const;
        int Size() const;
        int ByteSize() const;
        bool operator[]( int idx ) const;
        bool At( int idx ) const;
        void Set( int idx, bool v );
        // todo: more functions
    private:
        char*       buf;
        int         size;
        int         maxSize;
    };


}

#endif
