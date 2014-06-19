#ifndef CONSOLE_HPP
#define CONSOLE_HPP
namespace Lib3 {
namespace Console {





INLINE void SetLocale() {
#ifdef __linux
	setlocale(LC_ALL, "zh_CN.UTF-8");
#else
	setlocale(LC_ALL, "chs");
#endif
}
INLINE void Pause() {
    Write("press any key to continue. . .");
#ifdef __linux
    getchar();
#else
    _getch();
#endif
}
INLINE void WriteNull() {
    Write("(Null)");
}
INLINE void WriteChar(Int32 const &c) {
#ifdef __linux
    printf("%lc", c);
    fflush(stdout);                     // 令 Linux 终端立即输出　而不是等 \n
#else
	putwchar(c);
#endif
}






INLINE void Write(Boolean   const &in) { Write(in ? "true" : "false"); }
INLINE void Write(SByte     const &in) { WriteChar(in); }
INLINE void Write(Char      const &in) { WriteChar(in); }
INLINE void Write(Byte      const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Int16     const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(UInt16    const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Int32     const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(UInt32    const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Int64     const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(UInt64    const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Single    const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Double    const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(DateTime  const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Decimal   const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(Guid      const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert(in, buf) ); }
INLINE void Write(BitArray  const &in) {
    for (UInt32 i = 0; i < in.Length(); ++i) {
        if (i > 0 && i % 8 == 0)
            WriteChar(' ');
        WriteChar(in[i] ? '1' : '0');
    }
}
INLINE void Write(DbTable   const &dt) {
    var cols = dt.ColumnCount();
    var rows = dt.RowCount();
    if (cols == 0) {
        Write("No any columns found!\n");
        return;
    }
    WriteFormat("Columns: %0, Rows: %1\n", cols, rows);
    for (UInt32 i = 0; i < cols; i++) {
        Write(dt.ColumnNames()[i]);
        WriteChar('\t');
    }
    WriteChar('\n');
    for (UInt32 i = 0; i < rows; i++) {
        for (UInt32 j = 0; j < cols; j++) {
            WriteColFuncs[dt.ColumnTypes()[j]](dt.ColumnDatas()[j], i);
            if (j < cols - 1)
                WriteChar('\t');
        }
        WriteChar('\n');
    }
}
INLINE void Write(DbSet     const &ds) {
    var len = ds.Length();
    WriteFormat("Tables: %0, AffectedRows: %1, Message = %2\n", len, ds.AffectedRows(), ds.Message());
    for (UInt32 i = 0; i < len; ++i)
        Write(ds[i]);
}

template<Byte ctid> void WriteColFunc(void * const &list, UInt32 const &idx) {
    typedef typename DbTypes<ctid>::Type T;
    typedef List<T> LT;
    Write((*(LT*)list)[idx]);
}

INLINE void Write(SString const &s) {
    Write(s.Buffer(), s.Length());
}
INLINE void Write(String  const &s) {
    Write(s.Buffer(), s.Length());
}
INLINE void Write(SByte   const * const &s) {
	if (s == NULL) {
		WriteNull();
		return;
	}
    UInt32 i = 0;
    while(var c = s[i++])
        WriteChar(c);
}
INLINE void Write(Char    const * const &s) {
	if (s == NULL) {
		WriteNull();
		return;
	}
    UInt32 i = 0;
    while(var c = s[i++])
        WriteChar(c);
}
INLINE void Write(Binary  const &a) {
    WriteHex(a.Buffer(), a.Length());
}

template<UInt32 len> void Write(SByte const (&s)[len]) {
    for (UInt32 i = 0; i < len; ++i)
        if (var c = s[i])
            WriteChar(c);
        else
            break;
}
template<UInt32 len> void Write(Char  const (&s)[len]) {
    for (UInt32 i = 0; i < len; ++i)
        if (var c = s[i])
            WriteChar(c);
        else
            break;
}
template<UInt32 len> void Write(Byte  const (&s)[len]) {
    WriteHex(s, len);
}


template<typename T> void Write(Nullable<T>   const &nv) {
    if (nv.HasValue())
        Write(nv.Value());
    else
        WriteNull();
}
template<typename T> void Write(Array<T>      const &a ) {
    Write(a.Buffer(), a.Length());
}
template<typename T> void Write(List<T>       const &a ) {
    Write("Count = ");
    Write(a.Length());
    WriteChar(NewLine);
    for (UInt32 i = 0; i < a.Length(); ++i) {
        Write(a[i]);
        WriteChar(NewLine);
    }
}
template<typename T> void Write(LinkedList<T> const &a ) {
    Write("Count = ");
    Write(a.Length());
    WriteChar(NewLine);
    for (UInt32 i = 0; i < a.Length(); ++i) {
        Write(a[i]);
        WriteChar(NewLine);
    }
}
template<typename T> void Write(T             const &v ) {
    v.Dump();
}

template<typename TK, typename TV>
void Write(Dictionary<TK, TV> const &a) {
    Write("Count = ");
    var len = a.Count();
    Write(len);
    WriteChar(NewLine);
    if (len) {
        var p = a.UsedNodeHead();
Lab1:
        Write(p->Key);
        Write(", ");
        Write(p->Value);
        WriteChar(NewLine);
        if ((p = p->UsedNext))
            goto Lab1;
    }
}

template<typename T, UInt32 len>
INLINE void Write(T const (&a)[len]) {
    Write("Count = ");
    Write(len);
    WriteChar(NewLine);
    for (UInt32 i = 0; i < len; ++i) {
        Write(a[i]);
        WriteChar(NewLine);
    }
}

INLINE void Write(SByte const * const &s,   UInt32 const &len) {
	if (s == NULL) {
		WriteNull();
		return;
	}
    if (len == 0)
        return;
	Char C;
	Byte c1, c2, c3;
	UInt32 i = 0;
	do {
		c1 = (Byte)s[i++];
		if (c1 < 0x80u) C = c1;
		else if ((c1 & 0xE0u) == 0xE0u)
		{
            c2 = (Byte)s[i++];
			if (c2) {
                c3 = (Byte)s[i++];
				if (c3)
					C = ((c1 & 0x0Fu) << 12) | ((c2 & 0x3Fu) << 6) | (c3 & 0x3Fu);
				else
                    break;
            }
			else
                break;
		}
		else if ((c1 & 0xC0u) == 0xC0u) {
            c2 = (Byte)s[i++];
			if (c2)
				C = ((c1 & 0x1Fu) << 6) | (c2 & 0x3Fu);
			else
                break;
		}
		WriteChar(C);
	} while (i < len);
}
INLINE void Write(Char  const * const &s,   UInt32 const &len) {
	if (s == NULL) {
		WriteNull();
		return;
	}
	for(UInt32 i = 0; i < len; ++i)
		WriteChar(s[i]);
}
INLINE void Write(Byte  const * const &buf, UInt32 const &len) {
    WriteHex(buf, len);
}
template<typename T>
INLINE void Write(T const * const &in, UInt32 const &len) {
    if (in == NULL) {
		WriteNull();
		return;
	}
    Write("Count = ");
    Write(len);
    WriteChar(NewLine);
    for (UInt32 i = 0; i < len; ++i) {
        Write(in[i]);
        WriteChar(NewLine);
    }
}


INLINE void WriteHex(Byte     const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert_Hex(in, buf) ); }                  
INLINE void WriteHex(UInt16   const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert_Hex(in, buf) ); }                  
INLINE void WriteHex(UInt32   const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert_Hex(in, buf) ); }                  
INLINE void WriteHex(UInt64   const &in) { ALIGN8( SByte buf[64] ); Write( buf, Converts::Convert_Hex(in, buf) ); }                  
INLINE void WriteHex(Byte const * const &s, UInt32 const &len) {
	if (s == NULL) {
		WriteNull();
		return;
	}
    Write("\nLen = ");
    SByte buf[32];
    var chars = Converts::Convert(len, buf);
    Write(buf, chars);
    for(UInt32 i = 0; i < 16 - chars; ++i)
        WriteChar(' ');
    if (len == 0)
        return;
    Write("0  1  2  3  | 4  5  6  7  | 8  9  A  B  | C  D  E  F\n");
    for(UInt32 i = 0; i < len; ++i) {
        if ((i % 16) == 0) {
            if (i)
                WriteChar('\n');
            WriteHex(i);
            Write("              ");
        }
        else if (i && (i % 4 == 0))
            Write("   ");
        else WriteChar(' ');
        WriteHex(s[i]);
    }
    WriteChar('\n');
    //Write("----------------------------------+-------------+-------------+------------\n");
}
template<typename T>
INLINE void WriteHex(T const &v) {
    v.DumpHex();
}












template<UInt32 len >
void WriteFormat(Char const (&s)[len]) {
    Write(s);
}
template<UInt32 len , typename T0>
void WriteFormat(Char const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P0
}
template<UInt32 len , typename T0, typename T1>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1) {
    #define CS_P1
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P1
}
template<UInt32 len , typename T0, typename T1, typename T2>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P2
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P3
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P4
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P5
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P6
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P7
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P8
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P9
}





template<UInt32 len>
void WriteFormat(SByte const (&s)[len]) {
    Write(s);
}
template<UInt32 len, typename T0>
void WriteFormat(SByte const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/ConsoleWF.inc"
    #undef CS_P0
}
template<UInt32 len, typename T0, typename T1>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1) {
    #define CS_P1
    #include "../INC/ConsoleWF.inc"
    #undef CS_P1
}
template<UInt32 len, typename T0, typename T1, typename T2>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/ConsoleWF.inc"
    #undef CS_P2
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/ConsoleWF.inc"
    #undef CS_P3
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/ConsoleWF.inc"
    #undef CS_P4
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/ConsoleWF.inc"
    #undef CS_P5
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/ConsoleWF.inc"
    #undef CS_P6
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/ConsoleWF.inc"
    #undef CS_P7
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/ConsoleWF.inc"
    #undef CS_P8
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/ConsoleWF.inc"
    #undef CS_P9
}






