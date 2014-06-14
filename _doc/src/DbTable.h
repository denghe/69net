#ifndef _DBTABLE_H_
#define _DBTABLE_H_

// 当前所支持的数据类型
enum class DbTypes : uint8
{
    Boolean ,
    Int8    ,
    Int16   ,
    Int32   ,
    Int64   ,
    UInt8   ,
    UInt16  ,
    UInt32  ,
    UInt64  ,
    Float   ,
    Double  ,
    DateTime,
    String  ,
    Binary  ,
};

struct DbRow;
struct DbColumn;
struct DbValue;

struct DbTable
{
    DbTable();
    ~DbTable();

    void                    toString( String & s ) const; // dump

    void                    clearData();    // 只清数据行
    void                    clear();        // 清所有（包括字段也会清）
    DbTable &               addColumn( String const & colName, DbTypes dataType, bool nullable = false );
    int                     getColumnIndex( String const & colName );   // 根据字段名找索引

    DbRow                   newRow();       // 将创建一行全是默认值的数据，可空字段为空值
    DbRow                   operator[]( int rowIdx );
    DbRow                   get( int rowIdx );
    DbValue                 get( int rowIdx, int colIdx );

    bool                    isNull( int rowIdx, int colIdx ) const;
    void                    setNull( int rowIdx, int colIdx );

    bool                    getBoolean ( int rowIdx, int colIdx ) const;
    int8     const &        getInt8    ( int rowIdx, int colIdx ) const;
    int16    const &        getInt16   ( int rowIdx, int colIdx ) const;
    int32    const &        getInt32   ( int rowIdx, int colIdx ) const;
    int64    const &        getInt64   ( int rowIdx, int colIdx ) const;
    uint8    const &        getUInt8   ( int rowIdx, int colIdx ) const;
    uint16   const &        getUInt16  ( int rowIdx, int colIdx ) const;
    uint32   const &        getUInt32  ( int rowIdx, int colIdx ) const;
    uint64   const &        getUInt64  ( int rowIdx, int colIdx ) const;
    float    const &        getFloat   ( int rowIdx, int colIdx ) const;
    double   const &        getDouble  ( int rowIdx, int colIdx ) const;
    DateTime const &        getDateTime( int rowIdx, int colIdx ) const;
    String   const &        getString  ( int rowIdx, int colIdx ) const;
    Binary   const &        getBinary  ( int rowIdx, int colIdx ) const;

    void                    setBoolean ( int rowIdx, int colIdx, bool     const & v );
    void                    setInt8    ( int rowIdx, int colIdx, int8     const & v );
    void                    setInt16   ( int rowIdx, int colIdx, int16    const & v );
    void                    setInt32   ( int rowIdx, int colIdx, int32    const & v );
    void                    setInt64   ( int rowIdx, int colIdx, int64    const & v );
    void                    setUInt8   ( int rowIdx, int colIdx, uint8    const & v );
    void                    setUInt16  ( int rowIdx, int colIdx, uint16   const & v );
    void                    setUInt32  ( int rowIdx, int colIdx, uint32   const & v );
    void                    setUInt64  ( int rowIdx, int colIdx, uint64   const & v );
    void                    setFloat   ( int rowIdx, int colIdx, float    const & v );
    void                    setDouble  ( int rowIdx, int colIdx, double   const & v );
    void                    setDateTime( int rowIdx, int colIdx, DateTime const & v );
    void                    setString  ( int rowIdx, int colIdx, String   const & v );
    void                    setBinary  ( int rowIdx, int colIdx, Binary   const & v );
    void                    setString  ( int rowIdx, int colIdx, String && v );
    void                    setBinary  ( int rowIdx, int colIdx, Binary && v );
    void                    setString  ( int rowIdx, int colIdx, char const * v );

