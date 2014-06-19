#ifndef DBSET_H
#define DBSET_H
namespace Lib3 {


/*

支持以下数据类型：
编号        数据类型
0           Byte     
1           UInt16   
2           UInt32   
3           UInt64   
4           SByte    
5           Int16    
6           Int32    
7           Int64    
8           Double   
9           Single   
10          Char     
11          Boolean  
12          DateTime 
13          Guid     
14          Decimal  
15          Binary  
16          SString  
17          String   
18          Nullable<Byte     >
19          Nullable<UInt16   >
20          Nullable<UInt32   >
21          Nullable<UInt64   >
22          Nullable<SByte    >
23          Nullable<Int16    >
24          Nullable<Int32    >
25          Nullable<Int64    >
26          Nullable<Double   >
27          Nullable<Single   >
28          Nullable<Char     >
29          Nullable<Boolean  >
30          Nullable<DateTime >
31          Nullable<Guid     >
32          Nullable<Decimal  >
33          Nullable<Binary   >
34          Nullable<SString  >
35          Nullable<String   >

// 代码示例：

DbSet ds;
{
    ds.Message() = L"汉字信息";
    ds.AffectedRows() = -1;
    var &dt = ds.NewDbTable();
    dt.AddColumn<Int32>(L"编号");
    dt.AddColumn<String>(L"名称");
    dt.AddColumn<Nullable<Boolean>>(L"性别");
    dt.AddColumn<Nullable<DateTime>>();         // 字段名为空
    dt.AddRow(1, String(L"asdf"),      Nullable<Boolean>(true),    Nullable<DateTime>());
    dt.AddRow(2, String(L"qwer"),      Nullable<Boolean>(false),   Nullable<DateTime>(DateTime::Now()));
    dt.AddRow(3, String(L"zzz"),       Nullable<Boolean>(),        Nullable<DateTime>(DateTime::Now()));
    dt.AddRow(4, String(L"12345"),     Nullable<Boolean>(),        Nullable<DateTime>(DateTime::Now()));
    dt.AddRow(5, String(L"ererer"),    Nullable<Boolean>(true),    Nullable<DateTime>());
    dt.AddColumn<Nullable<Binary  >>(L"图片");  // 在有数据的情况下再加字段

    // 无性能损失的值初始化
    var &nv = dt.Value<Nullable<Binary  >>(4, 4);
    nv.HasValue() = true;
    var &v = nv.Value();
    new (&v) Binary  ();
    v.Write(123).Write(SString("asdf"));

    var &cd1 = dt.ColumnData<String>(L"名称");                 // 引用到要查找的字段的数据容器
    Cout << cd1.Contains(L"zzz") << NewLine << NewLine;         // 查找是否存在某值

    ds.Add(dt);             // 复制添加 (ds 不会判断是否已存在或重复，无脑创建复制一份)

    dt.RemoveColumn(3);     // 删后两个字段
    dt.RemoveColumn(3);
    dt.RemoveRow(0);        // 删前3行
    dt.RemoveRows(0, 2);
    Cout << dt << NewLine;
    Cout << dt.Value<String>(1, 1) << NewLine << NewLine;     // 显示指定行列的值
    dt.ClearData();
    Cout << dt << NewLine;
    dt.ClearAll();
    Cout << dt << NewLine;
}
Cout << ds;
    
*/

struct DbTable {
    DbTable();
    ~DbTable();
    DbTable(DbTable const &dt);
    DbTable &                           operator=(DbTable const &dt);

    template<typename T> DbTable &      AddColumn(String const &colName = String::Empty);
    template<typename V1> 
    DbTable &                           AddRow(V1 const &v1);
    template<typename V1, typename V2>
    DbTable &                           AddRow(V1 const &v1, V2 const &v2);
    /* 其内包含 3 ~ 99 个长度的模板函数 */
    #include "../INC/DbSetAR.inc"

    // 以指定数据类型来引用到指定行指定列的值
    template<typename T> T const &      Value(UInt32 const &rowIdx, UInt32 const &colIdx) const;
    template<typename T> T &            Value(UInt32 const &rowIdx, UInt32 const &colIdx);
    Int32                               GetColumnIndex(String const &colName) const;   // 根据字段名来查找字段索引 (字段名有重的话以找到的第1个为准)，没找到返回 -1
    void                                ChangeColumnName(UInt32 const &colIdx, String const &newname); // 改名的同时更新 hash （必须用此函数来改字段名）
    void                                RemoveRow(UInt32 const &rowIdx);                        // 删数据行
    void                                RemoveRows(UInt32 const &offset, UInt32 const &len);    // 删数行数据行
    void                                RemoveColumn(UInt32 const &colIdx);                     // 删数据列
    void                                ClearData();    // 删所有数据（保留列信息）
    void                                ClearAll();     // 删所有列所有数据