template<typename T>
void WriteFormat(T const &t) {
    Write(t);
}
template<typename T0>
void WriteFormat(SString const &str, T0 const &p0) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P0
    #include "../INC/ConsoleWF.inc"
    #undef CS_P0
}
template<typename T0, typename T1>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P1
    #include "../INC/ConsoleWF.inc"
    #undef CS_P1
}
template<typename T0, typename T1, typename T2>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P2
    #include "../INC/ConsoleWF.inc"
    #undef CS_P2
}
template<typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P3
    #include "../INC/ConsoleWF.inc"
    #undef CS_P3
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P4
    #include "../INC/ConsoleWF.inc"
    #undef CS_P4
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P5
    #include "../INC/ConsoleWF.inc"
    #undef CS_P5
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P6
    #include "../INC/ConsoleWF.inc"
    #undef CS_P6
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P7
    #include "../INC/ConsoleWF.inc"
    #undef CS_P7
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P8
    #include "../INC/ConsoleWF.inc"
    #undef CS_P8
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P9
    #include "../INC/ConsoleWF.inc"
    #undef CS_P9
}





template<typename T0>
void WriteFormat(String const &str, T0 const &p0) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P0
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P0
}
template<typename T0, typename T1>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P1
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P1
}
template<typename T0, typename T1, typename T2>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P2
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P2
}
template<typename T0, typename T1, typename T2, typename T3>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P3
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P3
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P4
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P4
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P5
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P5
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P6
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P6
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P7
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P7
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P8
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P8
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P9
    #include "../INC/ConsoleWFW.inc"
    #undef CS_P9
}






























}}
#endif
