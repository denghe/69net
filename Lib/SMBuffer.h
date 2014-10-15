#ifndef _SMBUFFER_H__
#define _SMBUFFER_H__

namespace xxx
{


    // 将结构体及内存全部构建在 SharedMemory 上的一个循环 buffer
    // 使用方法：直接将 SM 内存指针 硬转成这个类的指针
    // SM 创建者还须调用 Init 函数初始化
    // 仅限一读一写两个进程/线程同时使用
    class SMBuffer
    {
    public:
        void Init( int bufLen );
        int Write( char const* buf, int len );  // 返回实际写入长度
        int Read( char* buf, int len );         // 返回实际读取长度
        int Copy( char* buf, int len );         // 返回实际复制长度
        int Skip( int len );                    // 返回实际跳过长度
        void Clear();
        bool Empty() const;
        int Size() const;
        int Space() const;

    private:
        SMBuffer() = delete;
        SMBuffer( SMBuffer const& other ) = delete;
        SMBuffer& operator=( SMBuffer const& other ) = delete;

        std::atomic<int> _size;
        int _bufLen, _ro, _wo;
        char _buf[ 1 ];     // point to buffer space
    };


}

#endif
