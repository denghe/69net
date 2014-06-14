#include "Precompile.h"

void DbTable::toString( String & s ) const
{
    s << "DbTable: rows = " << _rowCount << ", cols = " << _columnCount << '\n';
    s << _columnNames << '\n';
    s << _columnDbTypes << '\n';
    for( int x = 0; x < _rowCount; ++x )
    {
        for( int y = 0; y < _columnCount; ++y )
        {
            DbValue( const_cast<DbTable&>(*this), x, y ).toString( s );
            s << ", ";
        }
        s << '\n';
    }
}

DbTable::DbTable()
    : _rowCount( 0 )
    , _columnCount( 0 )
{
}

DbTable::~DbTable()
{
    for( int i = 0; i < _columnCount; ++i )
    {
        if( i >= _columnNulls._len ) break;  // 有可能在还原数据的过程中失败 从而造成 _columnNulls 的长度小于字段数
        if( _columnNulls[ i ] )
        {
            delete _columnNulls[ i ];
        }
        deleteContainerFuncs[ _columnDbTypes[ i ] ]( _columnDatas[ i ] );
    }
}

void DbTable::clearData()
{
    for( int i = 0; i < _columnCount; ++i )
    {
        if( i >= _columnNulls._len ) break;
        if( _columnNulls[ i ] )
        {
            _columnNulls[ i ]->clear();
        }
        clearContainerFuncs[ _columnDbTypes[ i ] ]( _columnDatas[ i ] );
    }
    _rowCount = 0;
}

void DbTable::clear()
{
    for( int i = 0; i < _columnCount; ++i )
    {
        if( i >= _columnNulls._len ) break;
        if( _columnNulls[ i ] )
        {
            delete _columnNulls[ i ];
        }
        deleteContainerFuncs[ _columnDbTypes[ i ] ]( _columnDatas[ i ] );
    }
    _columnDbTypes.clear();
    _columnNames.clear();
    _columnNulls.clear();
    _columnDatas.clear();
    _rowCount = 0;
    _columnCount = 0;
}

int DbTable::getColumnIndex( String const & colName )
{
    for( int i = 0; i < _columnCount; ++i )
    {
        if( _columnNames[ i ] == colName ) return i;
    }
    return -1;
}


DbTable & DbTable::addColumn( String const & colName, DbTypes dataType, bool nullable /*= false */ )
{
    _columnNames.add( colName );
    _columnDbTypes.add( (uint8)dataType );
    _columnNulls.add( nullable ? new List<bool>() : nullptr );
    addContainerFuncs[ (int)dataType ]( _columnDatas );
    ++_columnCount;
    // todo: 如果允许在有数据行存在的情况下，再增加新字段， 则需要相应的添加具备默认值的列数据
    return *this;
}

DbRow DbTable::newRow()
{
    for( int i = 0; i < _columnCount ; ++i )
    {
        if( _columnNulls[ i ] )
        {
            _columnNulls[ i ]->add( true );
        }
        addDefaultFuncs[ _columnDbTypes[ i ] ]( _columnDatas[ i ] );
    }
    return DbRow( *this, _rowCount++ );
}

DbRow DbTable::get( int rowIdx )
{
    return DbRow( *this, rowIdx );
}

DbRow DbTable::operator[]( int rowIdx )
{
    return DbRow( *this, rowIdx );
}





#define GET_TYPE_CODE( FN, TN )                             \
TN const & DbTable::get##FN( int rowIdx, int colIdx ) const \
{                                                           \
    assert( colIdx < _columnCount                           \
        && _columnDbTypes[ colIdx ] == (uint8)DbTypes::FN   \
        && ( !_columnNulls[ colIdx ]                        \
        || !(*_columnNulls[ colIdx ])[ rowIdx ] ) );        \
    return (*(List<TN>*)_columnDatas[ colIdx ])[ rowIdx ];  \
}