    template<typename T> List<T> const& ColumnData(UInt32 const &colIdx) const;       // 引用到列数据 (const 版)
    template<typename T> List<T> const& ColumnData(String const &colName) const;     // 引用到列数据 (const 版)
    template<typename T> List<T> &      ColumnData(UInt32 const &colIdx);             // 引用到列数据
    template<typename T> List<T> &      ColumnData(String const &colName);           // 引用到列数据

    List<String> const &               ColumnNames() const;    // 引用私有 fields (const 版)
    List<Byte> const &                  ColumnTypes() const;    // 引用私有 fields (const 版)
    List<void*> const &                 ColumnDatas() const;    // 引用私有 fields (const 版)
    UInt32 const &                      RowCount() const;       // 引用私有 fields (const 版)
    UInt32 const &                      ColumnCount() const;    // 引用私有 fields (const 版)
    List<String> &                     ColumnNames();          // 引用私有 fields
    List<Byte> &                        ColumnTypes();          // 引用私有 fields
    List<void*> &                       ColumnDatas();          // 引用私有 fields
    UInt32 &                            RowCount();             // 引用私有 fields
    UInt32 &                            ColumnCount();          // 引用私有 fields
private:
    List<Byte>                          _colTypes;          // 存字段类型编号 0 ~ 39
    List<String>                       _colNames;          // 存字段名
    List<void*>                         _colDatas;          // 存字段数据 List<T> 指针（按列存）
    List<UInt32>                        _colNamesHash;      // 存字段名哈希值
    UInt32                              _rowCount;          // 行总数（和 _colDatas 中的 List<T>.Length() 同步）
    UInt32                              _colCount;          // 列总数（和上面的 List 容器同步）

    typedef void                        (* RemoveColumnFunc_t)(void * const &list);
    template<typename T> static void    RemoveColumnFunc(void * const &list);
    static RemoveColumnFunc_t const     RemoveColumnFuncs[];

    typedef void                        (* RemoveRowFunc_t)(void * const &list, UInt32 const &idx);
    template<typename T> static void    RemoveRowFunc(void * const &list, UInt32 const &idx);
    static RemoveRowFunc_t const        RemoveRowFuncs[];

    typedef void                        (* RemoveRowsFunc_t)(void * const &list, UInt32 const &offset, UInt32 const &len);
    template<typename T> static void    RemoveRowsFunc(void * const &list, UInt32 const &offset, UInt32 const &len);
    static RemoveRowsFunc_t const       RemoveRowsFuncs[];

    typedef void                        (* CopyColumnFunc_t)(List<void*> &dst, void * const &list);
    template<typename T> static void    CopyColumnFunc(List<void*> &dst, void * const &list);
    static CopyColumnFunc_t const       CopyColumnFuncs[];

};



struct DbSet : public List<DbTable> {
    typedef List<DbTable> BaseType;
    DbSet();
    ~DbSet();
    DbSet(DbSet const &ds);
    DbSet &                             operator=(DbSet const &ds);
    DbTable &                           NewDbTable();               // 在 DbSet 中直接创建一个 DbTable 并返回引用 （没有复制的过程）
    void                                Clear();                    // 清数据

