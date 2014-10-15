#ifndef _CIRCLEBUFFER_H__
#define _CIRCLEBUFFER_H__

namespace xxx
{


    // 块式扩容的先进先出循环读写流( 主用于发数据啥的 )
    class BlockBuffer : Memmoveable
    {
    public:
        explicit BlockBuffer( Pool& p );
        ~BlockBuffer();
        void Write( char const* buf, int len );
        int Read( char* buf, int len );             // 试复制指定长度到 buf 并移动读指针. 返回实际复制的长度
        void Copy( char* buf, int len );            // 复制指定长度到 buf, 如果 size 不足会出错
        void Clear();
        bool Empty() const;
        int Size() const;                           // 未读之数据长
    private:
        BlockBuffer( BlockBuffer const& other ) = delete;
        BlockBuffer& operator=( BlockBuffer const& other ) = delete;
        struct Page
        {
            Page*   next;
            char    data[ 1 ];
        };
        Page*       _wp;                            // 正用于写的页
        Page*       _rp;                            // 正用于读的页
        int         _wpLen;                         // 正用于写的页的已写长度
        int         _rpLen;                         // 正用于读的页的已读长度
        int         _size;                          // 正读页剩余长度 + 正写页已写长度 + 中间跨过的页数 * 页size
        Pool*       _pool;
    };


}

#endif