//GET_TYPE_CODE( Boolean, bool );   bool 无法直接返回引用。byte 复用
GET_TYPE_CODE( Int8, int8 );
GET_TYPE_CODE( Int16, int16 );
GET_TYPE_CODE( Int32, int32 );
GET_TYPE_CODE( Int64, int64 );
GET_TYPE_CODE( UInt8, uint8 );
GET_TYPE_CODE( UInt16, uint16 );
GET_TYPE_CODE( UInt32, uint32 );
GET_TYPE_CODE( UInt64, uint64 );
GET_TYPE_CODE( Float, float );
GET_TYPE_CODE( Double, double );
GET_TYPE_CODE( DateTime, DateTime );
GET_TYPE_CODE( String, String );
GET_TYPE_CODE( Binary, Binary );

bool DbTable::getBoolean( int rowIdx, int colIdx ) const
{
    assert( colIdx < _columnCount
    && _columnDbTypes[ colIdx ] == (uint8)DbTypes::Boolean
    && ( !_columnNulls[ colIdx ]
    || !(*_columnNulls[ colIdx ])[ rowIdx ] ) );
    return (*(List<bool>*)_columnDatas[ colIdx ])[ rowIdx ];
}






#define SET_TYPE_CODE( FN, TN )                                 \
void DbTable::set##FN( int rowIdx, int colIdx, TN const & v )   \
{                                                               \
    assert( colIdx < _columnCount                               \
        && _columnDbTypes[ colIdx ] == (uint8)DbTypes::FN );    \
    if( _columnNulls[ colIdx ] )                                \
        _columnNulls[ colIdx ]->set( rowIdx, false );           \
    (*(List<TN>*)_columnDatas[ colIdx ]).set( rowIdx, v );      \
}

SET_TYPE_CODE( Boolean, bool );
SET_TYPE_CODE( Int8, int8 );
SET_TYPE_CODE( Int16, int16 );
SET_TYPE_CODE( Int32, int32 );
SET_TYPE_CODE( Int64, int64 );
SET_TYPE_CODE( UInt8, uint8 );
SET_TYPE_CODE( UInt16, uint16 );
SET_TYPE_CODE( UInt32, uint32 );
SET_TYPE_CODE( UInt64, uint64 );
SET_TYPE_CODE( Float, float );
SET_TYPE_CODE( Double, double );
SET_TYPE_CODE( DateTime, DateTime );
SET_TYPE_CODE( String, String );
SET_TYPE_CODE( Binary, Binary );

#define SET_SPECIALTYPE_CODE( FN, TN )                      \
void DbTable::set##FN( int rowIdx, int colIdx, TN && v )    \
{                                                           \
    assert( colIdx < _columnCount                           \
        && _columnDbTypes[ colIdx ] == (uint8)DbTypes::FN );\
    if( _columnNulls[ colIdx ] )                            \
        _columnNulls[ colIdx ]->set( rowIdx, false );       \
    (*(List<TN>*)_columnDatas[ colIdx ]).set( rowIdx, std::forward<TN>(v) ); \
}

SET_SPECIALTYPE_CODE( String, String );
SET_SPECIALTYPE_CODE( Binary, Binary );

void DbTable::setString( int rowIdx, int colIdx, char const * v )  
{                                                              
    assert( colIdx < _columnCount
    && _columnDbTypes[ colIdx ] == (uint8)DbTypes::String );
    if( _columnNulls[ colIdx ] )                         
        _columnNulls[ colIdx ]->set( rowIdx, false );
    (*(List<String>*)_columnDatas[ colIdx ]).set( rowIdx, v );
}





DbValue DbTable::get( int rowIdx, int colIdx )
{
    return DbValue( *this, rowIdx, colIdx );
}

bool DbTable::isNull( int rowIdx, int colIdx ) const
{
    assert( colIdx < _columnCount );
    if( !_columnNulls[ colIdx ] ) return false;
    return (*_columnNulls[ colIdx ])[ rowIdx ];
}

void DbTable::setNull( int rowIdx, int colIdx )
{
    assert( colIdx < _columnCount && _columnNulls[ colIdx ] );
    _columnNulls[ colIdx ]->set( rowIdx, true );
}