    // 直接引用到值所在地址
    // refBoolean 无法实现故不提供
    int8     &              refInt8    ( int rowIdx, int colIdx );
    int16    &              refInt16   ( int rowIdx, int colIdx );
    int32    &              refInt32   ( int rowIdx, int colIdx );
    int64    &              refInt64   ( int rowIdx, int colIdx );
    uint8    &              refUInt8   ( int rowIdx, int colIdx );
    uint16   &              refUInt16  ( int rowIdx, int colIdx );
    uint32   &              refUInt32  ( int rowIdx, int colIdx );
    uint64   &              refUInt64  ( int rowIdx, int colIdx );
    float    &              refFloat   ( int rowIdx, int colIdx );
    double   &              refDouble  ( int rowIdx, int colIdx );
    DateTime &              refDateTime( int rowIdx, int colIdx );
    String   &              refString  ( int rowIdx, int colIdx );
    Binary   &              refBinary  ( int rowIdx, int colIdx );

    List<uint8>             _columnDbTypes;     // 每字段的数据类型
    List<String>            _columnNames;       // 每字段的名字
    List<void*>             _columnDatas;       // 每字段的数据集合
    List<List<bool>*>       _columnNulls;       // 每字段的空值标记集合
    int                     _rowCount;          // 数据总行数。受增删数据行同步
    int                     _columnCount;       // 字段总个数。受增删字段操作同步



    // for Binary
    //void binaryW( Binary & b ) const;
    //ReadStatus binaryR( Buffer & b );
    //void binaryDW( Buffer & b ) const;
    //void binaryDR( Buffer & b );

    // for Bytes
    void bytesW( Bytes & b ) const;
    ReadStatus bytesR( Bytes & b );
    //void bytesDW( Bytes & b ) const;
    //void bytesDR( Bytes & b );
};


struct DbValue
{
    // operatorT series
    operator                 bool() const;
    operator                int8 const & () const;
    operator                int16 const & () const;
    operator                int32 const & () const;
    operator                int64 const & () const;
    operator                uint8 const & () const;
    operator                uint16 const & () const;
    operator                uint32 const & () const;
    operator                uint64 const & () const;
    operator                float const & () const;
    operator                double const & () const;
    operator                DateTime const & () const;
    operator                String const & () const;
    operator                Binary const & () const;

    // operator= series
    void                    operator=( bool     const & v );
    void                    operator=( int8     const & v );
    void                    operator=( int16    const & v );
    void                    operator=( int32    const & v );
    void                    operator=( int64    const & v );
    void                    operator=( uint8    const & v );
    void                    operator=( uint16   const & v );
    void                    operator=( uint32   const & v );
    void                    operator=( uint64   const & v );
    void                    operator=( float    const & v );
    void                    operator=( double   const & v );
    void                    operator=( DateTime const & v );
    void                    operator=( String   const & v );
    void                    operator=( Binary   const & v );
    void                    operator=( String && v );
    void                    operator=( Binary && v );
    void                    operator=( char const * v );

    bool                    isNull() const;
    void                    setNull();
    void                    toString( String & s ) const;

    DbValue( DbTable & table, int rowIdx, int colIdx );
    int                     _rowIndex;
    int                     _columnIndex;
    DbTable *               _table;
};

struct DbRow
{
    DbTable *               _table;
    int                     _rowIndex;

    DbRow( DbTable & dt, int rowIdx );

    DbRow & operator=( DbRow const & r );

    bool                    isNull( int colIdx ) const;
    bool                    isNull( String const & name ) const;

    void                    setNull( int colIdx );
    void                    setNull( String const & name );

    DbValue                 getValue( int colIdx );
    DbValue                 getValue( String const & name );

    DbValue                 operator[]( int colIdx );
    DbValue                 operator[]( String const & name );
};





// 向列容器中添加一个默认值（用于 newRow 时填充新行数据）
void addDefaultBoolean  ( void* container );
void addDefaultInt8     ( void* container );
void addDefaultInt16    ( void* container );
void addDefaultInt32    ( void* container );
void addDefaultInt64    ( void* container );
void addDefaultUInt8    ( void* container );
void addDefaultUInt16   ( void* container );
void addDefaultUInt32   ( void* container );
void addDefaultUInt64   ( void* container );
void addDefaultFloat    ( void* container );
void addDefaultDouble   ( void* container );
void addDefaultDateTime ( void* container );
void addDefaultString   ( void* container );
void addDefaultBinary   ( void* container );

