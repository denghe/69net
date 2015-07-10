#pragma once

namespace xxx
{
    class OldPool : Memmoveable
    {
    public:
        OldPool();     // need Init
        void Init( int _itemBufLen, int _pageBufLen = 4096, int capacity = 128, bool _attachThis = false );
        OldPool( int _itemBufLen, int _pageBufLen = 4096, int capacity = 128, bool _attachThis = false );
        ~OldPool();
        OldPool( OldPool&& o );
        OldPool& operator=( OldPool&& o );
        OldPool( OldPool const& o ) = delete;
        OldPool& operator=( OldPool const& o ) = delete;
        void* Alloc();
        void Free( void* p );
        void Clear();
        void Compress();    // free all no use page & item
        void Reserve();     // new 1 page
        void Reserve( int capacity );
        int Size() const;
        int PageCount() const;
        int PageBufLen() const;
        int ItemBufLen() const;
        bool AttachThis() const;
    private:
        List<void*> items;
        List<void*> pages;
        int itemBufLen;
        int pageBufLen;
        bool attachThis;
    };
}