#define REF_TYPE_CODE( FN, TN )                             \
TN & DbTable::ref##FN( int rowIdx, int colIdx )             \
{                                                           \
    return (*(List<TN>*)_columnDatas[ colIdx ])[ rowIdx ];  \
}
//REF_TYPE_CODE( Boolean, bool );   bool 无法引用。byte 复用
REF_TYPE_CODE( Int8, int8 );
REF_TYPE_CODE( Int16, int16 );
REF_TYPE_CODE( Int32, int32 );
REF_TYPE_CODE( Int64, int64 );
REF_TYPE_CODE( UInt8, uint8 );
REF_TYPE_CODE( UInt16, uint16 );
REF_TYPE_CODE( UInt32, uint32 );
REF_TYPE_CODE( UInt64, uint64 );
REF_TYPE_CODE( Float, float );
REF_TYPE_CODE( Double, double );
REF_TYPE_CODE( DateTime, DateTime );
REF_TYPE_CODE( String, String );
REF_TYPE_CODE( Binary, Binary );




#define OPERATOR_BRACKETS_TYPE_CODE( FN, TN )           \
DbValue::operator TN const & () const                   \
{                                                       \
    return _table->get##FN( _rowIndex, _columnIndex );  \
}

//OPERATOR_BRACKETS_TYPE_CODE( Boolean, bool );   bool 无法引用。byte 复用
OPERATOR_BRACKETS_TYPE_CODE( Int8, int8 );
OPERATOR_BRACKETS_TYPE_CODE( Int16, int16 );
OPERATOR_BRACKETS_TYPE_CODE( Int32, int32 );
OPERATOR_BRACKETS_TYPE_CODE( Int64, int64 );
OPERATOR_BRACKETS_TYPE_CODE( UInt8, uint8 );
OPERATOR_BRACKETS_TYPE_CODE( UInt16, uint16 );
OPERATOR_BRACKETS_TYPE_CODE( UInt32, uint32 );
OPERATOR_BRACKETS_TYPE_CODE( UInt64, uint64 );
OPERATOR_BRACKETS_TYPE_CODE( Float, float );
OPERATOR_BRACKETS_TYPE_CODE( Double, double );
OPERATOR_BRACKETS_TYPE_CODE( DateTime, DateTime );
OPERATOR_BRACKETS_TYPE_CODE( String, String );
OPERATOR_BRACKETS_TYPE_CODE( Binary, Binary );

DbValue::operator bool() const
{
    return _table->getBoolean( _rowIndex, _columnIndex );
}



#define OPERATOR_EQUAL_TYPE_CODE( FN, TN )          \
void DbValue::operator=( TN const & v )             \
{                                                   \
    _table->set##FN( _rowIndex, _columnIndex, v );  \
}
OPERATOR_EQUAL_TYPE_CODE( Boolean, bool );
OPERATOR_EQUAL_TYPE_CODE( Int8, int8 );
OPERATOR_EQUAL_TYPE_CODE( Int16, int16 );
OPERATOR_EQUAL_TYPE_CODE( Int32, int32 );
OPERATOR_EQUAL_TYPE_CODE( Int64, int64 );
OPERATOR_EQUAL_TYPE_CODE( UInt8, uint8 );
OPERATOR_EQUAL_TYPE_CODE( UInt16, uint16 );
OPERATOR_EQUAL_TYPE_CODE( UInt32, uint32 );
OPERATOR_EQUAL_TYPE_CODE( UInt64, uint64 );
OPERATOR_EQUAL_TYPE_CODE( Float, float );
OPERATOR_EQUAL_TYPE_CODE( Double, double );
OPERATOR_EQUAL_TYPE_CODE( DateTime, DateTime );
OPERATOR_EQUAL_TYPE_CODE( String, String );
OPERATOR_EQUAL_TYPE_CODE( Binary, Binary );

void DbValue::operator=( String && v )
{
    _table->setString( _rowIndex, _columnIndex, std::forward<String>(v) );
}

