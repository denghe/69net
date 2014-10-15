#ifndef _OLDPOOL_H__
#define _OLDPOOL_H__

namespace xxx
{


    class OldPool : Memmoveable
    {
    public:
        OldPool();     // need Init
        void Init( int ItemBufLen, int pageBufLen = 4096, int capacity = 128, bool AttachThis = false );
        OldPool( int ItemBufLen, int pageBufLen = 4096, int capacity = 128, bool AttachThis = false );
        ~OldPool();
        OldPool( OldPool&& other );
        OldPool& operator=( OldPool&& other );
        OldPool( OldPool const& other ) = delete;
        OldPool& operator=( OldPool const& other ) = delete;
        void* Alloc();
        void Free( void* p );
        void Clear();
        void Compress();    // free all no use page & item
        void Reserve();     // new 1 page
        void Reserve( int capacity );
        int Size() const;
        int pageCount() const;
        int pageBufLen() const;
        int ItemBufLen() const;
        bool AttachThis() const;
    private:
        List<void*> _items;
        List<void*> _pages;
        int _itemBufLen;
        int _pageBufLen;
        bool _attachThis;
    };


}

#endif
