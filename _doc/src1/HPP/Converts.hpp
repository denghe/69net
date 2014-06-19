#ifndef CONVERTS_HPP
#define CONVERTS_HPP
namespace Lib3 {
namespace Converts {






// 取 int32 转换后的长度
INLINE UInt32 GetLength(UInt32 const &in) {
    // todo: if ( n < ...... ) 这种代码理论上讲可以优化成树形, 类似折半查找从而减少 if 次数
    if (in < 10L) return 1;
    else if (in < 100L) return 2;
    else if (in < 1000L) return 3;
    else if (in < 10000L) return 4;
    else if (in < 100000L) return 5;
    else if (in < 1000000L) return 6;
    else if (in < 10000000L) return 7;
    else if (in < 100000000L) return 8;
    else if (in < 1000000000L) return 9;
    else return 10;
}

// 取 int64 转换后的长度
INLINE UInt32 GetLength(UInt64 const &in) {
    if      (in < 10000000000L) return GetLength((UInt32)in);
    else if (in < 100000000000L) return 11;
    else if (in < 1000000000000L) return 12;
    else if (in < 10000000000000L) return 13;
    else if (in < 100000000000000L) return 14;
    else if (in < 1000000000000000L) return 15;
    else if (in < 10000000000000000L) return 16;
    else if (in < 100000000000000000L) return 17;
    else if (in < 1000000000000000000L) return 18;
    else if (in < (UInt64)((UInt64)1000000000000000000L * (UInt64)10)) return 19;
    else return 20;
}


// 将 10 进制数值字串转为相应的值类型
//


// 下面两个函数为内部使用
	
// string 转为各种长度的 有符号整数. Out 取值范围： SByte, Int16, Int32, Int64
template <typename Out, typename CharType>
INLINE void ToInt(CharType const * const &in, Out &out) {
    assert ( in );
    var in_ = in;
    if (*in_ == '0') {
        out = 0;
        return;
    }
    bool b;
    if (*in_ == '-') {
        b = true;
        ++in_;
    }
    else b = false;
    Out r = *(in_) - '0';
    CharType c;
    while ((c = *(++in_))) r = r * 10 + (c - '0');
    out = b ? -r : r;
}

// string (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： Byte, UInt16, UInt32, UInt64
template <typename Out, typename CharType>
INLINE void ToUInt(CharType const * const &in, Out &out) {
    assert ( in );
    var in_ = in;
    if (*in_ == '0') {
        out = 0;
        return;
    }
    Out r = *(in_) - '0';
    CharType c;
    while ((c = *(++in_))) r = r * 10 + (c - '0');
    out = r;
}

INLINE void ConvertCore(SByte const * const &in, Byte     &out) {
    ToUInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, UInt16   &out) {
    ToUInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, UInt32   &out) {
    ToUInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, UInt64   &out) {
    ToUInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, SByte    &out) {
    ToInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, Int16    &out) {
    ToInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, Int32    &out) {
    ToInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, Int64    &out) {
    ToInt(in, out);
}
INLINE void ConvertCore(SByte const * const &in, Double   &out) {
    out = strtod(in, Null);
}
INLINE void ConvertCore(SByte const * const &in, Single   &out) {
    out = (Single)strtod(in, Null);
}
INLINE void ConvertCore(SByte const * const &in, Boolean  &out) {
    out = (in[0] == '1' || in[0] == 'T' || in[0] == 't');
}
INLINE void ConvertCore(SByte const * const &in, Char     &out) {
    Byte c = in[0];
    if (c < 0x80u) {
        out = c;
        return;
    }
    else if ((c & 0xE0) == 0xE0) {
        out = ((c & 0x0F) << 12) | ((in[1] & 0x3F) << 6) | (in[2] & 0x3F);
        return;
    }
    else if ((c & 0xC0u) == 0xC0u) {
        out = ((c & 0x1F) << 6) | (in[1] & 0x3F);
        return;
    }
}
INLINE void ConvertCore(SByte const * const &in, DateTime &out) {
    out.Year    = (in[0 ] - 48) * 1000 + (in[1] - 48) * 100 + (in[2] - 48) * 10 + (in[3] - 48);
    out.Month   = (in[5 ] - 48) * 10 + (in[6 ] - 48);
    out.Day     = (in[8 ] - 48) * 10 + (in[9 ] - 48);
    out.Hour    = (in[11] - 48) * 10 + (in[12] - 48);
    out.Minute  = (in[14] - 48) * 10 + (in[15] - 48);
    out.Second  = (in[17] - 48) * 10 + (in[18] - 48);
}
INLINE void ConvertCore(SByte const * const &in, Guid     &out) {
    out.Set(in);
}
// 步骤: 
// 1. 扫负数 / 0 : 记录 / 或置 0. 进一步判断是否有小数点, 跳出或 goto 4
// 2. 扫数字, 判 ., 没遇到 . 就 I = I * 10 + 数字
// 3. 遇到 . 开始处理小数部分
// 4. 小数位数计数 开扫, 类似上面的操作 D = D * 10 + 数字
// 5. 负数的话 I = -I;
INLINE void ConvertCore(SByte const * const &in, Decimal  &out) {
    var in_ = in;
    var c = *in_++;
    var b = false;
    if (c == '-') {
        b = true;
        c = *in_++;
    }
    var I = 0;
    if (c == '0') {
        out.Value() = 0;
        if (*in_++) goto Digi;
        return;
    }
    I = c - '0';
    while ((c = *in_++)) {
        if (c == '.') goto Digi;
        I = I * 10 + (c - '0');
    }
    out.Set(b ? -I : I);
    return;
Digi:
    c = *in_++;
    var D = c - '0';
    while ((c = *in_++)) 
        D = D * 10 + (c - '0');
    out.Set((b ? -I : I), D);
}





INLINE void Convert(SByte const * const &in, Byte     &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, UInt16   &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, UInt32   &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, UInt64   &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, SByte    &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Int16    &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Int32    &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Int64    &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Double   &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Single   &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Char     &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Boolean  &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, DateTime &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Guid     &out) { assert ( in ); ConvertCore(in, out); }
INLINE void Convert(SByte const * const &in, Decimal  &out) { assert ( in ); ConvertCore(in, out); }



template<typename CharType> INLINE UInt32 ConvertCore(UInt32 const &in, CharType * const &out) {
    if (in) {
        UInt32 idx, len;
        len = GetLength(in);
        idx = len;
        UInt64 v = in, tmp;
        do {
            if (sizeof(size_t) > 4)
                tmp = (v * 1717986919) >> 34;			// v / 10的高效率版
            else
                tmp = v / 10;
            out[--idx] = (CharType)(48 + v - tmp * 10);
        } while (v = tmp);
        return len;
    }
    out[0] = '0';
    return 1;
}
template<typename CharType> INLINE UInt32 ConvertCore(UInt64 const &in, CharType * const &out) {
    if (in) {
        if (in <= 0xFFFFFFFFL)
            return Convert((UInt32)in, out);
        UInt32 len, idx;
        len = GetLength(in);
        idx = len;
        UInt64 v = in, tmp;
        do {
            tmp = v / 10;
            out[--idx] = (CharType)(48 + v - tmp * 10);
        } while(v = tmp);
        return len;
    }
    out[0] = '0';
    return 1;
}
template<typename CharType> INLINE UInt32 ConvertCore(Int32  const &in, CharType * const &out) {
    if(in < 0) {
        *out = '-';
        return ConvertCore((UInt32)-in, out + 1) + 1;
    }
    return ConvertCore((UInt32)in, out);
}
template<typename CharType> INLINE UInt32 ConvertCore(Int64  const &in, CharType * const &out) {
    if (in < 0) {
        *out = '-';
        return ConvertCore((UInt64)-in, out + 1) + 1;
    }
    return ConvertCore((UInt64)in, out);
}

const SByte _string_true[] = "true";
const SByte _string_false[] = "false";
template<typename CharType>
INLINE UInt32 ConvertCore(Boolean const &in, CharType * const &out) {
    if (in) {
        out[0] = _string_true[0];
        out[1] = _string_true[1];
        out[2] = _string_true[2];
        out[3] = _string_true[3];
        return 4;
    } else {
        out[0] = _string_false[0];
        out[1] = _string_false[1];
        out[2] = _string_false[2];
        out[3] = _string_false[3];
        out[4] = _string_false[4];
        return 5;
    }
}
INLINE UInt32 ConvertCore(Double  const &in, SByte *    const &out) {
#ifdef __linux
    return sprintf(out, "%.17g", in);
#else
    return sprintf_s(out, 32, "%.17g", in);
#endif
}
INLINE UInt32 ConvertCore(Single  const &in, SByte *    const &out) {
#ifdef __linux
    return sprintf(out, "%.17g", in);
#else
    return sprintf_s(out, 32, "%.17g", in);
#endif
}
INLINE UInt32 ConvertCore(Double  const &in, Char *     const &out) {
#ifdef __linux
    return swprintf(out, 32, L"%.17g", in);
#else
    return swprintf_s(out, 32, L"%.17g", in);
#endif
}
INLINE UInt32 ConvertCore(Single  const &in, Char *     const &out) {
#ifdef __linux
    return swprintf(out, 32, L"%.17g", in);
#else
    return swprintf_s(out, 32, L"%.17g", in);
#endif
}
INLINE UInt32 ConvertCore(Char    const &in, SByte *    const &out) {
    return Convert_Utf8(in, out);
}
INLINE UInt32 ConvertCore(Char    const &in, Char *     const &out) {
    out[0] = in;
    return 1;
}

template<typename CharType> INLINE UInt32 ConvertCore(DateTime const &in, CharType * const &out) {
    UInt32 num = in.Year;
    out[0 ] = '0' +  num / 1000;
    out[1 ] = '0' + (num / 100) % 10;
    out[2 ] = '0' + (num / 10) % 10;
    out[3 ] = '0' +  num % 10;
    out[4 ] = '-';
    num = in.Month;
    out[5 ] = '0' + (num / 10) % 10;
    out[6 ] = '0' +  num % 10;
    out[7 ] = '-';
    num = in.Day;
    out[8 ] = '0' + (num / 10) % 10;
    out[9 ] = '0' +  num % 10;
    out[10] = ' ';
    num = in.Hour;
    out[11] = '0' + (num / 10) % 10;
    out[12] = '0' +  num % 10;
    out[13] = ':';
    num = in.Minute;
    out[14] = '0' + (num / 10) % 10;
    out[15] = '0' +  num % 10;
    out[16] = ':';
    num = in.Second;
    out[17] = '0' + (num / 10) % 10;
    out[18] = '0' +  num % 10;
    return 19;
}
template<typename CharType> INLINE UInt32 ConvertCore(Guid     const &in, CharType * const &out) {
    var p = (Byte*)&in;
    Convert_Hex(*(UInt32*)p, out);
    out[8] = '-';
    Convert_Hex(*(UInt16*)(p + 4), out + 9);
    out[13] = '-';
    Convert_Hex(*(UInt16*)(p + 6), out + 14);
    out[18] = '-';
    Convert_Hex(*(UInt16*)(p + 8), out + 19);
    out[23] = '-';
    Convert_Hex(*(p + 10), out + 24);
    Convert_Hex(*(p + 11), out + 26);
    Convert_Hex(*(p + 12), out + 28);
    Convert_Hex(*(p + 13), out + 30);
    Convert_Hex(*(p + 14), out + 32);
    Convert_Hex(*(p + 15), out + 34);
    return 36;
}
template<typename CharType> INLINE UInt32 ConvertCore(Decimal  const &in, CharType * const &out) {
    var len = ConvertCore(in.Int(), out);
    if (var digi = in.Digi()) {
        out[len] = '.';
        return len + ConvertCore(digi, out + len + 1) + 1;
    }
    return len;
}






INLINE UInt32 Convert(Byte     const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt16   const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt32   const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt64   const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(SByte    const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int16    const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int32    const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int64    const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Double   const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Single   const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Char     const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Boolean  const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(DateTime const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Guid     const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Decimal  const &in, SByte * const &out) { assert ( out ); return ConvertCore(in, out); }

INLINE UInt32 Convert(Byte     const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt16   const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt32   const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(UInt64   const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(SByte    const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int16    const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int32    const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Int64    const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Double   const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Single   const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Char     const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Boolean  const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(DateTime const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Guid     const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }
INLINE UInt32 Convert(Decimal  const &in, Char * const &out) { assert ( out ); return ConvertCore(in, out); }





template<typename CharType> INLINE UInt32 ToHex(Byte   const &in, CharType * const &out) {
    assert ( out );
	var &p = *(NumBits::Size1*)&in;
	out[0] = Int2HexTable[p.Byte0_H];
	out[1] = Int2HexTable[p.Byte0_L];
	return 2;
}
template<typename CharType> INLINE UInt32 ToHex(UInt16 const &in, CharType * const &out) {
    assert ( out );
	var &p = *(NumBits::Size2*)&in;
	out[0] = Int2HexTable[p.Byte1_H];
	out[1] = Int2HexTable[p.Byte1_L];
	out[2] = Int2HexTable[p.Byte0_H];
	out[3] = Int2HexTable[p.Byte0_L];
	return 4;
}
template<typename CharType> INLINE UInt32 ToHex(UInt32 const &in, CharType * const &out) {
    assert ( out );
	var &p = *(NumBits::Size4*)&in;
	out[0] = Int2HexTable[p.Byte3_H];
	out[1] = Int2HexTable[p.Byte3_L];
	out[2] = Int2HexTable[p.Byte2_H];
	out[3] = Int2HexTable[p.Byte2_L];
	out[4] = Int2HexTable[p.Byte1_H];
	out[5] = Int2HexTable[p.Byte1_L];
	out[6] = Int2HexTable[p.Byte0_H];
	out[7] = Int2HexTable[p.Byte0_L];
	return 8;
}
template<typename CharType> INLINE UInt32 ToHex(UInt64 const &in, CharType * const &out) {
    assert ( out );
	var &p = *(NumBits::Size8*)&in;
	out[0]  = Int2HexTable[p.Byte7_H];
	out[1]  = Int2HexTable[p.Byte7_L];
	out[2]  = Int2HexTable[p.Byte6_H];
	out[3]  = Int2HexTable[p.Byte6_L];
	out[4]  = Int2HexTable[p.Byte5_H];
	out[5]  = Int2HexTable[p.Byte5_L];
	out[6]  = Int2HexTable[p.Byte4_H];
	out[7]  = Int2HexTable[p.Byte4_L];
	out[8]  = Int2HexTable[p.Byte3_H];
	out[9]  = Int2HexTable[p.Byte3_L];
	out[10] = Int2HexTable[p.Byte2_H];
	out[11] = Int2HexTable[p.Byte2_L];
	out[12] = Int2HexTable[p.Byte1_H];
	out[13] = Int2HexTable[p.Byte1_L];
	out[14] = Int2HexTable[p.Byte0_H];
	out[15] = Int2HexTable[p.Byte0_L];
	return 16;
}
template<typename CharType> INLINE UInt32 ToHex(Guid   const &in, CharType * const &out) {
    var p = (Byte*)&in;
    Convert_Hex(*(UInt32*)p, out);
    Convert_Hex(*(UInt16*)(p + 4), out + 8);
    Convert_Hex(*(UInt16*)(p + 6), out + 12);
    Convert_Hex(*(p +  8), out + 16);
    Convert_Hex(*(p +  9), out + 18);
    Convert_Hex(*(p + 10), out + 20);
    Convert_Hex(*(p + 11), out + 22);
    Convert_Hex(*(p + 12), out + 24);
    Convert_Hex(*(p + 13), out + 26);
    Convert_Hex(*(p + 14), out + 28);
    Convert_Hex(*(p + 15), out + 30);
    return 32;
}



// 下面 return 0 是未完成的


INLINE UInt32 Convert_Hex(Byte     const &in, SByte * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt16   const &in, SByte * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt32   const &in, SByte * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt64   const &in, SByte * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(SByte    const &in, SByte * const &out) { assert ( out ); return ToHex((Byte   const &)in, out); }
INLINE UInt32 Convert_Hex(Int16    const &in, SByte * const &out) { assert ( out ); return ToHex((UInt16 const &)in, out); }
INLINE UInt32 Convert_Hex(Int32    const &in, SByte * const &out) { assert ( out ); return ToHex((UInt32 const &)in, out); }
INLINE UInt32 Convert_Hex(Int64    const &in, SByte * const &out) { assert ( out ); return ToHex((UInt64 const &)in, out); }
INLINE UInt32 Convert_Hex(Double   const &in, SByte * const &out) { assert ( out ); return ToHex(*(UInt64*)&in, out); }
INLINE UInt32 Convert_Hex(Single   const &in, SByte * const &out) { assert ( out ); return ToHex(*(UInt32*)&in, out); }
INLINE UInt32 Convert_Hex(Char     const &in, SByte * const &out) { assert ( out ); return ToHex((UInt32 const &)in, out); }
INLINE UInt32 Convert_Hex(Boolean  const &in, SByte * const &out) { assert ( out ); return ToHex((Byte const &)in, out); }
INLINE UInt32 Convert_Hex(DateTime const &in, SByte * const &out) { assert ( out ); return ToHex((UInt64 const &)in.Value, out); }
INLINE UInt32 Convert_Hex(Guid     const &in, SByte * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(Decimal  const &in, SByte * const &out) { assert ( out ); return ToHex(in.Value(), out); }

INLINE UInt32 Convert_Hex(Byte     const &in, Char * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt16   const &in, Char * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt32   const &in, Char * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(UInt64   const &in, Char * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(SByte    const &in, Char * const &out) { assert ( out ); return ToHex((Byte   const &)in, out); }
INLINE UInt32 Convert_Hex(Int16    const &in, Char * const &out) { assert ( out ); return ToHex((UInt16 const &)in, out); }
INLINE UInt32 Convert_Hex(Int32    const &in, Char * const &out) { assert ( out ); return ToHex((UInt32 const &)in, out); }
INLINE UInt32 Convert_Hex(Int64    const &in, Char * const &out) { assert ( out ); return ToHex((UInt64 const &)in, out); }
INLINE UInt32 Convert_Hex(Double   const &in, Char * const &out) { assert ( out ); return ToHex(*(UInt64*)&in, out); }
INLINE UInt32 Convert_Hex(Single   const &in, Char * const &out) { assert ( out ); return ToHex(*(UInt32*)&in, out); }
INLINE UInt32 Convert_Hex(Char     const &in, Char * const &out) { assert ( out ); return ToHex((UInt32 const &)in, out); }
INLINE UInt32 Convert_Hex(Boolean  const &in, Char * const &out) { assert ( out ); return ToHex((Byte const &)in, out); }
INLINE UInt32 Convert_Hex(DateTime const &in, Char * const &out) { assert ( out ); return ToHex((UInt64 const &)in.Value, out); }
INLINE UInt32 Convert_Hex(Guid     const &in, Char * const &out) { assert ( out ); return ToHex(in, out); }
INLINE UInt32 Convert_Hex(Decimal  const &in, Char * const &out) { assert ( out ); return ToHex(in.Value(), out); }




template<typename CharType> INLINE void HexToInt(CharType const * const &in, Byte   &out) {
    assert ( in );
	var &p = *(NumBits::Size1*)&out;
	p.Byte0_H = Hex2IntTable[(UInt32)in[0]];
	p.Byte0_L = Hex2IntTable[(UInt32)in[1]];
}
template<typename CharType> INLINE void HexToInt(CharType const * const &in, UInt16 &out) {
    assert ( in );
	var &p = *(NumBits::Size2*)&out;
	p.Byte1_H = Hex2IntTable[(UInt32)in[0]];
	p.Byte1_L = Hex2IntTable[(UInt32)in[1]];
	p.Byte0_H = Hex2IntTable[(UInt32)in[2]];
	p.Byte0_L = Hex2IntTable[(UInt32)in[3]];
}
template<typename CharType> INLINE void HexToInt(CharType const * const &in, UInt32 &out) {
    assert ( in );
	var &p = *(NumBits::Size4*)&out;
	p.Byte3_H = Hex2IntTable[(UInt32)in[0]];
	p.Byte3_L = Hex2IntTable[(UInt32)in[1]];
	p.Byte2_H = Hex2IntTable[(UInt32)in[2]];
	p.Byte2_L = Hex2IntTable[(UInt32)in[3]];
	p.Byte1_H = Hex2IntTable[(UInt32)in[4]];
	p.Byte1_L = Hex2IntTable[(UInt32)in[5]];
	p.Byte0_H = Hex2IntTable[(UInt32)in[6]];
	p.Byte0_L = Hex2IntTable[(UInt32)in[7]];
}
template<typename CharType> INLINE void HexToInt(CharType const * const &in, UInt64 &out) {
    assert ( in );
	var &p = *(NumBits::Size8*)&out;
	p.Byte7_H = Hex2IntTable[(UInt32)in[0]];
	p.Byte7_L = Hex2IntTable[(UInt32)in[1]];
	p.Byte6_H = Hex2IntTable[(UInt32)in[2]];
	p.Byte6_L = Hex2IntTable[(UInt32)in[3]];
	p.Byte5_H = Hex2IntTable[(UInt32)in[4]];
	p.Byte5_L = Hex2IntTable[(UInt32)in[5]];
	p.Byte4_H = Hex2IntTable[(UInt32)in[6]];
	p.Byte4_L = Hex2IntTable[(UInt32)in[7]];
	p.Byte3_H = Hex2IntTable[(UInt32)in[8]];
	p.Byte3_L = Hex2IntTable[(UInt32)in[9]];
	p.Byte2_H = Hex2IntTable[(UInt32)in[10]];
	p.Byte2_L = Hex2IntTable[(UInt32)in[11]];
	p.Byte1_H = Hex2IntTable[(UInt32)in[12]];
	p.Byte1_L = Hex2IntTable[(UInt32)in[13]];
	p.Byte0_H = Hex2IntTable[(UInt32)in[14]];
	p.Byte0_L = Hex2IntTable[(UInt32)in[15]];
}

INLINE void Convert_Hex(SByte const * const in, Byte   &out) { assert ( in ); HexToInt(in, out); }
INLINE void Convert_Hex(SByte const * const in, UInt16 &out) { assert ( in ); HexToInt(in, out); }
INLINE void Convert_Hex(SByte const * const in, UInt32 &out) { assert ( in ); HexToInt(in, out); }
INLINE void Convert_Hex(SByte const * const in, UInt64 &out) { assert ( in ); HexToInt(in, out); }



// Unicode , Utf8 互转 （当前只实现了 UCS2 的字符集转换，即：2 字节）

INLINE UInt32 Convert_Unicode_GetLength(SByte const * const &in) {
    assert ( in );
    var in_ = in;
    UInt32 count = 0;
    while (Byte c = *in_++) {
        if (c < 0x80u) { }
        else if ((c & 0xE0) == 0xE0) in_ += 2;
        else if ((c & 0xC0u) == 0xC0u) in_++;
        count++;
    }
    return count;
}
INLINE UInt32 Convert_Utf8_GetLength(Char const * const &in) {
    assert ( in );
    var in_ = in;
    UInt32 count = 0;
    while (Char c = *in_++) {
        if (c < 0x80) count++;
        else if (c < 0x800) count +=2;
        else count += 3;
    }
    return count;
}
INLINE UInt32 Convert_Unicode(SByte const * const &inStr, Char * const &outWStr) {
    assert ( inStr && outWStr );
    var outWStr_ = outWStr;
    var inStr_ = inStr;
    var bak = outWStr;
    while (Byte c = *inStr_++) {
        if (c < 0x80u) *outWStr_++ = c;
        else if ((c & 0xE0) == 0xE0) {
            *outWStr_++ = ((c & 0x0F) << 12) | (((inStr_[0]) & 0x3F) << 6) | ((inStr_[1]) & 0x3F);
            inStr_ += 2;
        }
        else if ((c & 0xC0u) == 0xC0u) {
            *outWStr_++ = ((c & 0x1F) << 6) | (*inStr_++ & 0x3F);
        }
    }
    return (UInt32)(outWStr_ - bak);
}
INLINE UInt32 Convert_Utf8(Char const * const &inStr, SByte * const &outWStr) {
    assert ( inStr && outWStr );
    var inStr_ = inStr;
    UInt32 idx = 0;                                         // 用下标访问 Release 版里要快丁点. 但如果把 in 也搞成下标访问　反而会更慢
    while (Char c = *inStr_++) {
        if (c < 0x80) outWStr[idx++] = (Byte)c;            // 0xxx xxxx
        else if (c < 0x800) {                               // 110x xxxx 10xx xxxx
            outWStr[idx + 0] = ((c >> 6) & 0x1F) | 0xC0;    // 1f: 0001 1111    c0: 1100 0000
            outWStr[idx + 1] = (c & 0x3F) | 0x80;           // 3f: 0011 1111    80: 1000 0000
            idx += 2;
        }
        else {                                              // 1110 xxxx 10xx xxxx 10xx xxxx
            outWStr[idx + 0] = ((c >> 12) & 0x0F) | 0xE0;   // 0f: 0000 1111    e0: 1110 0000
            outWStr[idx + 1] = ((c >> 6) & 0x3F) | 0x80;    // 3f: 0011 1111    80: 1000 0000
            outWStr[idx + 2] = (c & 0x3F) | 0x80;           // 3f: 0011 1111    80: 1000 0000
            idx += 3;
        }
    }
    return idx;
}
INLINE UInt32 Convert_Unicode_GetLength(SByte const * const &inStr, UInt32 const &inLen) {
    assert ( inStr );
    UInt32 count = 0, i = 0;
    do {
        Byte c = inStr[i++];
        if (c < 0x80u) { }
        else if ((c & 0xE0) == 0xE0) i += 2;
        else if ((c & 0xC0u) == 0xC0u) ++i;
        count++;
    } while (i < inLen);
    return count;

}
INLINE UInt32 Convert_Utf8_GetLength(Char const * const &inStr, UInt32 const &inLen) {
    assert ( inStr );
    UInt32 count = 0, i = 0;
    do {
        Char c = inStr[i++];
        if (c < 0x80) count++;
        else if (c < 0x800) count +=2;
        else count += 3;
    } while (i < inLen);
    return count;
}
INLINE UInt32 Convert_Unicode(SByte const * const &inStr, UInt32 const &inLen, Char * const &outWStr) {
    assert ( inStr && outWStr );
    var outWStr_ = outWStr;
    var bak = outWStr;
    size_t i = 0;
    do {
        Byte c = inStr[i++];
        if (c < 0x80u) *outWStr_++ = c;
        else if ((c & 0xE0) == 0xE0) {
            *outWStr_++ = ((c & 0x0F) << 12) | (((inStr[i]) & 0x3F) << 6) | ((inStr[i + 1]) & 0x3F);
            i += 2;
        }
        else if ((c & 0xC0u) == 0xC0u) {
            *outWStr_++ = ((c & 0x1F) << 6) | (inStr[i++] & 0x3F);
        }
    } while (i < inLen);
    return (UInt32)(outWStr_ - bak);
}
INLINE UInt32 Convert_Utf8(Char const * const &inWStr, UInt32 const &inLen, SByte * const &outStr) {
    assert ( inWStr && outStr );
    UInt32 idx = 0, i = 0;
    do {
        Char c = inWStr[i++];
        if (c < 0x80) outStr[idx++] = (Byte)c;       
        else if (c < 0x800) {							
            outStr[idx + 0] = ((c >> 6) & 0x1F) | 0xC0; 
            outStr[idx + 1] = (c & 0x3F) | 0x80;
            idx += 2;
        }
        else {											
            outStr[idx + 0] = ((c >> 12) & 0x0F) | 0xE0;
            outStr[idx + 1] = ((c >> 6) & 0x3F) | 0x80; 
            outStr[idx + 2] = (c & 0x3F) | 0x80;
            idx += 3;
        }
    } while (i < inLen);
    return idx;
}
INLINE UInt32 Convert_Utf8(Char const &in, SByte * const &out) {
    assert ( out );
    UInt32 idx = 0;
    if (in < 0x80) out[idx++] = (Byte)in;
    else if (in < 0x800) {							
        out[idx + 0] = ((in >> 6) & 0x1F) | 0xC0; 
        out[idx + 1] = (in & 0x3F) | 0x80;
        idx += 2;
    }
    else {											
        out[idx + 0] = ((in >> 12) & 0x0F) | 0xE0;
        out[idx + 1] = ((in >> 6) & 0x3F) | 0x80; 
        out[idx + 2] = (in & 0x3F) | 0x80;
        idx += 3;
    }
    return idx;
}






}}
#endif