void DbValue::operator=( Binary && v )
{
    _table->setBinary( _rowIndex, _columnIndex, std::forward<Binary>(v) );
}

void DbValue::operator=( char const * v )
{
    _table->setString( _rowIndex, _columnIndex, v );
}



DbValue::DbValue( DbTable & table, int rowIdx, int colIdx )
    : _rowIndex( rowIdx )
    , _columnIndex( colIdx )
	, _table( &table )
{
}

bool DbValue::isNull() const
{
    return _table->isNull( _rowIndex, _columnIndex );
}

void DbValue::setNull()
{
    _table->setNull( _rowIndex, _columnIndex );
}

void DbValue::toString( String & s ) const
{
    if( isNull() )
    {
        s << "[null]";
    }
    else
    {
        toStringFuncs[ _table->_columnDbTypes[ _columnIndex ] ]( s, *this );
    }
}






bool DbRow::isNull( int colIdx ) const
{
    return _table->isNull( _rowIndex, colIdx );
}

bool DbRow::isNull( String const & name ) const
{
    return _table->isNull( _rowIndex, _table->getColumnIndex( name ) );
}

void DbRow::setNull( int colIdx )
{
    _table->setNull( _rowIndex, colIdx );
}

void DbRow::setNull( String const & name )
{
    _table->setNull( _rowIndex, _table->getColumnIndex( name ) );
}






DbValue DbRow::getValue( int colIdx )
{
    return DbValue( *_table, _rowIndex, colIdx );
}

DbValue DbRow::getValue( String const & name )
{
    return DbValue( *_table, _rowIndex, _table->getColumnIndex( name ) );
}

DbValue DbRow::operator[]( int colIdx )
{
    return DbValue( *_table, _rowIndex, colIdx );
}

DbValue DbRow::operator[]( String const & name )
{
    return DbValue( *_table, _rowIndex, _table->getColumnIndex( name ) );
}






DbRow::DbRow( DbTable & dt, int rowIdx )
    : _table( &dt )
    , _rowIndex( rowIdx )
{
}

DbRow & DbRow::operator=( DbRow const & r )
{
    this->_table = r._table;
    this->_rowIndex = r._rowIndex;
    return *this;
}







#define ADDDEFAULT_TYPE_CODE( FN, TN, V )   \
void addDefault##FN( void* container )      \
{                                           \
    ((List<TN>*)container)->add( V );       \
}

ADDDEFAULT_TYPE_CODE( Boolean, bool, false );
ADDDEFAULT_TYPE_CODE( Int8, int8, 0 );
ADDDEFAULT_TYPE_CODE( Int16, int16, 0 );
ADDDEFAULT_TYPE_CODE( Int32, int32, 0 );
ADDDEFAULT_TYPE_CODE( Int64, int64, 0 );
ADDDEFAULT_TYPE_CODE( UInt8, uint8, 0 );
ADDDEFAULT_TYPE_CODE( UInt16, uint16, 0 );
ADDDEFAULT_TYPE_CODE( UInt32, uint32, 0 );
ADDDEFAULT_TYPE_CODE( UInt64, uint64, 0 );
ADDDEFAULT_TYPE_CODE( Float, float, 0.0f );
ADDDEFAULT_TYPE_CODE( Double, double, 0 );
ADDDEFAULT_TYPE_CODE( DateTime, DateTime, DateTime::defaultValue );
ADDDEFAULT_TYPE_CODE( String, String, String::defaultValue );
ADDDEFAULT_TYPE_CODE( Binary, Binary, Binary::defaultValue );