typedef void( *addDefaultFuncType )( void* ) ;

static const addDefaultFuncType addDefaultFuncs[] =
{
    addDefaultBoolean   ,
    addDefaultInt8      ,
    addDefaultInt16     ,
    addDefaultInt32     ,
    addDefaultInt64     ,
    addDefaultUInt8     ,
    addDefaultUInt16    ,
    addDefaultUInt32    ,
    addDefaultUInt64    ,
    addDefaultFloat     ,
    addDefaultDouble    ,
    addDefaultDateTime  ,
    addDefaultString    ,
    addDefaultBinary    ,
};

// 创建列容器
void addContainerBoolean    ( List<void*> & container );
void addContainerInt8       ( List<void*> & container );
void addContainerInt16      ( List<void*> & container );
void addContainerInt32      ( List<void*> & container );
void addContainerInt64      ( List<void*> & container );
void addContainerUInt8      ( List<void*> & container );
void addContainerUInt16     ( List<void*> & container );
void addContainerUInt32     ( List<void*> & container );
void addContainerUInt64     ( List<void*> & container );
void addContainerFloat      ( List<void*> & container );
void addContainerDouble     ( List<void*> & container );
void addContainerDateTime   ( List<void*> & container );
void addContainerString     ( List<void*> & container );
void addContainerBinary     ( List<void*> & container );

typedef void( *addContainerFuncType )( List<void*> & ) ;

static const addContainerFuncType addContainerFuncs[] =
{
    addContainerBoolean ,
    addContainerInt8    ,
    addContainerInt16   ,
    addContainerInt32   ,
    addContainerInt64   ,  
    addContainerUInt8   ,
    addContainerUInt16  ,
    addContainerUInt32  ,
    addContainerUInt64  ,
    addContainerFloat   ,
    addContainerDouble  ,
    addContainerDateTime,
    addContainerString  ,
    addContainerBinary  ,
};

// 回收列容器
void deleteContainerBoolean ( void* container );
void deleteContainerInt8    ( void* container );
void deleteContainerInt16   ( void* container );
void deleteContainerInt32   ( void* container );
void deleteContainerInt64   ( void* container );
void deleteContainerUInt8   ( void* container );
void deleteContainerUInt16  ( void* container );
void deleteContainerUInt32  ( void* container );
void deleteContainerUInt64  ( void* container );
void deleteContainerFloat   ( void* container );
void deleteContainerDouble  ( void* container );
void deleteContainerDateTime( void* container );
void deleteContainerString  ( void* container );
void deleteContainerBinary  ( void* container );

typedef void( *deleteContainerFuncType )( void* ) ;

static const deleteContainerFuncType deleteContainerFuncs[] =
{
    deleteContainerBoolean  ,
    deleteContainerInt8     ,
    deleteContainerInt16    ,
    deleteContainerInt32    ,
    deleteContainerInt64    ,
    deleteContainerUInt8    ,
    deleteContainerUInt16   ,
    deleteContainerUInt32   ,
    deleteContainerUInt64   ,
    deleteContainerFloat    ,
    deleteContainerDouble   ,
    deleteContainerDateTime ,
    deleteContainerString   ,
    deleteContainerBinary   ,
};

// clear 列容器
void clearContainerBoolean ( void* container );
void clearContainerInt8    ( void* container );
void clearContainerInt16   ( void* container );
void clearContainerInt32   ( void* container );
void clearContainerInt64   ( void* container );
void clearContainerUInt8   ( void* container );
void clearContainerUInt16  ( void* container );
void clearContainerUInt32  ( void* container );
void clearContainerUInt64  ( void* container );
void clearContainerFloat   ( void* container );
void clearContainerDouble  ( void* container );
void clearContainerDateTime( void* container );
void clearContainerString  ( void* container );
void clearContainerBinary  ( void* container );

typedef void( *clearContainerFuncType )( void* ) ;

