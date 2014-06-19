#ifndef DBSET_HPP
#define DBSET_HPP
namespace Lib3 {




INLINE DbTable::DbTable() {
    _rowCount = _colCount = 0;
}

INLINE DbTable::~DbTable() {
    for (UInt32 i = 0; i < _colCount; ++i)
        RemoveColumnFuncs[_colTypes[i]](_colDatas[i]);
}

INLINE DbTable::DbTable(DbTable const &dt) {
    _colTypes     = dt._colTypes;    
    _colNames     = dt._colNames;    
    _colNamesHash = dt._colNamesHash;
    _rowCount     = dt._rowCount;    
    _colCount     = dt._colCount;    
    for (UInt32 i = 0; i < _colCount; ++i)
        CopyColumnFuncs[_colTypes[i]](_colDatas, dt._colDatas[i]);
}
INLINE DbTable & DbTable::operator=(DbTable const &dt) {
    if (&dt == this)
        return *this;
    ClearAll();
    _colTypes     = dt._colTypes;    
    _colNames     = dt._colNames;    
    _colNamesHash = dt._colNamesHash;
    _rowCount     = dt._rowCount;    
    _colCount     = dt._colCount;    
    for (UInt32 i = 0; i < _colCount; ++i)
        CopyColumnFuncs[_colTypes[i]](_colDatas, dt._colDatas[i]);
    return *this;
}



template<typename T> INLINE DbTable & DbTable::AddColumn(String const &colName) {
    _colNames       << colName; 
    _colNamesHash   << Math::GetHashCode(colName);
    _colTypes       << DbTypeIDs<T>::ID(); 
    var &list = *(new List<T>());
    _colDatas       << (void*)&list;
    if (_rowCount) {
        list.EnsureLength(_rowCount);
        for (UInt32 i = 0; i < _rowCount; ++i)
            new (&list[i]) T();
    }
    ++_colCount;
    return *this; 
}


template<typename V1> 
DbTable & DbTable::AddRow(V1 const &v1) {
    assert( _colNames.Length() == 1 );
    assert( _colTypes[0] == DbTypeIDs<V1>::ID() );
    ((List<V1>*)_colDatas[0])->Add(v1);
    ++_rowCount;
    return *this;
}
template<typename V1, typename V2>
DbTable & DbTable::AddRow(V1 const &v1, V2 const &v2) {
    assert( _colNames.Length() == 2 );
    assert( _colTypes[0] == DbTypeIDs<V1>::ID() );
    assert( _colTypes[1] == DbTypeIDs<V2>::ID() );
    ((List<V1>*)_colDatas[0])->Add(v1);
    ((List<V2>*)_colDatas[1])->Add(v2);
    ++_rowCount;
    return *this;
}
/* 下面是 3 ~ 99 个函数重载的实现 */
#include "../INC/DbSetAR_hpp.inc"




template<typename T>
INLINE T const & DbTable::Value(UInt32 const &rowIdx, UInt32 const &colIdx) const {
    assert ( colIdx < _colCount && rowIdx < _rowCount );
    return (*(List<T>*)_colDatas[colIdx])[rowIdx];
}

template<typename T>
INLINE T & DbTable::Value(UInt32 const &rowIdx, UInt32 const &colIdx) {
    assert ( colIdx < _colCount && rowIdx < _rowCount );
    return (*(List<T>*)_colDatas[colIdx])[rowIdx];
}

INLINE Int32 DbTable::GetColumnIndex(String const &colName) const {
    var hc = Math::GetHashCode(colName);
    for (UInt32 i = 0; i < _colCount; ++i)
        if (_colNamesHash[i] == hc)
            return i;
    return -1;
}

INLINE void DbTable::ChangeColumnName(UInt32 const &colIdx, String const &newname) {
    assert ( colIdx < _colCount );
    _colNames[colIdx] = newname;
    _colNamesHash[colIdx] = Math::GetHashCode(newname);
}

INLINE void DbTable::RemoveColumn(UInt32 const &colIdx) {
    assert ( colIdx < _colCount );
    _colTypes.RemoveAt(colIdx);
    _colNames.RemoveAt(colIdx);
    _colDatas.RemoveAt(colIdx);
    _colNamesHash.RemoveAt(colIdx);
    --_colCount;
}

INLINE void DbTable::RemoveRow(UInt32 const &rowIdx) {
    assert ( rowIdx < _rowCount );
    for (UInt32 i = 0; i < _colCount; ++i)
        RemoveRowFuncs[_colTypes[i]](_colDatas[i], rowIdx);
    --_rowCount;
}

INLINE void DbTable::RemoveRows(UInt32 const &offset, UInt32 const &len) {
    if (offset >= _rowCount)
        return;
    UInt32 len_ = len;
    if (offset + len_ > _rowCount)
        len_ = _rowCount - offset;
    _rowCount -= len_;
    for (UInt32 i = 0; i < _colCount; ++i)
        RemoveRowsFuncs[_colTypes[i]](_colDatas[i], offset, len);
}

INLINE void DbTable::ClearData() {
    for (UInt32 i = 0; i < _colCount; ++i)
        RemoveRowsFuncs[_colTypes[i]](_colDatas[i], 0, 0);
    _rowCount = 0;
}

INLINE void DbTable::ClearAll() {
    for (UInt32 i = 0; i < _colCount; ++i)
        RemoveColumnFuncs[_colTypes[i]](_colDatas[i]);
    _colTypes.Clear();
    _colNames.Clear();
    _colDatas.Clear();
    _colNamesHash.Clear();
    _rowCount = 0;
    _colCount = 0;
}


template<typename T> INLINE List<T> const& DbTable::ColumnData(UInt32 const &colIdx) const {
    assert ( colIdx < _colCount );
    return *(List<String>*)_colDatas[colIdx];
}
template<typename T> INLINE List<T> const& DbTable::ColumnData(String const &colName) const {
    return *(List<String>*)_colDatas[GetColumnIndex(colName)];
}
template<typename T> INLINE List<T> &      DbTable::ColumnData(UInt32 const &colIdx) {
    assert ( colIdx < _colCount );
    return *(List<String>*)_colDatas[colIdx];
}   
template<typename T> INLINE List<T> &      DbTable::ColumnData(String const &colName) {
    return *(List<String>*)_colDatas[GetColumnIndex(colName)];
}



INLINE List<String> const & DbTable::ColumnNames() const {
    return _colNames;
}
INLINE List<Byte> const & DbTable::ColumnTypes() const {
    return _colTypes;
}
INLINE List<void*> const & DbTable::ColumnDatas() const {
    return _colDatas;
}
INLINE UInt32 const & DbTable::RowCount() const {
    return _rowCount;
}
INLINE UInt32 const & DbTable::ColumnCount() const {
    return _colCount;
}
INLINE List<String> & DbTable::ColumnNames() {
    return _colNames;
}
INLINE List<Byte> & DbTable::ColumnTypes() {
    return _colTypes;
}
INLINE List<void*> & DbTable::ColumnDatas() {
    return _colDatas;
}
INLINE UInt32 & DbTable::RowCount() {
    return _rowCount;
}
INLINE UInt32 & DbTable::ColumnCount() {
    return _colCount;
}



template<typename T> INLINE void DbTable::RemoveColumnFunc(void * const &list) {
    delete (List<T>*)list;
}

template<typename T> INLINE void DbTable::RemoveRowFunc(void * const &list, UInt32 const &idx) {
    (*(List<T>*)list).RemoveAt(idx);
}

template<typename T> INLINE void DbTable::RemoveRowsFunc(void * const &list, UInt32 const &offset, UInt32 const &len) {
    (*(List<T>*)list).Remove(offset, len);
}

template<typename T> INLINE void DbTable::CopyColumnFunc(List<void*> &dst, void * const &list) {
    dst.Add(new List<T>( *(List<T>*)list ));
}










INLINE DbSet::DbSet(){
    _affectedRows = 0;
}

INLINE DbSet::~DbSet(){
}

INLINE DbSet::DbSet(DbSet const &ds){
    for (UInt32 i = 0; i < ds.Length(); ++i)
        Add(ds[i]);
    _msg = ds._msg;
    _affectedRows = ds._affectedRows;
}

INLINE DbSet & DbSet::operator=(DbSet const &ds){
    if (&ds == this)
        return *this;
    BaseType::Clear();
    for (UInt32 i = 0; i < ds.Length(); ++i)
        Add(ds[i]);
    _msg = ds._msg;
    _affectedRows = ds._affectedRows;
}

INLINE DbTable & DbSet::NewDbTable() {
    EnsureSpace(1);
    return *(new (&(*this)[Length()++]) DbTable());
}

INLINE void DbSet::Clear() {
    _msg.Clear();
    _affectedRows = 0;
    BaseType::Clear();
}


INLINE Int64 const & DbSet::AffectedRows() const{
    return _affectedRows;
}

INLINE String const & DbSet::Message() const{
    return _msg;
}

INLINE Int64 & DbSet::AffectedRows(){
    return _affectedRows;
}

INLINE String & DbSet::Message(){
    return _msg;
}








}
#endif