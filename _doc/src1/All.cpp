#include "All.h"

// 本目录的 _cpp.hpp 均为 .cpp 改名 去掉共有头尾
// 通过这种链接方式，可以极大提升编译速度
// 列表可乱序
//
//#include "Xxxxx_cpp.hpp"

// 下面是将一些 CPP 内少量的代码合并到一起

namespace Lib3 {

namespace Console {
WriteColFunc_t const WriteColFuncs[] = {
    WriteColFunc<0 >,
    WriteColFunc<1 >,
    WriteColFunc<2 >,
    WriteColFunc<3 >,
    WriteColFunc<4 >,
    WriteColFunc<5 >,
    WriteColFunc<6 >,
    WriteColFunc<7 >,
    WriteColFunc<8 >,
    WriteColFunc<9 >,
    WriteColFunc<10>,
    WriteColFunc<11>,
    WriteColFunc<12>,
    WriteColFunc<13>,
    WriteColFunc<14>,
    WriteColFunc<15>,
    WriteColFunc<16>,
    WriteColFunc<17>,
    WriteColFunc<18>,
    WriteColFunc<19>,
    WriteColFunc<20>,
    WriteColFunc<21>,
    WriteColFunc<22>,
    WriteColFunc<23>,
    WriteColFunc<24>,
    WriteColFunc<25>,
    WriteColFunc<26>,
    WriteColFunc<27>,
    WriteColFunc<28>,
    WriteColFunc<29>,
    WriteColFunc<30>,
    WriteColFunc<31>,
    WriteColFunc<32>,
    WriteColFunc<33>,
    WriteColFunc<34>,
    WriteColFunc<35>
};
}


#define __FUNCS_INIT(O, T, N, F) \
O::T const O::N[] = { \
    O::F<Byte     >,          \
    O::F<UInt16   >,          \
    O::F<UInt32   >,          \
    O::F<UInt64   >,          \
    O::F<SByte    >,          \
    O::F<Int16    >,          \
    O::F<Int32    >,          \
    O::F<Int64    >,          \
    O::F<Double   >,          \
    O::F<Single   >,          \
    O::F<Char     >,          \
    O::F<Boolean  >,          \
    O::F<DateTime >,          \
    O::F<Guid     >,          \
    O::F<Decimal  >,          \
    O::F<Binary   >,          \
    O::F<SString  >,          \
    O::F<String   >,          \
    O::F<Nullable<Byte     >>,\
    O::F<Nullable<UInt16   >>,\
    O::F<Nullable<UInt32   >>,\
    O::F<Nullable<UInt64   >>,\
    O::F<Nullable<SByte    >>,\
    O::F<Nullable<Int16    >>,\
    O::F<Nullable<Int32    >>,\
    O::F<Nullable<Int64    >>,\
    O::F<Nullable<Double   >>,\
    O::F<Nullable<Single   >>,\
    O::F<Nullable<Char     >>,\
    O::F<Nullable<Boolean  >>,\
    O::F<Nullable<DateTime >>,\
    O::F<Nullable<Guid     >>,\
    O::F<Nullable<Decimal  >>,\
    O::F<Nullable<Binary   >>,\
    O::F<Nullable<SString  >>,\
    O::F<Nullable<String   >> \
}

__FUNCS_INIT(DbTable, RemoveColumnFunc_t, RemoveColumnFuncs, RemoveColumnFunc);
__FUNCS_INIT(DbTable, RemoveRowFunc_t   , RemoveRowFuncs   , RemoveRowFunc   );
__FUNCS_INIT(DbTable, RemoveRowsFunc_t  , RemoveRowsFuncs  , RemoveRowsFunc  );
__FUNCS_INIT(DbTable, CopyColumnFunc_t  , CopyColumnFuncs  , CopyColumnFunc  );

#undef __FUNCS_INIT


namespace GC {
CRT const CRT::Instance;
}

namespace Strings {
UInt32 ToString_CharBuferIndex  = 0;
UInt32 ToString_WCharBuferIndex = 0;
ALIGN8( SByte ToString_CharBuffer [ToString_BuferCount][ToString_BuferLength] );
ALIGN8( Char ToString_WCharBuffer[ToString_BuferCount][ToString_BuferLength] );
}

SString const SString::Empty("");
String const String::Empty(L"");



const BinaryFormatter_Normal::DbSetHandler::WriteColumnDataFunc_t BinaryFormatter_Normal::DbSetHandler::WriteColumnDataFuncs[] = {
    WriteColumnDataFunc<Byte               >,
    WriteColumnDataFunc<UInt16             >,
    WriteColumnDataFunc<UInt32             >,
    WriteColumnDataFunc<UInt64             >,
    WriteColumnDataFunc<SByte              >,
    WriteColumnDataFunc<Int16              >,
    WriteColumnDataFunc<Int32              >,
    WriteColumnDataFunc<Int64              >,
    WriteColumnDataFunc<Double             >,
    WriteColumnDataFunc<Single             >,
    WriteColumnDataFunc<Char               >,
    WriteColumnDataFunc<Boolean            >,
    WriteColumnDataFunc<DateTime           >,
    WriteColumnDataFunc<Guid               >,
    WriteColumnDataFunc<Decimal            >,
    WriteColumnDataFunc<Binary             >,
    WriteColumnDataFunc<SString            >,
    WriteColumnDataFunc<String             >,
    WriteColumnDataFunc<Nullable<Byte     >>,
    WriteColumnDataFunc<Nullable<UInt16   >>,
    WriteColumnDataFunc<Nullable<UInt32   >>,
    WriteColumnDataFunc<Nullable<UInt64   >>,
    WriteColumnDataFunc<Nullable<SByte    >>,
    WriteColumnDataFunc<Nullable<Int16    >>,
    WriteColumnDataFunc<Nullable<Int32    >>,
    WriteColumnDataFunc<Nullable<Int64    >>,
    WriteColumnDataFunc<Nullable<Double   >>,
    WriteColumnDataFunc<Nullable<Single   >>,
    WriteColumnDataFunc<Nullable<Char     >>,
    WriteColumnDataFunc<Nullable<Boolean  >>,
    WriteColumnDataFunc<Nullable<DateTime >>,
    WriteColumnDataFunc<Nullable<Guid     >>,
    WriteColumnDataFunc<Nullable<Decimal  >>,
    WriteColumnDataFunc<Nullable<Binary   >>,
    WriteColumnDataFunc<Nullable<SString  >>,
    WriteColumnDataFunc<Nullable<String   >>
};

const BinaryFormatter_Normal::DbSetHandler::ReadColumnDataFunc_t BinaryFormatter_Normal::DbSetHandler::ReadColumnDataFuncs[] = {
    ReadColumnDataFunc<Byte               >,
    ReadColumnDataFunc<UInt16             >,
    ReadColumnDataFunc<UInt32             >,
    ReadColumnDataFunc<UInt64             >,
    ReadColumnDataFunc<SByte              >,
    ReadColumnDataFunc<Int16              >,
    ReadColumnDataFunc<Int32              >,
    ReadColumnDataFunc<Int64              >,
    ReadColumnDataFunc<Double             >,
    ReadColumnDataFunc<Single             >,
    ReadColumnDataFunc<Char               >,
    ReadColumnDataFunc<Boolean            >,
    ReadColumnDataFunc<DateTime           >,
    ReadColumnDataFunc<Guid               >,
    ReadColumnDataFunc<Decimal            >,
    ReadColumnDataFunc<Binary             >,
    ReadColumnDataFunc<SString            >,
    ReadColumnDataFunc<String             >,
    ReadColumnDataFunc<Nullable<Byte     >>,
    ReadColumnDataFunc<Nullable<UInt16   >>,
    ReadColumnDataFunc<Nullable<UInt32   >>,
    ReadColumnDataFunc<Nullable<UInt64   >>,
    ReadColumnDataFunc<Nullable<SByte    >>,
    ReadColumnDataFunc<Nullable<Int16    >>,
    ReadColumnDataFunc<Nullable<Int32    >>,
    ReadColumnDataFunc<Nullable<Int64    >>,
    ReadColumnDataFunc<Nullable<Double   >>,
    ReadColumnDataFunc<Nullable<Single   >>,
    ReadColumnDataFunc<Nullable<Char     >>,
    ReadColumnDataFunc<Nullable<Boolean  >>,
    ReadColumnDataFunc<Nullable<DateTime >>,
    ReadColumnDataFunc<Nullable<Guid     >>,
    ReadColumnDataFunc<Nullable<Decimal  >>,
    ReadColumnDataFunc<Nullable<Binary   >>,
    ReadColumnDataFunc<Nullable<SString  >>,
    ReadColumnDataFunc<Nullable<String   >>
};

const BinaryFormatter_Normal::DbSetHandler::CreateColumnDataFunc_t BinaryFormatter_Normal::DbSetHandler::CreateColumnDataFuncs[] = {
    CreateColumnDataFunc<Byte               >,
    CreateColumnDataFunc<UInt16             >,
    CreateColumnDataFunc<UInt32             >,
    CreateColumnDataFunc<UInt64             >,
    CreateColumnDataFunc<SByte              >,
    CreateColumnDataFunc<Int16              >,
    CreateColumnDataFunc<Int32              >,
    CreateColumnDataFunc<Int64              >,
    CreateColumnDataFunc<Double             >,
    CreateColumnDataFunc<Single             >,
    CreateColumnDataFunc<Char               >,
    CreateColumnDataFunc<Boolean            >,
    CreateColumnDataFunc<DateTime           >,
    CreateColumnDataFunc<Guid               >,
    CreateColumnDataFunc<Decimal            >,
    CreateColumnDataFunc<Binary             >,
    CreateColumnDataFunc<SString            >,
    CreateColumnDataFunc<String             >,
    CreateColumnDataFunc<Nullable<Byte     >>,
    CreateColumnDataFunc<Nullable<UInt16   >>,
    CreateColumnDataFunc<Nullable<UInt32   >>,
    CreateColumnDataFunc<Nullable<UInt64   >>,
    CreateColumnDataFunc<Nullable<SByte    >>,
    CreateColumnDataFunc<Nullable<Int16    >>,
    CreateColumnDataFunc<Nullable<Int32    >>,
    CreateColumnDataFunc<Nullable<Int64    >>,
    CreateColumnDataFunc<Nullable<Double   >>,
    CreateColumnDataFunc<Nullable<Single   >>,
    CreateColumnDataFunc<Nullable<Char     >>,
    CreateColumnDataFunc<Nullable<Boolean  >>,
    CreateColumnDataFunc<Nullable<DateTime >>,
    CreateColumnDataFunc<Nullable<Guid     >>,
    CreateColumnDataFunc<Nullable<Decimal  >>,
    CreateColumnDataFunc<Nullable<Binary   >>,
    CreateColumnDataFunc<Nullable<SString  >>,
    CreateColumnDataFunc<Nullable<String   >>
};




}