static const clearContainerFuncType clearContainerFuncs[] =
{
    clearContainerBoolean  ,
    clearContainerInt8     ,
    clearContainerInt16    ,
    clearContainerInt32    ,
    clearContainerInt64    ,
    clearContainerUInt8    ,
    clearContainerUInt16   ,
    clearContainerUInt32   ,
    clearContainerUInt64   ,
    clearContainerFloat    ,
    clearContainerDouble   ,
    clearContainerDateTime ,
    clearContainerString   ,
    clearContainerBinary   ,
};

// toString 系列
void toStringBoolean  ( String & s, DbValue const & v );
void toStringInt8     ( String & s, DbValue const & v );
void toStringInt16    ( String & s, DbValue const & v );
void toStringInt32    ( String & s, DbValue const & v );
void toStringInt64    ( String & s, DbValue const & v );
void toStringUInt8    ( String & s, DbValue const & v );
void toStringUInt16   ( String & s, DbValue const & v );
void toStringUInt32   ( String & s, DbValue const & v );
void toStringUInt64   ( String & s, DbValue const & v );
void toStringFloat    ( String & s, DbValue const & v );
void toStringDouble   ( String & s, DbValue const & v );
void toStringDateTime ( String & s, DbValue const & v );
void toStringString   ( String & s, DbValue const & v );
void toStringBinary   ( String & s, DbValue const & v );

typedef void( *toStringFuncType )( String &, DbValue const & ) ;

static const toStringFuncType toStringFuncs[] =
{
    toStringBoolean ,
    toStringInt8    ,
    toStringInt16   ,
    toStringInt32   ,
    toStringInt64   ,
    toStringUInt8   ,
    toStringUInt16  ,
    toStringUInt32  ,
    toStringUInt64  ,
    toStringFloat   ,
    toStringDouble  ,
    toStringDateTime,
    toStringString  ,
    toStringBinary  ,
};


// Bytes 系列: 将 _columnDatas[ ? ] 转为相应的 List<T> 写入 bytes
void toBytesBoolean  ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesInt8     ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesInt16    ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesInt32    ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesInt64    ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesUInt8    ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesUInt16   ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesUInt32   ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesUInt64   ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesFloat    ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesDouble   ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesDateTime ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesString   ( Bytes & b, DbTable const & dt, int colIdx );
void toBytesBinary   ( Bytes & b, DbTable const & dt, int colIdx );

typedef void( *toBytesFuncType )( Bytes &, DbTable const &, int ) ;

static const toBytesFuncType toBytesFuncs[] =
{
    toBytesBoolean ,
    toBytesInt8    ,
    toBytesInt16   ,
    toBytesInt32   ,
    toBytesInt64   ,
    toBytesUInt8   ,
    toBytesUInt16  ,
    toBytesUInt32  ,
    toBytesUInt64  ,
    toBytesFloat   ,
    toBytesDouble  ,
    toBytesDateTime,
    toBytesString  ,
    toBytesBinary  ,
};

// Bytes 系列: 从 bytes 读出数据填充到 _columnDatas[ ? ]
ReadStatus fromBytesBoolean  ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesInt8     ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesInt16    ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesInt32    ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesInt64    ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesUInt8    ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesUInt16   ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesUInt32   ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesUInt64   ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesFloat    ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesDouble   ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesDateTime ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesString   ( Bytes & b, DbTable & dt, int colIdx );
ReadStatus fromBytesBinary   ( Bytes & b, DbTable & dt, int colIdx );

typedef ReadStatus( *fromBytesFuncType )( Bytes &, DbTable &, int ) ;

static const fromBytesFuncType fromBytesFuncs[] =
{
    fromBytesBoolean ,
    fromBytesInt8    ,
    fromBytesInt16   ,
    fromBytesInt32   ,
    fromBytesInt64   ,
    fromBytesUInt8   ,
    fromBytesUInt16  ,
    fromBytesUInt32  ,
    fromBytesUInt64  ,
    fromBytesFloat   ,
    fromBytesDouble  ,
    fromBytesDateTime,
    fromBytesString  ,
    fromBytesBinary  ,
};




#endif