#define ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( FN, TN )  \
void addContainer##FN( List<void*> & container )            \
{                                                           \
    container.add( new List<TN>() );                        \
}                                                           \
void deleteContainer##FN( void* container )                 \
{                                                           \
    delete (List<TN>*)container;                            \
}                                                           \
void toString##FN( String & s, DbValue const & v )          \
{                                                           \
    s.write( (TN const &)v );                               \
}                                                           \
void clearContainer##FN( void* container )                  \
{                                                           \
    ((List<TN>*)container)->clear();                        \
}

ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Boolean, bool );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Int8, int8 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Int16, int16 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Int32, int32 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Int64, int64 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( UInt8, uint8 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( UInt16, uint16 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( UInt32, uint32 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( UInt64, uint64 );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Float, float );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Double, double );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( DateTime, DateTime );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( String, String );
ADD_CONTAINER_DELETE_TOSTRING_CLEAR_CODE( Binary, Binary );








#define TOBYTES_TYPE_CODE( FN, TN )                             \
void toBytes##FN( Bytes & b, DbTable const & dt, int colIdx )   \
{                                                               \
    auto & list = *(List<TN>*)dt._columnDatas[ colIdx ];        \
    if( dt._columnNulls[ colIdx ] )                             \
    {                                                           \
        auto &flags = *dt._columnNulls[ colIdx ];               \
        for( int i = 0; i < dt._rowCount; ++i )                 \
        {                                                       \
            if( !flags[ i ] ) b << list[ i ];                   \
        }                                                       \
    }                                                           \
    else                                                        \
        b << list;                                              \
}

void toBytesBoolean( Bytes & b, DbTable const & dt, int colIdx )
{
    auto & list = *(List<bool>*)dt._columnDatas[ colIdx ];
    if( dt._columnNulls[ colIdx ] )
    {
        auto &flags = *dt._columnNulls[ colIdx ];

        //List<bool> tmp;
        //for( int i = 0; i < dt._rowCount; ++i )
        //{
        //    if( !flags[ i ] ) tmp << list[ i ];
        //}
        //b << tmp;

        // 因为长度未知，先空 5 byte，最后长度，先写数据，看实际占了多少，再移动靠拢
        int size = ( dt._rowCount - 1 ) / 8 + 1;
        b.ensure( size + 5 );               // 按最长的可能性来 ensure 并空 5 byte
        int data = b._len + 5;              // 存数据区的偏移量, 空出 5 字节的长度写入区
        char buf[ sizeof(List<bool>) ];
        auto &tmp = *(List<bool>*)buf;
        tmp._len = 0;
        tmp._bufSize = size;
        tmp._buf = (decltype(tmp._buf))( b._buf + data );
        for( int i = 0; i < dt._rowCount; ++i )
        {
            if( !flags[ i ] ) tmp.addDirect( list[ i ] );
        }
        if( tmp._len == 0 )
        {
            b.writeDirect( (byte)0 );
        }
        else
        {
            b.writeDirect( (uint)tmp._len );
            size = ( tmp._len - 1 ) / 8 + 1;
            memmove( b._buf + b._len, b._buf + data, size );
            b._len += size;
            b._buf[ b._len - 1 ] &= (uint8)~( (uint8)0xFF << ( tmp._len % 8 ) );
        }
    }
    else
        b << list;
}

TOBYTES_TYPE_CODE( Int8, int8 );
TOBYTES_TYPE_CODE( Int16, int16 );
TOBYTES_TYPE_CODE( Int32, int32 );
TOBYTES_TYPE_CODE( Int64, int64 );
TOBYTES_TYPE_CODE( UInt8, uint8 );
TOBYTES_TYPE_CODE( UInt16, uint16 );
TOBYTES_TYPE_CODE( UInt32, uint32 );
TOBYTES_TYPE_CODE( UInt64, uint64 );
TOBYTES_TYPE_CODE( Float, float );
TOBYTES_TYPE_CODE( Double, double );
TOBYTES_TYPE_CODE( DateTime, DateTime );
TOBYTES_TYPE_CODE( String, String );
TOBYTES_TYPE_CODE( Binary, Binary );









// 序列化流程为：先写 _columnDbTypes, Names, _columnNulls( 作为 List<bool> 写 )
// 然后 _columnNulls（可空才写）, _columnDatas（可空则只写有值的数据）
void DbTable::bytesW( Bytes & b ) const
{
    b << _columnNames;
    b << _columnDbTypes;

    //List<bool> list( _columnCount );
    //for( int i = 0; i < _columnCount ; ++i )
    //{
    //    list.addDirect( _columnNulls[ i ] != nullptr );
    //}
    //b << list;

                                                        // 在当前位置硬转出来一个 List<bool> 令其 _buf 等于目标地址写 _columnNulls 的字段可空属性
    b << (uint)_columnCount;                            // 模拟写入数组长度值
    int size = ( _columnCount - 1 ) / 8 + 1;            // 算下要写多少字节
    b.ensure( size );                                   // 确保一下长度
    char buf[ sizeof(List<bool>) ];                     // 创建 list 本体容器
    auto &list = *(List<bool>*)buf;                     // 硬转
    list._len = 0;                                      // 写指针初始化
    list._bufSize = size;                               // 这个值须令 list 不会扩容
    list._buf = (decltype(list._buf))(b._buf + b._len); // 令 _buf 指向当前写入点
    b._len += size;                                     // 令 b 的写入点跳过 List<bool>
    int tail = b._len - 1;                              // 记录数据尾点方便清未用 bit
    for( int i = 0; i < _columnCount ; ++i )
    {
        list.addDirect( _columnNulls[ i ] != nullptr ); // 直接往 b._buf 的 写入点 写 字段可空属性
        if( _columnNulls[ i ] ) b << *_columnNulls[ i ];// 写所有可空字段的可空行标记
    }                                                   // list 不能回收
    b._buf[ tail ] &= (uint8)~( (uint8)0xFF << ( list._len % 8 ) );     // 尾巴的 2 进制数过滤掉未用的 bit
    b << (uint)_rowCount;

    for( int i = 0; i < _columnCount ; ++i )
    {
        toBytesFuncs[ _columnDbTypes[ i ] ]( b, *this, i );
    }
}







#define FROMBYTES_TYPE_CODE( FN, TN )                           \
ReadStatus fromBytes##FN( Bytes & b, DbTable & dt, int colIdx ) \
{                                                               \
    auto & list = *(List<TN>*)dt._columnDatas[ colIdx ];        \
    if( dt._columnNulls[ colIdx ] )                             \
    {                                                           \
        auto &flags = *dt._columnNulls[ colIdx ];               \
        for( int i = 0; i < dt._rowCount; ++i )                 \
        {                                                       \
            new ( &list._buf[ i ] ) TN();                       \
            list._len = i + 1;                                  \
            if( !flags[ i ] )                                   \
            {                                                   \
                auto rtv = b >> list[ i ];                      \
                if( rtv != ReadStatus::Success ) return rtv;    \
            }                                                   \
        }                                                       \
        return ReadStatus::Success;                             \
    }                                                           \
    else                                                        \
        return b >> list;                                       \
}

ReadStatus fromBytesBoolean( Bytes & b, DbTable & dt, int colIdx )
{
    auto & list = *(List<bool>*)dt._columnDatas[ colIdx ];
    if( dt._columnNulls[ colIdx ] )
    {
        auto &flags = *dt._columnNulls[ colIdx ];

        //List<bool> tmp;
        //auto rtv = b >> tmp;
        //if( rtv != ReadStatus::Success ) return rtv;

        uint len = 0;   // List<bool> 长
        auto rtv = b >> len;
        if( rtv != ReadStatus::Success ) return rtv;
        int size = ( len - 1 ) / 8 + 1;
        if( b._readIdx + size > b._len ) return ReadStatus::NotEnoughData;
        char buf[ sizeof(List<bool>) ];
        auto &tmp = *(List<bool>*)buf;
        tmp._len = 0;
        tmp._bufSize = size;
        tmp._buf = (decltype(tmp._buf))b._buf + b._readIdx;
        b._readIdx += size;

        int idx = 0;
        for( int i = 0; i < dt._rowCount; ++i )
        {
            if( !flags[ i ] ) list.set( i , tmp[ idx++ ] );
        }
        return ReadStatus::Success;
    }
    else
        return b >> list;
}

FROMBYTES_TYPE_CODE( Int8, int8 );
FROMBYTES_TYPE_CODE( Int16, int16 );
FROMBYTES_TYPE_CODE( Int32, int32 );
FROMBYTES_TYPE_CODE( Int64, int64 );
FROMBYTES_TYPE_CODE( UInt8, uint8 );
FROMBYTES_TYPE_CODE( UInt16, uint16 );
FROMBYTES_TYPE_CODE( UInt32, uint32 );
FROMBYTES_TYPE_CODE( UInt64, uint64 );
FROMBYTES_TYPE_CODE( Float, float );
FROMBYTES_TYPE_CODE( Double, double );
FROMBYTES_TYPE_CODE( DateTime, DateTime );
FROMBYTES_TYPE_CODE( String, String );
FROMBYTES_TYPE_CODE( Binary, Binary );






// 反序列化流程: 对于待填充的 DbTable 而言，分为两种情况：新建的，已存在数据的
// 新建的只需要判断 _columns 长度即可得知，从而按写入顺序依次填充即可
// 已存在数据的，理论上讲需要以 减少创建新对象的需求，尽量利用现成空间为原则
// 先清空行数据。对于已存在的 _columnNulls, 先将它们的指针放到一个 List 备用。
// 然后开始逐个还原字段信息。还原时，先读出数据类型和可空信息。再和 _columns 作对比。
// 找出数据类型符合的字段，将该字段的数据类型与 _columns[i] 的作交换，
// 再还原字段名，如果是可空的，就去 _columnNulls List 取，没有了就建。
// 如果在 _columns 里面没找出来相同的数据类型，就 add 一个并与 _columns[i] 的数据类型交换
// 字段名，可空的还原同上。交换数据类型的同时，也要交换 _columnDatas 中字段对应的指针
// 当字段信息还原完毕时，_columnCount 大于字段数，则将后面的字段信息回收（同时回收数据）
// 
// 先实现向空的 DbTable 填充再说。即便不空，也先清空。
ReadStatus DbTable::bytesR( Bytes & b )
{
    this->clear();

    auto rtv = b >> _columnNames;
    if( rtv != ReadStatus::Success ) return rtv;

    rtv = b >> _columnDbTypes;
    if( rtv != ReadStatus::Success ) return rtv;


    //List<bool> list;
    //rtv = b >> list;
    //if( rtv != ReadStatus::Success ) return rtv;
    //_columnCount = list._len;

    uint len = 0;   // 字段个数
    rtv = b >> len;
    if( rtv != ReadStatus::Success ) return rtv;

    int size = ( len - 1 ) / 8 + 1;
    if( b._readIdx + size > b._len ) return ReadStatus::NotEnoughData;

    char buf[ sizeof(List<bool>) ];     // 字段s 可空属性 list
    auto &list = *(List<bool>*)buf;
    list._len = 0;
    list._bufSize = size;
    list._buf = (decltype(list._buf))b._buf + b._readIdx;
    b._readIdx += size;                                 // 跳过 List<bool> 的数据区

    _columnCount = len;
    _columnNulls.ensure( _columnCount );
    for( int i = 0; i < _columnCount ; ++i )
    {
        _columnNulls._len = i + 1;
        if( list[ i ] )
        {
            _columnNulls[ i ] = new List<bool>();
            rtv = b >> *_columnNulls[ i ];
            if( rtv != ReadStatus::Success ) return rtv;
        }
        else
        {
             _columnNulls[ i ] = nullptr;
        }
        addContainerFuncs[ _columnDbTypes[ i ] ]( _columnDatas );
    }

    rtv = b >> (uint&)_rowCount;
    if( rtv != ReadStatus::Success ) return rtv;

    for( int i = 0; i < _columnCount ; ++i )
    {
        rtv = fromBytesFuncs[ _columnDbTypes[ i ] ]( b, *this, i );
        if( rtv != ReadStatus::Success ) return rtv;
    }

    return ReadStatus::Success;
}