    Int64 const &                       AffectedRows() const;
    String const &                     Message() const;
    Int64 &                             AffectedRows();
    String &                           Message();
private:
    Int64                               _affectedRows;
    String                              _msg;
};




template<typename T> struct DbTypeIDs { static Byte ID(); };             // 编号
template<> struct DbTypeIDs<Byte               > { static Byte ID() { return 0 ; }; };
template<> struct DbTypeIDs<UInt16             > { static Byte ID() { return 1 ; }; };
template<> struct DbTypeIDs<UInt32             > { static Byte ID() { return 2 ; }; };
template<> struct DbTypeIDs<UInt64             > { static Byte ID() { return 3 ; }; };
template<> struct DbTypeIDs<SByte              > { static Byte ID() { return 4 ; }; };
template<> struct DbTypeIDs<Int16              > { static Byte ID() { return 5 ; }; };
template<> struct DbTypeIDs<Int32              > { static Byte ID() { return 6 ; }; };
template<> struct DbTypeIDs<Int64              > { static Byte ID() { return 7 ; }; };
template<> struct DbTypeIDs<Double             > { static Byte ID() { return 8 ; }; };
template<> struct DbTypeIDs<Single             > { static Byte ID() { return 9 ; }; };
template<> struct DbTypeIDs<Char               > { static Byte ID() { return 10; }; };
template<> struct DbTypeIDs<Boolean            > { static Byte ID() { return 11; }; };
template<> struct DbTypeIDs<DateTime           > { static Byte ID() { return 12; }; };
template<> struct DbTypeIDs<Guid               > { static Byte ID() { return 13; }; };
template<> struct DbTypeIDs<Decimal            > { static Byte ID() { return 14; }; };
template<> struct DbTypeIDs<Binary             > { static Byte ID() { return 15; }; };
template<> struct DbTypeIDs<SString            > { static Byte ID() { return 16; }; };
template<> struct DbTypeIDs<String             > { static Byte ID() { return 17; }; };
template<> struct DbTypeIDs<Nullable<Byte     >> { static Byte ID() { return 18; }; };
template<> struct DbTypeIDs<Nullable<UInt16   >> { static Byte ID() { return 19; }; };
template<> struct DbTypeIDs<Nullable<UInt32   >> { static Byte ID() { return 20; }; };
template<> struct DbTypeIDs<Nullable<UInt64   >> { static Byte ID() { return 21; }; };
template<> struct DbTypeIDs<Nullable<SByte    >> { static Byte ID() { return 22; }; };
template<> struct DbTypeIDs<Nullable<Int16    >> { static Byte ID() { return 23; }; };
template<> struct DbTypeIDs<Nullable<Int32    >> { static Byte ID() { return 24; }; };
template<> struct DbTypeIDs<Nullable<Int64    >> { static Byte ID() { return 25; }; };
template<> struct DbTypeIDs<Nullable<Double   >> { static Byte ID() { return 26; }; };
template<> struct DbTypeIDs<Nullable<Single   >> { static Byte ID() { return 27; }; };
template<> struct DbTypeIDs<Nullable<Char     >> { static Byte ID() { return 28; }; };
template<> struct DbTypeIDs<Nullable<Boolean  >> { static Byte ID() { return 29; }; };
template<> struct DbTypeIDs<Nullable<DateTime >> { static Byte ID() { return 30; }; };
template<> struct DbTypeIDs<Nullable<Guid     >> { static Byte ID() { return 31; }; };
template<> struct DbTypeIDs<Nullable<Decimal  >> { static Byte ID() { return 32; }; };
template<> struct DbTypeIDs<Nullable<Binary   >> { static Byte ID() { return 33; }; };
template<> struct DbTypeIDs<Nullable<SString  >> { static Byte ID() { return 34; }; };
template<> struct DbTypeIDs<Nullable<String   >> { static Byte ID() { return 35; }; };




template<UInt32 tid> struct DbTypes { };
template<> struct DbTypes<0 > { typedef Byte                Type; };
template<> struct DbTypes<1 > { typedef UInt16              Type; };
template<> struct DbTypes<2 > { typedef UInt32              Type; };
template<> struct DbTypes<3 > { typedef UInt64              Type; };
template<> struct DbTypes<4 > { typedef SByte               Type; };
template<> struct DbTypes<5 > { typedef Int16               Type; };
template<> struct DbTypes<6 > { typedef Int32               Type; };
template<> struct DbTypes<7 > { typedef Int64               Type; };
template<> struct DbTypes<8 > { typedef Double              Type; };
template<> struct DbTypes<9 > { typedef Single              Type; };
template<> struct DbTypes<10> { typedef Char                Type; };
template<> struct DbTypes<11> { typedef Boolean             Type; };
template<> struct DbTypes<12> { typedef DateTime            Type; };
template<> struct DbTypes<13> { typedef Guid                Type; };
template<> struct DbTypes<14> { typedef Decimal             Type; };
template<> struct DbTypes<15> { typedef Binary              Type; };
template<> struct DbTypes<16> { typedef SString             Type; };
template<> struct DbTypes<17> { typedef String              Type; };
template<> struct DbTypes<18> { typedef Nullable<Byte     > Type; };
template<> struct DbTypes<19> { typedef Nullable<UInt16   > Type; };
template<> struct DbTypes<20> { typedef Nullable<UInt32   > Type; };
template<> struct DbTypes<21> { typedef Nullable<UInt64   > Type; };
template<> struct DbTypes<22> { typedef Nullable<SByte    > Type; };
template<> struct DbTypes<23> { typedef Nullable<Int16    > Type; };
template<> struct DbTypes<24> { typedef Nullable<Int32    > Type; };
template<> struct DbTypes<25> { typedef Nullable<Int64    > Type; };
template<> struct DbTypes<26> { typedef Nullable<Double   > Type; };
template<> struct DbTypes<27> { typedef Nullable<Single   > Type; };
template<> struct DbTypes<28> { typedef Nullable<Char     > Type; };
template<> struct DbTypes<29> { typedef Nullable<Boolean  > Type; };
template<> struct DbTypes<30> { typedef Nullable<DateTime > Type; };
template<> struct DbTypes<31> { typedef Nullable<Guid     > Type; };
template<> struct DbTypes<32> { typedef Nullable<Decimal  > Type; };
template<> struct DbTypes<33> { typedef Nullable<Binary   > Type; };
template<> struct DbTypes<34> { typedef Nullable<SString  > Type; };
template<> struct DbTypes<35> { typedef Nullable<String   > Type; };




// 下面是生成 AddRow 相关代码的生成器
//
//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;
//using System.IO;
//using System.Windows.Forms;
//
//namespace Net45Test1 {
//    class Program {
//        [STAThreadAttribute()]
//        static void Main(string[] args) {
//
//            // 这段是生成声明部分的
//            /*
//             长像：
//            template<typename V1, typename V2>
//            DbTable & AddRow(V1 const &v1, V2 const &v2);
//
//                        var sb = new StringBuilder();
//                        for (int i = 3; i <= 99; i++) {
//                            var tmp1 = new StringBuilder();
//                            var tmp2 = new StringBuilder();
//                            for (int j = 1; j <= i; j++) {
//                                tmp1.Append("typename V" + j);
//                                tmp2.Append("V" + j + " const &v" + j);
//                                if (j < i) {
//                                    tmp1.Append(", ");
//                                    tmp2.Append(", ");
//                                }
//                            }
//                            sb.Append(string.Format(@"template<{0}>
//            DbTable & AddRow({1});
//            ", tmp1, tmp2));
//                        }
//                        Clipboard.SetText(sb.ToString());
//            */
//
//
//            /*
//            长像：
//            template<typename V1, typename V2>
//            DbTable & DbTable::AddRow(V1 const &v1, V2 const &v2) {
//                assert( _colNames.Length() == 2 );
//                assert( _colTypes[0] == DbTypeIDs<V1>::ID() );
//                assert( _colTypes[1] == DbTypeIDs<V2>::ID() );
//                ((List<V1>*)_colDatas[0])->Add(v1);
//                ((List<V2>*)_colDatas[1])->Add(v2);
//                ++_rowCount;
//                return *this;
//            }
//            */
//            var sb = new StringBuilder();
//            for (int i = 3; i <= 99; i++) {
//                var sb0 = new StringBuilder();
//                var sb1 = new StringBuilder();
//                var sb2 = new StringBuilder();
//                var sb3 = new StringBuilder();
//                for (int j = 1; j <= i; j++) {
//                    sb0.Append("typename V" + j);
//                    sb1.Append("V" + j + " const &v" + j);
//                    sb2.Append("    assert( _colTypes[" + (j - 1) + "] == DbTypeIDs<V" + j + ">::ID() );\r\n");
//                    sb3.Append("    ((List<V" + j + ">*)_colDatas[" + (j - 1) + "])->Add(v" + j + ");\r\n");
//                    if (j < i) {
//                        sb0.Append(", ");
//                        sb1.Append(", ");
//                    }
//                }
//                sb.Append(string.Format(@"template<{0}>
//DbTable & DbTable::AddRow({1}) {{
//    assert( _colNames.Length() == {4} );
//{2}{3}    ++_rowCount;
//    return *this;
//}}
//", sb0, sb1, sb2, sb3, i));
//            }
//            Clipboard.SetText(sb.ToString());
//
//
//        }
//    }
//}








}
#endif