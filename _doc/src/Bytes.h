#ifndef _BYTES_H_
#define _BYTES_H_



// todo: 支持指针对象的序列化，支持 std 智能指针, 支持 nullptr 
// todo: 特化 Ref<T>




template<typename T>
struct List;

const int DefaultBytesBufferSize = 32;

struct Bytes
{
    static Bytes const defaultValue;   // 默认值静态对象 用于初始化一些列表成员

    int         _readIdx;
    int         _len;                   // 同时也是 write Index
    int         _bufSize;
    char *      _buf;

    explicit Bytes( int bufSize = DefaultBytesBufferSize );
    Bytes( Bytes const & o );
    Bytes( Bytes && o );
    ~Bytes();
    Bytes & operator=( Bytes const & o );
    Bytes & operator=( Bytes && o );

    void        clearReadIndex();               // 清 读 指针
    void        clearLength();                  // 清 写 指针
    void        clear();                        // 清 读 + 写 指针
    void        clear( int bufSize );           // 清 读 + 写 指针 并确保 bufSize 足够长（不会减肥）

    void        ensure( int len );              // 确保 wfree >= len ( 可能会触发扩容操作 )
                                        
    int         getFreeWriteSpace() const;      // 返回可写区间字节数 ( _bufSize - _len )
    int         getFreeReadSpace() const;       // 返回剩余可读字节数 ( _len - _readIdx )
                                        
    void        dump( String & s ) const;       // 将数据导出为文本
    void        toString( String & s ) const;   // 将数据导出为文本( 纯数据 16进制 序列 )

    void        readDirect7Bit( uint64 & v );
    void        readDirect7Bit( uint32 & v );
    ReadStatus  read7Bit( uint64 & v );
    ReadStatus  read7Bit( uint32 & v );

    // direct: 直接读写系列，将没有任何越界和失败判断（主用于一些内部代码或生成代码调用）
    // 

    // direct write
    void        writeDirect( uint64 v );
    void        writeDirect( uint32 v );
    void        writeDirect( uint16 v );
    void        writeDirect( uint8  v );
    void        writeDirect( int64  v );
    void        writeDirect( int32  v );
    void        writeDirect( int16  v );
    void        writeDirect( int8   v );
    void        writeDirect( bool   v );
    void        writeDirect( float  v );
    void        writeDirect( double v );
    void        writeDirect( const char * v, int len );
    void        writeDirect( const char * v );
    void        writeDirect( String const & v );
    void        writeDirect( DateTime const & v );
    void        writeDirect( Bytes const & v );

    // direct read
    void        readDirect( uint64 & v );
    void        readDirect( uint32 & v );
    void        readDirect( uint16 & v );
    void        readDirect( uint8  & v );
    void        readDirect( int64  & v );
    void        readDirect( int32  & v );
    void        readDirect( int16  & v );
    void        readDirect( int8   & v );
    void        readDirect( bool   & v );
    void        readDirect( float  & v );
    void        readDirect( double & v );
    void        readDirect( String & v );
    void        readDirect( Bytes & v );
    void        readDirect( DateTime & v );

    // 安全读写系列
    //

    // 一般写入。过程中会检查是否越界
    void        write( uint64 v );
    void        write( uint32 v );
    void        write( uint16 v );
    void        write( uint8  v );
    void        write( int64  v );
    void        write( int32  v );
    void        write( int16  v );
    void        write( int8   v );
    void        write( bool   v );
    void        write( float  v );
    void        write( double v );
    void        write( const char * v, int len );
    void        write( const char * v );
    void        write( String const & v );
    void        write( Bytes const & v );
    void        write( DateTime const & v );

    // 一般读取。会返回操作结果并通过参数填充读出的数值
    ReadStatus  read( uint64 & v );
    ReadStatus  read( uint32 & v );
    ReadStatus  read( uint16 & v );
    ReadStatus  read( uint8  & v );
    ReadStatus  read( int64  & v );
    ReadStatus  read( int32  & v );
    ReadStatus  read( int16  & v );
    ReadStatus  read( int8   & v );
    ReadStatus  read( bool   & v );
    ReadStatus  read( float  & v );
    ReadStatus  read( double & v );
    ReadStatus  read( String & v );
    ReadStatus  read( Bytes & v );
    ReadStatus  read( DateTime & v );


    // 这个等同于 w 但可连写
    template<typename T>
    Bytes & operator<<( T const & v );

    // 这个等同于 r。不可连写
    template<typename T>
    ReadStatus operator>>( T & v );;

    
    // 扩展部分，令 Bytes 支持扩展数据操作
    /*

    需要实现下列函数：

    void bytesW( Bytes & buf );
    ReadStatus bytesR( Bytes & buf );
    void bytesDW( Bytes & buf );
    void bytesDR( Bytes & buf );

    */

    template<typename T>
    void write( T const & v );

    template<typename T>
    ReadStatus read( T & v );

    template<typename T>
    void writeDirect( T const & v );

    template<typename T>
    void readDirect( T & v );




    // 扩展部分
    // 

    // 令 Bytes 支持容器操作
    //

    // List 支持
    //

    // direct write 系列
    void        writeDirect( List<uint8 > const & vs );
    void        writeDirect( List<int8  > const & vs );
    void        writeDirect( List<bool  > const & vs );
    void        writeDirect( List<float > const & vs );
    void        writeDirect( List<double> const & vs );
    void        writeDirect( List<DateTime> const & vs );
    template<typename T>
    void        writeDirect( List<T> const & vs );

    // direct read 系列
    void        readDirect( List<uint8 > & vs );
    void        readDirect( List<int8  > & vs );
    void        readDirect( List<bool  > & vs );
    void        readDirect( List<float > & vs );
    void        readDirect( List<double> & vs );
    void        readDirect( List<DateTime> & vs );
    template<typename T>
    void        readDirect( List<T> & vs );

    // write 系列
    void        write( List<uint8 > const & vs );
    void        write( List<int8  > const & vs );
    void        write( List<bool  > const & vs );
    void        write( List<float > const & vs );
    void        write( List<double> const & vs );
    void        write( List<DateTime> const & vs );
    template<typename T>
    void write( List<T> const & vs );

    // read 系列
    ReadStatus  read( List<uint8 > & vs );
    ReadStatus  read( List<int8  > & vs );
    ReadStatus  read( List<bool  > & vs );
    ReadStatus  read( List<float > & vs );
    ReadStatus  read( List<double> & vs );
    ReadStatus  read( List<DateTime> & vs );
    template<typename T>
    ReadStatus  read( List<T> & vs );




    template<typename T>
    void write( std::unique_ptr<T> const & v );

    template<typename T>
    ReadStatus read( std::unique_ptr<T> & v );
};


#endif
