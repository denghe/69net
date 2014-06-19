#ifndef BINARYFORMATTER_NORMAL_HPP
#define BINARYFORMATTER_NORMAL_HPP
namespace Lib3 {



INLINE Byte   BinaryFormatter_Normal::ZegZagEncode(SByte  const &i) {
    return ( i << 1 ) ^ ( i >> 7 );
}
INLINE UInt16 BinaryFormatter_Normal::ZegZagEncode(Int16  const &i) {
    return ( i << 1 ) ^ ( i >> 15 );
}
INLINE UInt32 BinaryFormatter_Normal::ZegZagEncode(Int32  const &i) {
    return ( i << 1 ) ^ ( i >> 31 );
}
INLINE UInt64 BinaryFormatter_Normal::ZegZagEncode(Int64  const &i) {
    return ( i << 1 ) ^ ( i >> 63 );
}
INLINE SByte  BinaryFormatter_Normal::ZegZagDecode(Byte   const &i) {
    return (SByte)( i >> 1 ) ^ ( -(SByte)( i & 1 ) );
}
INLINE Int16  BinaryFormatter_Normal::ZegZagDecode(UInt16 const &i) {
    return (Int16)( i >> 1 ) ^ ( -(Int16)( i & 1 ) );
}
INLINE Int32  BinaryFormatter_Normal::ZegZagDecode(UInt32 const &i) {
    return (Int32)( i >> 1 ) ^ ( -(Int32)( i & 1 ) );
}
INLINE Int64  BinaryFormatter_Normal::ZegZagDecode(UInt64 const &i) {
    return (Int64)( i >> 1 ) ^ ( -(Int64)( i & 1 ) );
}

INLINE void BinaryFormatter_Normal::Write7Bit(Binary &bin, UInt64 const &in) {
    bin.EnsureSpace(9);
    var p = bin.Buffer() + bin.Length();
    UInt64 i = in;
    var idx = 0;
Lab1:
    var b7 = i & 0x7F;
    if ((i >>= 7)) {
        p[idx++] = (Byte)(b7 | 0x80);
        if (idx == 8)
            p[idx++] = (Byte)i;
        else
            goto Lab1;
    } else
        p[idx++] = (Byte)b7;
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::Write7Bit(Binary &bin, UInt32 const &in) {
    bin.EnsureSpace(5);
    var p = bin.Buffer() + bin.Length();
    UInt32 i = in;
    var idx = 0;
Lab1:
    var b7 = i & 0x7F;
    if ((i >>= 7)) {
        p[idx++] = (Byte)(b7 | 0x80);
        goto Lab1;
    } else
        p[idx++] = (Byte)b7;
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::Write7Bit(Binary &bin, UInt16 const &in) {
    bin.EnsureSpace(3);
    var p = bin.Buffer() + bin.Length();
    UInt32 i = in;
    var idx = 0;
Lab1:
    var b7 = i & 0x7F;
    if ((i >>= 7)) {
        p[idx++] = (Byte)(b7 | 0x80);
        goto Lab1;
    } else
        p[idx++] = (Byte)b7;
    bin.Length() += idx;
}

INLINE ConvertStates BinaryFormatter_Normal::Read7Bit(Binary &bin, UInt64 &out) {
    if (bin.Offset() >= bin.Length())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    UInt64 i = 0, b7;
    UInt32 lshift = 0, idx = 0, len = bin.Length() - bin.Offset();
Lab1:
    b7 = p[idx++];
    if (b7 > 0x7F) {
        if (idx >= len)
            return ConvertState_NotEnoughData;
        if (idx < 8) {
            i |= (b7 & 0x7F) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else
            i |= (b7 << lshift) | ((UInt64)p[idx++] << 28 << 28);
    } else
        i |= b7 << lshift;
    out = i;
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read7Bit(Binary &bin, UInt32 &out) {
    if (bin.Offset() >= bin.Length())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    UInt32 i = 0, b7, lshift = 0, idx = 0, len = bin.Length() - bin.Offset();
Lab1:
    b7 = p[idx++];
    if (b7 > 0x7F) {
        if (idx == 5)
            return ConvertState_Overflow;
        if (idx >= len)
            return ConvertState_NotEnoughData;
        i |= (b7 & 0x7F) << lshift;
        lshift += 7;
        goto Lab1;
    } else {
        if (idx == 5 && b7 > 15)
            return ConvertState_Overflow;
        else
            i |= b7 << lshift;
    }
    out = i;
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read7Bit(Binary &bin, UInt16 &out) {
    if (bin.Offset() >= bin.Length())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    UInt32 i = 0, b7, lshift = 0, idx = 0, len = bin.Length() - bin.Offset();
Lab1:
    b7 = p[idx++];
    if (b7 > 0x7F) {
        if (idx == 3)
            return ConvertState_Overflow;
        if (idx >= len)
            return ConvertState_NotEnoughData;
        i |= (b7 & 0x7F) << lshift;
        lshift += 7;
        goto Lab1;
    } else {
        if (idx == 3 && b7 > 3)
            return ConvertState_Overflow;
        else
            i |= b7 << lshift;
    }
    out = UInt16(i);
    bin.Offset() += idx;
    return ConvertState_Success;
}

INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, UInt64 const &in) {
    bin.EnsureSpace(9);
    var p = bin.Buffer() + bin.Length();
    if (in == 0) {
        *p = 0;
        ++bin.Length();
        return;
    }
    Byte flag = 0;
    var &v = *(NumBits::Size8*)&in;
    var idx = 1;
    if (v.Byte0) { flag |= 1  ; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2  ; p[idx++] = v.Byte1; }
    if (v.Byte2) { flag |= 4  ; p[idx++] = v.Byte2; }
    if (v.Byte3) { flag |= 8  ; p[idx++] = v.Byte3; }
    if (v.Byte4) { flag |= 16 ; p[idx++] = v.Byte4; }
    if (v.Byte5) { flag |= 32 ; p[idx++] = v.Byte5; }
    if (v.Byte6) { flag |= 64 ; p[idx++] = v.Byte6; }
    if (v.Byte7) { flag |= 128; p[idx++] = v.Byte7; }
    p[0] = flag;
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, UInt32 const &in) {
    bin.EnsureSpace(5);
    var p = bin.Buffer() + bin.Length();
    if (in <= (0xFF - 15)) {
        *p = (Byte)in;
        ++bin.Length();
        return;
    }
    UInt32 flag = 0;
    var &v = *(NumBits::Size4*)&in;
    var idx = 1;
    if (v.Byte0) { flag |= 1; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2; p[idx++] = v.Byte1; }
    if (v.Byte2) { flag |= 4; p[idx++] = v.Byte2; }
    if (v.Byte3) { flag |= 8; p[idx++] = v.Byte3; }
    p[0] = (Byte)(0xFF - 15 + flag);
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, UInt16 const &in) {
    bin.EnsureSpace(3);
    var p = bin.Buffer() + bin.Length();
    if (in <= (0xFF - 3)) {
        *p = (Byte)in;
        ++bin.Length();
        return;
    }
    UInt32 flag = 0;
    var &v = *(NumBits::Size2*)&in;
    var idx = 1;
    if (v.Byte0) { flag |= 1; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2; p[idx++] = v.Byte1; }
    p[0] = (Byte)(0xFF - 3 + flag);
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, Byte   const &in) {
    bin.EnsureSpace(1);
    bin.Buffer()[bin.Length()] = in;
    ++bin.Length();
}

INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, UInt64 &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    out = 0;
    if (flag == 0) {
        ++bin.Offset();
        return ConvertState_Success;
    }
    if (bin.Offset() + Math::Table_Byte1BitNums[flag] + 1 > bin.Length())
        return ConvertState_NotEnoughData;
    var v = (Byte*)&out;
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    if (flag & 4  ) v[2] = p[idx++];
    if (flag & 8  ) v[3] = p[idx++];
    if (flag & 16 ) v[4] = p[idx++];
    if (flag & 32 ) v[5] = p[idx++];
    if (flag & 64 ) v[6] = p[idx++];
    if (flag & 128) v[7] = p[idx++];
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, UInt32 &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag <= (0xFF - 15)) {
        ++bin.Offset();
        out = flag;
        return ConvertState_Success;
    }
    else {
        out = 0;
        flag -= (0xFF - 15);
    }
    if (bin.Offset() + Math::Table_Byte1BitNums[flag] + 1 > bin.Length())
        return ConvertState_NotEnoughData;
    var v = (Byte*)&out;
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    if (flag & 4  ) v[2] = p[idx++];
    if (flag & 8  ) v[3] = p[idx++];
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, UInt16 &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag <= (0xFF - 3)) {
        ++bin.Offset();
        out = flag;
        return ConvertState_Success;
    }
    else {
        out = 0;
        flag -= (0xFF - 3);
    }
    if (bin.Offset() + Math::Table_Byte1BitNums[flag] + 1 > bin.Length())
        return ConvertState_NotEnoughData;
    var v = (Byte*)&out;
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, Byte   &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    out = bin.Buffer()[bin.Offset()++];
    return ConvertState_Success;
}

INLINE void BinaryFormatter_Normal::Write7Bit(Binary &bin, Nullable<UInt32> const &in) {
    bin.EnsureSpace(5);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        p[0] = 0xFF;                        // 最高位为 1 表示空 (FF比较好看)
        ++bin.Length();
        return;
    }
    UInt32 i = in;
    var idx = 0;
    var b7 = i & 0x3F;                      // 0011 1111
    if ((i >>= 6)) {
        p[idx++] = (Byte)(b7 | 0x40);       // 0100 0000
        goto Lab1;
    } else {
        p[0] = (Byte)b7;
        ++bin.Length();
        return;
    }
Lab1:
    b7 = i & 0x7F;                          // 0111 1111
    if ((i >>= 7)) {
        p[idx++] = (Byte)(b7 | 0x80);
        goto Lab1;
    } else
        p[idx++] = (Byte)b7;
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::Write7Bit(Binary &bin, Nullable<UInt16> const &in) {
    bin.EnsureSpace(3);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        p[0] = 0xFF;
        ++bin.Length();
        return;
    }
    UInt32 i = in;
    var idx = 0;
    var b7 = i & 0x3F;                      // 0011 1111
    if ((i >>= 6)) {
        p[idx++] = (Byte)(b7 | 0x40);       // 0100 0000
        goto Lab1;
    } else {
        p[0] = (Byte)b7;
        ++bin.Length();
        return;
    }
Lab1:
    b7 = i & 0x7F;                          // 0111 1111
    if ((i >>= 7)) {
        p[idx++] = (Byte)(b7 | 0x80);
        goto Lab1;
    } else
        p[idx++] = (Byte)b7;
    bin.Length() += idx;
}

INLINE ConvertStates BinaryFormatter_Normal::Read7Bit(Binary &bin, Nullable<UInt32> &out) {
    if (bin.Offset() >= bin.Length())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    UInt32 i = 0, b7, lshift = 0;
    b7 = p[0];
    if (b7 == 0xFF) {                       // 1111 1111
        out = Null;
        ++bin.Offset();
        return ConvertState_Success;
    }
    if (b7 >= 0x80)                         // 1000 0000
        return ConvertState_Failed;
    if (b7 < 0x40) {                        // 0100 0000
        out = b7;
        ++bin.Offset();
        return ConvertState_Success;
    }
    UInt32 idx = 1, len = bin.Length() - bin.Offset();
    i |= (b7 & 0x3F);                       // 0011 1111
    lshift += 6;
Lab1:
    b7 = p[idx++];
    if (b7 >= 0x80) {
        if (idx == 5)
            return ConvertState_Overflow;
        if (idx >= len)
            return ConvertState_NotEnoughData;
        i |= (b7 & 0x7F) << lshift;         // 0111 1111
        lshift += 7;
        goto Lab1;
    } else {
        if (idx == 5 && b7 > 0x1F)          // FF FF FF FF (1F)0001 1111
            return ConvertState_Overflow;
        else
            i |= b7 << lshift;
    }
    out = i;
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read7Bit(Binary &bin, Nullable<UInt16> &out) {
    if (bin.Offset() >= bin.Length())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    UInt32 i = 0, b7, lshift = 0;
    b7 = p[0];
    if (b7 == 0xFF) {                       // 1111 1111
        out = Null;
        ++bin.Offset();
        return ConvertState_Success;
    }
    if (b7 >= 0x80)                         // 1000 0000
        return ConvertState_Failed;
    if (b7 < 0x40) {                        // 0100 0000
        out = b7;
        ++bin.Offset();
        return ConvertState_Success;
    }
    UInt32 idx = 1, len = bin.Length() - bin.Offset();
    i |= (b7 & 0x3F);                       // 0011 1111
    lshift += 6;
Lab1:
    b7 = p[idx++];
    if (b7 >= 0x80) {
        if (idx == 3)
            return ConvertState_Overflow;
        if (idx >= len)
            return ConvertState_NotEnoughData;
        i |= (b7 & 0x7F) << lshift;
        lshift += 7;
        goto Lab1;
    } else {
        if (idx == 3 && b7 > 0x07)              // FF FF (07)0000 0111
            return ConvertState_Overflow;
        else
            i |= b7 << lshift;
    }
    out = i;
    bin.Offset() += idx;
    return ConvertState_Success;
}

INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, Nullable<UInt64> const &in) {
    bin.EnsureSpace(9);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        *p = 0xFF;                  // 1111 1111
        ++bin.Length();
        return;
    }
    if (in.Value() == 0) {
        *p = 0;
        ++bin.Length();
        return;
    }
    Byte flag = 0;
    var &v = *(NumBits::Size8*)&in;
    var idx = 1;
    if (v.Byte0) { flag |= 1  ; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2  ; p[idx++] = v.Byte1; }
    if (v.Byte2) { flag |= 4  ; p[idx++] = v.Byte2; }
    if (v.Byte3) { flag |= 8  ; p[idx++] = v.Byte3; }
    if (v.Byte4) { flag |= 16 ; p[idx++] = v.Byte4; }
    if (v.Byte5) { flag |= 32 ; p[idx++] = v.Byte5; }
    if (v.Byte6 || v.Byte7) { flag |= 64; p[idx++] = v.Byte6; p[idx++] = v.Byte7; }
    p[0] = flag;
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, Nullable<UInt32> const &in) {
    bin.EnsureSpace(5);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        *p = 0xFF;                  // 1111 1111
        ++bin.Length();
        return;
    }
    if (in.Value() <= (0xFF - 15 - 1)) {
        *p = (Byte)in.Value();
        ++bin.Length();
        return;
    }
    UInt32 flag = 0;
    var &v = *(NumBits::Size4*)&in.Value();
    var idx = 1;
    if (v.Byte0) { flag |= 1; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2; p[idx++] = v.Byte1; }
    if (v.Byte2) { flag |= 4; p[idx++] = v.Byte2; }
    if (v.Byte3) { flag |= 8; p[idx++] = v.Byte3; }
    p[0] = (Byte)(0xFF - 15 - 1 + flag);
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, Nullable<UInt16> const &in) {
    bin.EnsureSpace(3);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        *p = 0xFF;                  // 1111 1111
        ++bin.Length();
        return;
    }
    if (in.Value() <= (0xFF - 3 - 1)) {
        *p = (Byte)in.Value();
        ++bin.Length();
        return;
    }
    UInt32 flag = 0;
    var &v = *(NumBits::Size2*)&in.Value();
    var idx = 1;
    if (v.Byte0) { flag |= 1; p[idx++] = v.Byte0; }
    if (v.Byte1) { flag |= 2; p[idx++] = v.Byte1; }
    p[0] = (Byte)(0xFF - 3 - 1 + flag);
    bin.Length() += idx;
}
INLINE void BinaryFormatter_Normal::WriteMask(Binary &bin, Nullable<Byte  > const &in) {
    bin.EnsureSpace(3);
    var p = bin.Buffer() + bin.Length();
    if (in.HasValue() == false) {
        *p = 0xFF;                  // 1111 1111
        ++bin.Length();
        return;
    }
    if (in.Value() <= (0xFD)) {
        *p = (Byte)in.Value();
        ++bin.Length();
        return;
    }
    p[0] = (Byte)(0xFE);
    p[1] = in.Value();
    bin.Length() += 2;
}

INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, Nullable<UInt64> &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag == 0xFF) {
        ++bin.Offset();
        out = Null;
        return ConvertState_Success;
    }
    if (flag >= 0x80)
        return ConvertState_Failed;
    var len = Math::Table_Byte1BitNums[flag] + ((flag & 64) ? 2 : 1);
    if (bin.Offset() + len > bin.Length())
        return ConvertState_NotEnoughData;
    out = 0;
    var v = (Byte*)&out.Value();
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    if (flag & 4  ) v[2] = p[idx++];
    if (flag & 8  ) v[3] = p[idx++];
    if (flag & 16 ) v[4] = p[idx++];
    if (flag & 32 ) v[5] = p[idx++];
    if (flag & 64 ) { v[6] = p[idx++]; v[7] = p[idx++]; }
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, Nullable<UInt32> &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag == 0xFF) {
        ++bin.Offset();
        out = Null;
        return ConvertState_Success;
    }
    if (flag <= (0xFF - 15 - 1)) {
        ++bin.Offset();
        out = flag;
        return ConvertState_Success;
    }
    else {
        out = 0;
        flag -= (0xFF - 15 - 1);
    }
    if (bin.Offset() + Math::Table_Byte1BitNums[flag] + 1 > bin.Length())
        return ConvertState_NotEnoughData;
    var v = (Byte*)&out.Value();
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    if (flag & 4  ) v[2] = p[idx++];
    if (flag & 8  ) v[3] = p[idx++];
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, Nullable<UInt16> &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag == 0xFF) {
        ++bin.Offset();
        out = Null;
        return ConvertState_Success;
    }
    if (flag <= (0xFF - 3 - 1)) {
        ++bin.Offset();
        out = flag;
        return ConvertState_Success;
    }
    else {
        out = 0;
        flag -= (0xFF - 3 - 1);
    }
    if (bin.Offset() + Math::Table_Byte1BitNums[flag] + 1 > bin.Length())
        return ConvertState_NotEnoughData;
    var v = (Byte*)&out;
    var idx = 1;
    if (flag & 1  ) v[0] = p[idx++];
    if (flag & 2  ) v[1] = p[idx++];
    bin.Offset() += idx;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::ReadMask(Binary &bin, Nullable<Byte  > &out) {
    if (bin.Length() <= bin.Offset())
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    var flag = p[0];
    if (flag == 0xFF) {
        ++bin.Offset();
        out = Null;
    }
    else if (flag <= (0xFD)) {
        ++bin.Offset();
        out = flag;
    }
    else {
        out = p[1];
        bin.Offset() += 2;
    }
    return ConvertState_Success;
}




template<typename T> INLINE void          BinaryFormatter_Normal::DbSetHandler::WriteColumnDataFunc (Binary &bin, void * const &list) {
    Write(bin,  *(List<T>*)list);
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::DbSetHandler::ReadColumnDataFunc  (Binary &bin, void * const &list) {
    return Read(bin,  *(List<T> * const)list);
}
template<typename T> INLINE void          BinaryFormatter_Normal::DbSetHandler::CreateColumnDataFunc(List<void*> &a) {
    a.Add(new List<T>());
}




template<typename T>
INLINE void BinaryFormatter_Normal::Write(Binary &bin, T const &in) {
    in.SaveTo(bin);
}

template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<T>   const &in) {
    if (in.HasValue()) {
        Write(bin, Byte(0x01));
        Write(bin, in.Value());
    }
    else
        Write(bin, Byte(0xFF));
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, Array<T>      const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length());
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<T>       const &in) {
    var len = in.Length();
    Write(bin, len);
    for (UInt32 i = 0; i < len; ++i)
        Write(bin, in[i]);
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, LinkedList<T> const &in) {
    var len = in.Length();
    Write(bin, len);
    for (UInt32 i = 0; i < len; ++i)
        Write(bin, in[i]);
}
template<typename TK, typename TV>
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Dictionary<TK, TV> const &in) {
    var len = in.Count();
    Write(bin, len);
    if (len) {
        var p = in.UsedNodeHead();
Lab2:
        Write(bin, p->Key);
        Write(bin, p->Value);
        if ((p = p->UsedNext))
            goto Lab2;
    }
}

template<typename T>
INLINE void BinaryFormatter_Normal::Write(Binary &bin, T const * const &in, UInt32 const &len) {
    assert ( in );
    for (UInt32 i = 0; i < len; ++i)
        Write(bin, in[i]);
}
template<typename T, UInt32 len>
INLINE void BinaryFormatter_Normal::Write(Binary &bin, T const (&in)[len]) {
    Write(bin, in, len);
}



template<typename T>
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, T &out) {
    return out.LoadFrom(bin);
}

template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<T>   &out) {
    Byte b;
    var rtv = Read(bin, b);
    if (rtv)
        return rtv;
    if (b == 0xFF) {
        out = Null;                             
        return ConvertState_Success;            
    }
    else if (b != 0x01)
        return ConvertState_Failed;
    if (out.HasValue() == false) {
        out.HasValue() = true;                      
        new (&out.Value()) T();
    }
    rtv = Read(bin, out.Value());
    if (rtv)
        return rtv;
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Array<T>      &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();                                    // 清掉老数据 (带析构)
    out.Length() = len;
    out.EnsureLength();
    for (UInt32 i = 0; i < len; ++i) {
        new (&out[i]) T();                          // 用默认构造函数初始化单元值
        if ((rtv = Read(bin, out[i]))) {            // 试还原单元值
            out.Length() = i;                       // 如果失败，设置正确的已填充长度 (便于回收)，返回失败原因
            return rtv;
        }
    }
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<T>       &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();
    out.EnsureLength(len);
    for (UInt32 i = 0; i < len; ++i) {
        new (&out[i]) T();
        if ((rtv = Read(bin, out[i]))) {
            out.Length() = i;
            return rtv;
        }
    }
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, LinkedList<T> &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();
    out.EnsureLength(len);
    for (UInt32 i = 0; i < len; ++i) {
        new (&out[i]) T();
        if ((rtv = Read(bin, out[i]))) {
            out.Length() = i;
            return rtv;
        }
    }
    return ConvertState_Success;
}
template<typename TK, typename TV>
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Dictionary<TK, TV> &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();
    for (UInt32 i = 0; i < len; ++i) {       // 这里先用 copy Add 方式来搞了。今后再优化
        TK k;
        if ((rtv = Read(bin, k)))
            return rtv;
        TV v;
        if ((rtv = Read(bin, v)))
            return rtv;
        out.Add(k, v, true);                    // 允许 key 相同覆盖，避免还原过程中崩溃
    }
    return ConvertState_Success;
}

template<typename T>
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, T * const &out, UInt32 const &len) {
    assert ( out );
    for (UInt32 i = 0; i < len; ++i) {
        var rtv = Read(bin, out[i]);
        if (rtv)
            return rtv;
    }
    return ConvertState_Success;
}
template<typename T, UInt32 len>
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, T (&out)[len]) {
    return Read(bin, out, len);
}




INLINE void BinaryFormatter_Normal::Write(Binary &bin, Byte      const &in) {
    bin.EnsureSpace(1);
    bin.Buffer()[bin.Length()++] = in;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, UInt16    const &in) {
    Write7Bit(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, UInt32    const &in) {
    Write7Bit(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, UInt64    const &in) {
    WriteMask(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, SByte     const &in) {
    Write(bin, *(Byte*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Int16     const &in) {
    Write(bin, ZegZagEncode(in));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Int32     const &in) {
    Write(bin, ZegZagEncode(in));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Int64     const &in) {
    Write(bin, ZegZagEncode(in));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Double    const &in) {
    Write(bin, *(UInt64*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Single    const &in) {
    Write(bin, *(UInt32*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Char      const &in) {
    bin.EnsureSpace(sizeof(in));
#ifdef __IA
    *(UInt16*)(bin.Buffer() + bin.Length()) = in;
#else
    var p = bin.Buffer() + bin.Length();
    var &v = *(NumBits::Size2*)&in;
    p[0] = v.Byte0;
    p[1] = v.Byte1;
#endif
    bin.Length() += 2;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Boolean   const &in) {
    bin.EnsureSpace(1);
    bin.Buffer()[bin.Length()++] = in ? 1 : 0;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, DateTime  const &in) {
    Write(bin, in.Value);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Guid      const &in) {
    bin.EnsureSpace(16);
#ifdef __IA
    ((UInt64*)(bin.Buffer() + bin.Length()))[0] = ((UInt64*)&in)[0];
    ((UInt64*)(bin.Buffer() + bin.Length()))[1] = ((UInt64*)&in)[1];
#else
  #ifdef __LE
    memcpy(bin.Buffer() + bin.Length(), &in, 16);
  #else
    var p   = bin.Buffer() + bin.Length();          // Guid 结构是 i4 i2 i2 i2 i1 i1 i1 i1 i1 i1
    var v   = (Byte*)&in;
    p[ 0] = v[ 3];
    p[ 1] = v[ 2];
    p[ 2] = v[ 1];
    p[ 3] = v[ 0];

    p[ 4] = v[ 5];
    p[ 5] = v[ 4];

    p[ 6] = v[ 7];
    p[ 7] = v[ 6];

    p[ 8] = v[ 9];
    p[ 9] = v[ 8];

    p[10] = v[10];
    p[11] = v[11];
    p[12] = v[12];
    p[13] = v[13];
    p[14] = v[14];
    p[15] = v[15];
  #endif
#endif
    bin.Length() += 16;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Decimal   const &in) {
    Write(bin, in.Value());
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, BitArray  const &in) {
    var len = in.Length();
    Write(bin, len);
    if ((len = (len + 7) >> 3)) {       // 得到要复制的字节数
        bin.EnsureSpace(len);
#ifdef __LE
        memcpy(bin.Buffer() + bin.Length(), in.Buffer(), len);
#else
        var o = in.Buffer();
        var p = bin.Buffer() + bin.Length();
        var mod = len % sizeof(size_t);
        UInt32 i = 0;
        for (; i < len - mod; i += sizeof(size_t), p += sizeof(size_t)) {
            if (sizeof(size_t) == 4) {
                var &v = *(NumBits::Size4*)(o + i);
                p[0] = v.Byte0;
                p[1] = v.Byte1;
                p[2] = v.Byte2;
                p[3] = v.Byte3;
            } else {        // == 8
                var &v = *(NumBits::Size8*)(o + i);
                p[0] = v.Byte0;
                p[1] = v.Byte1;
                p[2] = v.Byte2;
                p[3] = v.Byte3;
                p[4] = v.Byte4;
                p[5] = v.Byte5;
                p[6] = v.Byte6;
                p[7] = v.Byte7;
            }
        }
        var n = (Byte*)(size_t*)&o[i];
        for (i = 0; i < mod; ++i)
            *p++ = (Byte)(n >> (i * 8));
#endif
        bin.Length() += len;
    }
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, SString   const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length());
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, String    const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length());
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, DbSet     const &in) {
    Write(bin, in.AffectedRows());
    Write(bin, in.Message());
    Write(bin, in.Length());
    for (UInt32 i = 0; i < in.Length(); ++i)
        Write(bin, in[i]);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, DbTable   const &in) {
    Write(bin, in.ColumnCount());
    Write(bin, in.RowCount());
    Write(bin, in.ColumnTypes());
    Write(bin, in.ColumnNames());
    for (UInt32 i = 0; i < in.ColumnCount(); ++i)
        DbSetHandler::WriteColumnDataFuncs[ in.ColumnTypes()[i] ]( bin, in.ColumnDatas()[i] );
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Binary    const &in) {
    UInt32 len = in.Length();            // 先读长度，避免传自己 (this == &in) 的时候多复制
    Write(bin, len);
    Write(bin, in.Buffer(), len);
}


INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Byte     &out) {
    if (bin.Length() < bin.Offset() + 1)
        return ConvertState_NotEnoughData;
    out = bin.Buffer()[bin.Offset()++];
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, UInt16   &out) {
    return Read7Bit(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, UInt32   &out) {
    return Read7Bit(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, UInt64   &out) {
    return ReadMask(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, SByte    &out) {
    return Read(bin, *(Byte*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Int16    &out) {
    UInt16 i;
    var rtv = Read(bin, i);
    if (rtv)
        return rtv;
    out = ZegZagDecode(i);
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Int32    &out) {
    UInt32 i;
    var rtv = Read(bin, i);
    if (rtv)
        return rtv;
    out = ZegZagDecode(i);
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Int64    &out) {
    UInt64 i;
    var rtv = Read(bin, i);
    if (rtv)
        return rtv;
    out = ZegZagDecode(i);
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Double   &out) {
    return Read(bin, *(UInt64*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Single   &out) {
    return Read(bin, *(UInt32*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Char     &out) {
    if (bin.Length() < bin.Offset() + sizeof(out))
        return ConvertState_NotEnoughData;
#ifdef __IA
    out = *(UInt16*)(bin.Buffer() + bin.Offset());
#else
	var &p = *(NumBits::Size2*)(bin.Buffer() + bin.Offset());
    var v = (Byte*)&out;
	v[0] = p.Byte0;
	v[1] = p.Byte1;
#endif
    bin.Offset() += 2;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Boolean  &out) {
    if (bin.Length() < bin.Offset() + 1)
        return ConvertState_NotEnoughData;
    var c = bin.Buffer()[bin.Offset()];
    if (c > 1)
        return ConvertState_Overflow;
    out = c ? true : false;
    ++bin.Offset();
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, DateTime &out) {
    return Read(bin, out.Value);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Guid     &out) {
    if (bin.Length() < bin.Offset() + sizeof(out))
        return ConvertState_NotEnoughData;
#ifdef __IA
    ((UInt64*)&out)[0] = ((UInt64*)(bin.Buffer() + bin.Offset()))[0];
    ((UInt64*)&out)[1] = ((UInt64*)(bin.Buffer() + bin.Offset()))[1];
#else
  #ifdef __LE
    memcpy(&out, bin.Buffer() + bin.Offset(), 16);
  #else
    var p   = bin.Buffer() + bin.Length();          // Guid 结构是 i4 i2 i2 i2 i1 i1 i1 i1 i1 i1
    var v   = (Byte*)&out;
    *(UInt32*)(v + 0) = p[0] + (p[1] << 8) + (p[2] << 16) + (p[3] << 24);
    *(UInt16*)(v + 4) = p[4] + (p[5] << 8);
    *(UInt16*)(v + 6) = p[6] + (p[7] << 8);
    *(UInt16*)(v + 8) = p[8] + (p[9] << 8);
    v[10] = p[10];
    v[11] = p[11];
    v[12] = p[12];
    v[13] = p[13];
    v[14] = p[14];
    v[15] = p[15];
  #endif
#endif
    bin.Offset() += 16;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Decimal  &out) {
    return Read(bin, *(UInt64*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, BitArray &out) {
    UInt32 bits;
    var rtv = Read(bin, bits);
    if (rtv)
        return rtv;
    var bytes = (bits + 7) >> 3;
    var sizets = (bytes + sizeof(size_t) - 1) >> ((sizeof(size_t) >> 2) + 1);
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    out.Set(bits, false);                   // 确保长度  todo: 改成 EnsureLength
    out.Length() = bits;                    // 设置 bits
    out.Buffer()[sizets - 1] = 0;           // 将 Length 所在 size_t 清 0
#ifdef __LE
    memcpy(out.Buffer(), bin.Buffer() + bin.Offset(), bytes);
#else
    var modbytes = bytes % sizeof(size_t);
    var p = bin.Buffer() + bin.Offset();
    for (UInt32 i = 0; i < bytes - modbytes; i += sizeof(size_t)) {
        if (sizeof(size_t) == 4) {
            var &v = *(NumBits::Size4*)(((Byte*)out.Buffer()) + i);
            v.Byte0 = p[0];
            v.Byte1 = p[1];
            v.Byte2 = p[2];
            v.Byte3 = p[3];
        }
        else {
            var &v = *(NumBits::Size8*)(((Byte*)out.Buffer()) + i);
            v.Byte0 = p[0];
            v.Byte1 = p[1];
            v.Byte2 = p[2];
            v.Byte3 = p[3];
            v.Byte4 = p[4];
            v.Byte5 = p[5];
            v.Byte6 = p[6];
            v.Byte7 = p[7];
        }
    }
    p = bin.Buffer() + bin.Offset() + bytes - modbytes;
    var pv = (Byte*)out.Buffer() + bytes - modbytes + sizeof(size_t);
    for (UInt32 i = 0; i < modbytes; ++i)
        *(pv - i) = *(p + i);
#endif
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, SString  &out) {
    UInt32 bytes;
    var rtv = Read(bin, bytes);
    if (rtv)
        return rtv;
    out.Clear();
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    out.Length() = bytes;
    out.EnsureLength();
    memcpy(out.Buffer(), bin.Buffer() + bin.Offset(), bytes);
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, String   &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();
    var bytes = len << 1;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    out.Length() = len;
    out.EnsureLength();
#ifdef __LE
    if (sizeof(Char) == 2)
        memcpy(out.Buffer(), bin.Buffer() + bin.Offset(), bytes);
    else {
        var p = bin.Buffer() + bin.Offset();
        for (UInt32 i = 0; i < len; ++i, p += 2)
  #ifdef __IA
            out.Buffer()[i] = *(UInt16*)p;
  #else
            out.Buffer()[i] = p[0] | (p[1] << 8);
  #endif
    }
#else
    var p = bin.Buffer() + bin.Offset();
    for (UInt32 i = 0; i < len; ++i, p += 2)
        out.Buffer()[i] = p[0] | (p[1] << 8);
#endif
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Binary   &out) {
    UInt32 len;
    var rtv = Read(bin, len);                       // 试读长度
    if (rtv)
        return rtv;                                 // 失败返回信息
    if (bin.Length() < bin.Offset() + len)
        return ConvertState_NotEnoughData;          // 返回不够长
    out.Length() = len;
    out.EnsureLength();
    memcpy(out.Buffer(), bin.Buffer() + bin.Offset(), len);
    bin.Offset() += len;                            // 校正 offset
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, DbSet    &out) {
    out.Clear();
    var rtv = Read(bin, out.AffectedRows());
    if (rtv)
        return rtv;
    rtv = Read(bin, out.Message());
    if (rtv)
        return rtv;
    UInt32 len;
    rtv = Read(bin, len);
    if (rtv)
        return rtv;
    for (UInt32 i = 0; i < len; ++i)        // 先把空表建起
        out.NewDbTable();
    for (UInt32 i = 0; i < len; ++i) {
        rtv = Read(bin, out[i]);            // 试填充
        if (rtv)
            return rtv;
    }
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, DbTable  &out) {
    out.ClearData();
    var rtv = Read(bin, out.ColumnCount());
    if (rtv)
        return rtv;
    rtv = Read(bin, out.RowCount());
    if (rtv)
        return rtv;
    rtv = Read(bin, out.ColumnTypes());
    if (rtv)
        return rtv;
    rtv = Read(bin, out.ColumnNames());
    if (rtv)
        return rtv;
    for (UInt32 i = 0; i < out.ColumnCount(); ++i)      // 先把空列建起
        DbSetHandler::CreateColumnDataFuncs[ out.ColumnTypes()[i] ]( out.ColumnDatas() );
    for (UInt32 i = 0; i < out.ColumnCount(); ++i) {    // 试填充
        rtv = DbSetHandler::ReadColumnDataFuncs[ out.ColumnTypes()[i] ]( bin, out.ColumnDatas()[i] );
        if (rtv)
            return rtv;
    }
    // todo: NamesHash
    return ConvertState_Success;
}




INLINE void BinaryFormatter_Normal::Write(Binary &bin, Byte  const * const &in, UInt32 const &len) {
    assert ( in );
    if (len == 0)
        return;
    bin.EnsureSpace(len);
    memcpy(bin.Buffer() + bin.Length(), in, len);
    bin.Length() += len;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, SByte const * const &in, UInt32 const &len) {
    Write(bin, (Byte*)in, len);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Char  const * const &in, UInt32 const &len) {
    assert ( in );
    if (len == 0)
        return;
    var bytes = len << 1;
    bin.EnsureSpace(bytes);
    if (sizeof(Char) == 2) {
#ifdef __LE
        memcpy(bin.Buffer() + bin.Length(), in, bytes); // 2字节长的Char + 小尾才能直接复制
#else
        var p = bin.Buffer() + bin.Length();
        for (UInt32 i = 0; i < bytes; i += 2, p += 2) {
            var &v = *(NumBits::Size2*)(((Byte*)in) + i);
            p[0] = v.Byte0;
            p[1] = v.Byte1;
        }
#endif
    }
    else {
        var p = bin.Buffer() + bin.Length();
        for (UInt32 i = 0; i < len; ++i, p += 2) {
            var c = (UInt16)(in[i]);
#ifdef __IA
            *(UInt16*)p = c;
#else
            var &v = *(NumBits::Size2*)&c;
            p[0] = v.Byte0;
            p[1] = v.Byte1;
#endif
        }
    }
    bin.Length() += bytes;
}

INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Byte  * const &out, UInt32 const &len) {
    assert ( out );
    if (bin.Length() < bin.Offset() + len)
        return ConvertState_NotEnoughData;
    memcpy(out, bin.Buffer() + bin.Offset(), len);
    bin.Offset() += len;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, SByte * const &out, UInt32 const &len) {
    return Read(bin, (Byte*)out, len);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Char  * const &out, UInt32 const &len) {
    assert ( out );
    var bytes = len << 1;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
#ifdef __LE
    if (sizeof(Char) == 2)
        memcpy(out, bin.Buffer() + bin.Offset(), bytes);
    else {
        var p = bin.Buffer() + bin.Offset();
        for (UInt32 i = 0; i < len; ++i, p += 2)
  #ifdef __IA
            out[i] = *(UInt16*)p;
  #else
            out[i] = p[0] | (p[1] << 8);
  #endif
    }
#else
    var p = bin.Buffer() + bin.Offset();
    for (UInt32 i = 0; i < len; ++i, p += 2)
        out[i] = p[0] | (p[1] << 8);
#endif
    bin.Offset() += bytes;
    return ConvertState_Success;
}


INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<Byte > const &in) { Write(bin, in.Length()); Write(bin, in.Buffer(), in.Length()); }
INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<SByte> const &in) { Write(bin, in.Length()); Write(bin, in.Buffer(), in.Length()); }
INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<Char > const &in) { Write(bin, in.Length()); Write(bin, in.Buffer(), in.Length()); }

INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<Byte > &out) { UInt32 len; var rtv = Read(bin, len); if (rtv) return rtv; out.Clear(); out.EnsureLength(len); return Read(bin, (Byte  *)out.Buffer(), len); }
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<SByte> &out) { UInt32 len; var rtv = Read(bin, len); if (rtv) return rtv; out.Clear(); out.EnsureLength(len); return Read(bin, (SByte *)out.Buffer(), len); }
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<Char > &out) { UInt32 len; var rtv = Read(bin, len); if (rtv) return rtv; out.Clear(); out.EnsureLength(len); return Read(bin, (Char  *)out.Buffer(), len); }




INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Byte    > const &in) {
    WriteMask(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<UInt16  > const &in) {
    Write7Bit(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<UInt32  > const &in) {
    Write7Bit(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<UInt64  > const &in) {
    WriteMask(bin, in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<SByte   > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, Nullable<Byte>(ZegZagEncode(in.Value())));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Int16   > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, Nullable<UInt16>(ZegZagEncode(in.Value())));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Int32   > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, Nullable<UInt32>(ZegZagEncode(in.Value())));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Int64   > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, Nullable<UInt64>(ZegZagEncode(in.Value())));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Double  > const &in) {
    Write(bin, *(Nullable<UInt64>*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Single  > const &in) {
    Write(bin, *(Nullable<UInt32>*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Char    > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, Nullable<UInt16>((UInt16)in.Value()));
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Boolean > const &in) {
    if (in.HasValue() == false)
        Write(bin, (Byte)0xFF);
    else
        Write(bin, in.Value());
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<DateTime> const &in) {
    Write(bin, *(Nullable<UInt64>*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Decimal > const &in) {
    Write(bin, *(Nullable<UInt64>*)&in);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<BitArray> const &in) {
    if (in.HasValue() == false) {
        Write(bin, (Byte)0xFF);
        return;
    }
    var len = in.Value().Length();
    Write(bin, len);
    if ((len = (len + 7) >> 3)) {       // 得到要复制的字节数
        bin.EnsureSpace(len);
#ifdef __LE
        memcpy(bin.Buffer() + bin.Length(), in.Value().Buffer(), len);
#else
        var o = in.Value().Buffer();
        var p = bin.Buffer() + bin.Length();
        var mod = len % sizeof(size_t);
        UInt32 i = 0;
        for (; i < len - mod; i += sizeof(size_t), p += sizeof(size_t)) {
            if (sizeof(size_t) == 4) {
                var &v = *(NumBits::Size4*)(o + i);
                p[0] = v.Byte0;
                p[1] = v.Byte1;
                p[2] = v.Byte2;
                p[3] = v.Byte3;
            } else {        // == 8
                var &v = *(NumBits::Size8*)(o + i);
                p[0] = v.Byte0;
                p[1] = v.Byte1;
                p[2] = v.Byte2;
                p[3] = v.Byte3;
                p[4] = v.Byte4;
                p[5] = v.Byte5;
                p[6] = v.Byte6;
                p[7] = v.Byte7;
            }
        }
        var n = (Byte*)(size_t*)&o[i];
        for (i = 0; i < mod; ++i)
            *p++ = (Byte)(n >> (i * 8));
#endif
        bin.Length() += len;
    }
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<SString > const &in) {
    if (in.HasValue() == false) {
        Write(bin, (Byte)0xFF);
        return;
    }
    var len = in.Value().Length();
    Write(bin, Nullable<UInt32>(len));
    if (len) {
        bin.EnsureSpace(len);
        memcpy(bin.Buffer() + bin.Length(), in.Value().Buffer(), len);
        bin.Length() += len;
    }
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<String  > const &in) {
    if (in.HasValue() == false) {
        Write(bin, (Byte)0xFF);
        return;
    }
    var len = in.Value().Length();
    Write(bin, Nullable<UInt32>(len));
    if (len == 0)
        return;
    var bytes = len << 1;
    bin.EnsureSpace(bytes);
    if (sizeof(Char) == 2) {
#ifdef __LE
        memcpy(bin.Buffer() + bin.Length(), in.Value().Buffer(), bytes); // 2字节长的Char + 小尾才能直接复制
#else
        var p = bin.Buffer() + bin.Length();
        for (UInt32 i = 0; i < bytes; i += 2, p += 2) {
            var &v = *(NumBits::Size2*)(((Byte*)in.Value().Buffer()) + i);
            p[0] = v.Byte0;
            p[1] = v.Byte1;
        }
#endif
    }
    else {
        var p = bin.Buffer() + bin.Length();
        for (UInt32 i = 0; i < len; ++i, p += 2) {
            var c = (UInt16)(in.Value().Buffer()[i]);
#ifdef __IA
            *(UInt16*)p = c;
#else
            var &v = *(NumBits::Size2*)&c;
            p[0] = v.Byte0;
            p[1] = v.Byte1;
#endif
        }
    }
    bin.Length() += bytes;
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Binary  > const &in) {
    if (in.HasValue() == false) {
        Write(bin, (Byte)0xFF);
        return;
    }
    var len = in.Value().Length();
    Write(bin, Nullable<UInt32>(len));
    if (len) {
        bin.EnsureSpace(len);
        memcpy(bin.Buffer() + bin.Length(), in.Value().Buffer(), len);
        bin.Length() += len;
    }
}


INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Byte    > &out) {
    return ReadMask(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<UInt16  > &out) {
    return Read7Bit(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<UInt32  > &out) {
    return Read7Bit(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<UInt64  > &out) {
    return ReadMask(bin, out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<SByte   > &out) {
	var &o = *(Nullable<Byte>*)&out;
    var rtv = Read(bin, o);
    if (rtv)
        return rtv;
    out.Value() = ZegZagDecode(o.Value());
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Int16   > &out) {
	var &o = *(Nullable<UInt16>*)&out;
    var rtv = Read(bin, o);
    if (rtv)
        return rtv;
    out.Value() = ZegZagDecode(o.Value());
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Int32   > &out) {
	var &o = *(Nullable<UInt32>*)&out;
    var rtv = Read(bin, o);
    if (rtv)
        return rtv;
    out.Value() = ZegZagDecode(o.Value());
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Int64   > &out) {
	var &o = *(Nullable<UInt64>*)&out;
    var rtv = Read(bin, o);
    if (rtv)
        return rtv;
    out.Value() = ZegZagDecode(o.Value());
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Double  > &out) {
    return Read(bin, *(Nullable<UInt64>*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Single  > &out) {
    return Read(bin, *(Nullable<UInt32>*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Char    > &out) {
    Nullable<UInt16> o;
    var rtv = Read(bin, o);
    if (rtv)
        return rtv;
    out = (Char)o.Value();
	return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Boolean > &out) {
    if (bin.Length() < bin.Offset() + 1)
        return ConvertState_NotEnoughData;
    var c = bin.Buffer()[bin.Offset()];
    if (c == 0xFF)
        out = Null;
    else if (c == 1)
        out = true;
    else if (c == 0)
        out = false;
    else
        return ConvertState_Overflow;
    ++bin.Offset();
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<DateTime> &out) {
    return Read(bin, *(Nullable<UInt64>*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Decimal > &out) {
    return Read(bin, *(Nullable<UInt64>*)&out);
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<BitArray> &out) {
    Nullable<UInt32> len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    if (len.HasValue() == false) {
        out = Null;
        return ConvertState_Success;
    }
    if (out.HasValue() == false) {
        out.HasValue() = true;
        new (&out.Value()) BitArray();  // todo: len init
    }
    var &o = out.Value();
    var bits = len.Value();
    var bytes = (bits + 7) >> 3;
    var sizets = (bytes + sizeof(size_t) - 1) >> ((sizeof(size_t) >> 2) + 1);
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    o.Set(bits, false);                   // 确保长度  todo: 改成 EnsureLength
    o.Length() = bits;                    // 设置 bits
    o.Buffer()[sizets - 1] = 0;           // 将 Length 所在 size_t 清 0
#ifdef __LE
    memcpy(o.Buffer(), bin.Buffer() + bin.Offset(), bytes);
#else
    var modbytes = bytes % sizeof(size_t);
    var p = bin.Buffer() + bin.Offset();
    for (UInt32 i = 0; i < bytes - modbytes; i += sizeof(size_t)) {
        if (sizeof(size_t) == 4) {
            var &v = *(NumBits::Size4*)(((Byte*)o.Buffer()) + i);
            v.Byte0 = p[0];
            v.Byte1 = p[1];
            v.Byte2 = p[2];
            v.Byte3 = p[3];
        }
        else {
            var &v = *(NumBits::Size8*)(((Byte*)o.Buffer()) + i);
            v.Byte0 = p[0];
            v.Byte1 = p[1];
            v.Byte2 = p[2];
            v.Byte3 = p[3];
            v.Byte4 = p[4];
            v.Byte5 = p[5];
            v.Byte6 = p[6];
            v.Byte7 = p[7];
        }
    }
    p = bin.Buffer() + bin.Offset() + bytes - modbytes;
    var pv = (Byte*)o.Buffer() + bytes - modbytes + sizeof(size_t);
    for (UInt32 i = 0; i < modbytes; ++i)
        *(pv - i) = *(p + i);
#endif
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<SString > &out) {
    Nullable<UInt32> tmp;
    var rtv = Read(bin, tmp);
    if (rtv)
        return rtv;
    if (tmp.HasValue() == false) {
        out = Null;
        return ConvertState_Success;
    }
    var &bytes = tmp.Value();
    var &o = out.Value();
    if (out.HasValue() == false) {
        out.HasValue() = true;
        new (&o) SString(bytes);
    }
    else {
        o.Clear();
        o.Length() = bytes;
        o.EnsureLength();
    }
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    o.Length() = bytes;
    o.EnsureLength();
    memcpy(o.Buffer(), bin.Buffer() + bin.Offset(), bytes);
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<String  > &out) {
    Nullable<UInt32> tmp;
    var rtv = Read(bin, tmp);
    if (rtv)
        return rtv;
    if (tmp.HasValue() == false) {
        out = Null;
        return ConvertState_Success;
    }
    var &len = tmp.Value();
    var &o = out.Value();
    if (out.HasValue() == false) {
        out.HasValue() = true;
        new (&o) String(len);
    }
    else {
        o.Clear();
        o.Length() = len;
        o.EnsureLength();
    }
    var bytes = len << 1;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    o.Length() = len;
    o.EnsureLength();
#ifdef __LE
    if (sizeof(Char) == 2)
        memcpy(o.Buffer(), bin.Buffer() + bin.Offset(), bytes);
    else {
        var p = bin.Buffer() + bin.Offset();
        for (UInt32 i = 0; i < len; ++i, p += 2)
  #ifdef __IA
            o.Buffer()[i] = *(UInt16*)p;
  #else
            o.Buffer()[i] = p[0] | (p[1] << 8);
  #endif
    }
#else
    var p = bin.Buffer() + bin.Offset();
    for (UInt32 i = 0; i < len; ++i, p += 2)
        o.Buffer()[i] = p[0] | (p[1] << 8);
#endif
    bin.Offset() += bytes;
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Binary  > &out) {
    Nullable<UInt32> tmp;
    var rtv = Read(bin, tmp);
    if (rtv)
        return rtv;
    if (tmp.HasValue() == false) {
        out = Null;
        return ConvertState_Success;
    }
    var &bytes = tmp.Value();
    var &o = out.Value();
    if (out.HasValue() == false) {
        out.HasValue() = true;
        new (&o) SString(bytes);
    }
    else {
        o.Clear();
        o.Length() = bytes;
        o.EnsureLength();
    }
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    o.Length() = bytes;
    o.EnsureLength();
    memcpy(o.Buffer(), bin.Buffer() + bin.Offset(), bytes);
    bin.Offset() += bytes;
    return ConvertState_Success;
}




template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, Array<Nullable<T>>      const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length());
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<Nullable<T>>       const &in) {
    var bits = in.Length();
    Write(bin, bits);
    if (bits == 0)
        return;
    var bytes = (bits + 7) >> 3;
    bin.EnsureSpace(bytes);
    var p = bin.Buffer() + bin.Length();
    bin.Length() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        Byte n = 0;
        var offset = i << 3;
        if (in[offset + 0].HasValue()) n |= 128;
        if (in[offset + 1].HasValue()) n |= 64;
        if (in[offset + 2].HasValue()) n |= 32;
        if (in[offset + 3].HasValue()) n |= 16;
        if (in[offset + 4].HasValue()) n |= 8;
        if (in[offset + 5].HasValue()) n |= 4;
        if (in[offset + 6].HasValue()) n |= 2;
        if (in[offset + 7].HasValue()) n |= 1;
        p[i] = n;
    }
    if (mod) {
        Byte n = 0;
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (in[offset + i].HasValue()) n |= v;
            v >>= 1;
        }
        p[bytes] = n;
    }
    for (UInt32 i = 0; i < bits; ++i)
        if (in[i].HasValue())
            Write(bin, in[i].Value());
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, LinkedList<Nullable<T>> const &in) {
    var bits = in.Length();
    Write(bin, bits);
    if (bits == 0)
        return;
    var bytes = (bits + 7) >> 3;
    bin.EnsureSpace(bytes);
    var p = bin.Buffer() + bin.Length();
    bin.Length() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        Byte n = 0;
        var offset = i << 3;
        if (in[offset + 0].HasValue()) n |= 128;
        if (in[offset + 1].HasValue()) n |= 64;
        if (in[offset + 2].HasValue()) n |= 32;
        if (in[offset + 3].HasValue()) n |= 16;
        if (in[offset + 4].HasValue()) n |= 8;
        if (in[offset + 5].HasValue()) n |= 4;
        if (in[offset + 6].HasValue()) n |= 2;
        if (in[offset + 7].HasValue()) n |= 1;
        p[i] = n;
    }
    if (mod) {
        Byte n = 0;
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (in[offset + i].HasValue()) n |= v;
            v >>= 1;
        }
        p[bytes] = n;
    }
    for (UInt32 i = 0; i < bits; ++i)
        if (in[i].HasValue())
            Write(bin, in[i].Value());
}
template<typename T> INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<T> const * const &in, UInt32 const &len) {
    assert ( in );
    if (len == 0)
        return;
    var bytes = (len + 7) >> 3;
    bin.EnsureSpace(bytes);
    var p = bin.Buffer() + bin.Length();
    bin.Length() += bytes;
    var mod = len % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        Byte n = 0;
        var offset = i << 3;
        if (in[offset + 0].HasValue()) n |= 128;
        if (in[offset + 1].HasValue()) n |= 64;
        if (in[offset + 2].HasValue()) n |= 32;
        if (in[offset + 3].HasValue()) n |= 16;
        if (in[offset + 4].HasValue()) n |= 8;
        if (in[offset + 5].HasValue()) n |= 4;
        if (in[offset + 6].HasValue()) n |= 2;
        if (in[offset + 7].HasValue()) n |= 1;
        p[i] = n;
    }
    if (mod) {
        Byte n = 0;
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (in[offset + i].HasValue()) n |= v;
            v >>= 1;
        }
        p[bytes] = n;
    }
    for (UInt32 i = 0; i < len; ++i)
        if (in[i].HasValue())
            Write(bin, in[i].Value());
}


template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Array<Nullable<T>>      &out) {
    UInt32 bits;
    var rtv = Read(bin, bits);
    if (rtv)
        return rtv;
    out.Clear();
    if (bits == 0)
        return ConvertState_Success;
    out.Length() = bits;
    out.EnsureLength();
    var bytes = (bits + 7) >> 3;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    bin.Offset() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        var offset = i << 3;
        if (n & 128) { var &o = out[offset + 0]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 0 + 1; return rtv; } } else out[offset + 0].HasValue() = false;
        if (n & 64 ) { var &o = out[offset + 1]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 1 + 1; return rtv; } } else out[offset + 1].HasValue() = false;
        if (n & 32 ) { var &o = out[offset + 2]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 2 + 1; return rtv; } } else out[offset + 2].HasValue() = false;
        if (n & 16 ) { var &o = out[offset + 3]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 3 + 1; return rtv; } } else out[offset + 3].HasValue() = false;
        if (n & 8  ) { var &o = out[offset + 4]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 4 + 1; return rtv; } } else out[offset + 4].HasValue() = false;
        if (n & 4  ) { var &o = out[offset + 5]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 5 + 1; return rtv; } } else out[offset + 5].HasValue() = false;
        if (n & 2  ) { var &o = out[offset + 6]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 6 + 1; return rtv; } } else out[offset + 6].HasValue() = false;
        if (n & 1  ) { var &o = out[offset + 7]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 7 + 1; return rtv; } } else out[offset + 7].HasValue() = false;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (n & v) { var &o = out[offset + i]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + i + 1; return rtv; } } else out[offset + i].HasValue() = false;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<Nullable<T>>       &out) {
    UInt32 bits;
    var rtv = Read(bin, bits);
    if (rtv)
        return rtv;
    out.Clear();
    if (bits == 0)
        return ConvertState_Success;
    out.EnsureLength(bits);
    var bytes = (bits + 7) >> 3;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    bin.Offset() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        var offset = i << 3;
        if (n & 128) { var &o = out[offset + 0]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 0 + 1; return rtv; } } else out[offset + 0].HasValue() = false;
        if (n & 64 ) { var &o = out[offset + 1]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 1 + 1; return rtv; } } else out[offset + 1].HasValue() = false;
        if (n & 32 ) { var &o = out[offset + 2]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 2 + 1; return rtv; } } else out[offset + 2].HasValue() = false;
        if (n & 16 ) { var &o = out[offset + 3]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 3 + 1; return rtv; } } else out[offset + 3].HasValue() = false;
        if (n & 8  ) { var &o = out[offset + 4]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 4 + 1; return rtv; } } else out[offset + 4].HasValue() = false;
        if (n & 4  ) { var &o = out[offset + 5]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 5 + 1; return rtv; } } else out[offset + 5].HasValue() = false;
        if (n & 2  ) { var &o = out[offset + 6]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 6 + 1; return rtv; } } else out[offset + 6].HasValue() = false;
        if (n & 1  ) { var &o = out[offset + 7]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 7 + 1; return rtv; } } else out[offset + 7].HasValue() = false;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (n & v) { var &o = out[offset + i]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + i + 1; return rtv; } } else out[offset + i].HasValue() = false;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, LinkedList<Nullable<T>> &out) {
    UInt32 bits;
    var rtv = Read(bin, bits);
    if (rtv)
        return rtv;
    out.Clear();
    if (bits == 0)
        return ConvertState_Success;
    out.EnsureLength(bits);
    var bytes = (bits + 7) >> 3;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    bin.Offset() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        var offset = i << 3;
        if (n & 128) { var &o = out[offset + 0]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 0 + 1; return rtv; } } else out[offset + 0].HasValue() = false;
        if (n & 64 ) { var &o = out[offset + 1]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 1 + 1; return rtv; } } else out[offset + 1].HasValue() = false;
        if (n & 32 ) { var &o = out[offset + 2]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 2 + 1; return rtv; } } else out[offset + 2].HasValue() = false;
        if (n & 16 ) { var &o = out[offset + 3]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 3 + 1; return rtv; } } else out[offset + 3].HasValue() = false;
        if (n & 8  ) { var &o = out[offset + 4]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 4 + 1; return rtv; } } else out[offset + 4].HasValue() = false;
        if (n & 4  ) { var &o = out[offset + 5]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 5 + 1; return rtv; } } else out[offset + 5].HasValue() = false;
        if (n & 2  ) { var &o = out[offset + 6]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 6 + 1; return rtv; } } else out[offset + 6].HasValue() = false;
        if (n & 1  ) { var &o = out[offset + 7]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 7 + 1; return rtv; } } else out[offset + 7].HasValue() = false;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (n & v) { var &o = out[offset + i]; o.HasValue() = true; new (&o.Value()) T(); rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + i + 1; return rtv; } } else out[offset + i].HasValue() = false;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}
template<typename T> INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<T> * const &out, UInt32 const &len) {
    assert ( out );
    if (len == 0)
        return ConvertState_Success;
    var bytes = (len + 7) >> 3;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    var p = bin.Buffer() + bin.Offset();
    bin.Offset() += bytes;
    var mod = len % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        var offset = i << 3;
        if (n & 128) { var &o = out[offset + 0]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 0 + 1; return rtv; } } else out[offset + 0] = Null;
        if (n & 64 ) { var &o = out[offset + 1]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 1 + 1; return rtv; } } else out[offset + 1] = Null;
        if (n & 32 ) { var &o = out[offset + 2]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 2 + 1; return rtv; } } else out[offset + 2] = Null;
        if (n & 16 ) { var &o = out[offset + 3]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 3 + 1; return rtv; } } else out[offset + 3] = Null;
        if (n & 8  ) { var &o = out[offset + 4]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 4 + 1; return rtv; } } else out[offset + 4] = Null;
        if (n & 4  ) { var &o = out[offset + 5]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 5 + 1; return rtv; } } else out[offset + 5] = Null;
        if (n & 2  ) { var &o = out[offset + 6]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 6 + 1; return rtv; } } else out[offset + 6] = Null;
        if (n & 1  ) { var &o = out[offset + 7]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + 7 + 1; return rtv; } } else out[offset + 7] = Null;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (n & v) { var &o = out[offset + i]; o = Null; o.HasValue() = true; new (&o.Value()) T(); var rtv = Read(bin, o.Value()); if (rtv) { out.Length() = offset + i + 1; return rtv; } } else out[offset + i] = Null;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}




INLINE void BinaryFormatter_Normal::Write(Binary &bin, Boolean const * const &in, UInt32 const &len) {
    assert ( in );
    if (len == 0)
        return;
    var bytes = (len + 7) >> 3;
    bin.EnsureSpace(bytes);
    var p = bin.Buffer() + bin.Length();
    bin.Length() += bytes;
    var mod = len % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        Byte n = 0;
        var offset = i << 3;
        if (in[offset + 0]) n |= 128;
        if (in[offset + 1]) n |= 64;
        if (in[offset + 2]) n |= 32;
        if (in[offset + 3]) n |= 16;
        if (in[offset + 4]) n |= 8;
        if (in[offset + 5]) n |= 4;
        if (in[offset + 6]) n |= 2;
        if (in[offset + 7]) n |= 1;
        p[i] = n;
    }
    if (mod) {
        Byte n = 0;
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (in[offset + i]) n |= v;
            v >>= 1;
        }
        p[bytes] = n;
    }
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, Nullable<Boolean> const * const &in, UInt32 const &len) {
    assert ( in );
    if (len == 0)
        return;
    var bytes = (len + 7) >> 3;
    bin.EnsureSpace(bytes * 2);
    var p = bin.Buffer() + bin.Length();
    var p2 = p + bytes;         // 空值标记后面的值区域
    Byte p2n = 0, p2v = 128;    // 值区域当前操作的 byte, 或 值
    var mod = len % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        Byte n = 8;
        var offset = i << 3;
        if (in[offset + 0].HasValue()) { n |= 128; if (in[offset + 0].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 1].HasValue()) { n |= 64;  if (in[offset + 1].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 2].HasValue()) { n |= 32;  if (in[offset + 2].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 3].HasValue()) { n |= 16;  if (in[offset + 3].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 4].HasValue()) { n |= 8;   if (in[offset + 4].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 5].HasValue()) { n |= 4;   if (in[offset + 5].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 6].HasValue()) { n |= 2;   if (in[offset + 6].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        if (in[offset + 7].HasValue()) { n |= 1;   if (in[offset + 7].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
        p[i] = n;
    }
    if (mod) {
        Byte n = 0;
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (in[offset + i].HasValue()) { n |= v; if (in[offset + i].Value()) p2n |= p2v; p2v >>= 1; if (p2v == 0) { *p2 = p2n; ++p2; p2n = 0; p2v = 128; } }
            v >>= 1;
        }
        p[bytes] = n;
    }
    if (p2v != 128) {
        *p2 = p2n;
        bin.Length() += UInt32(p2 - p + 1);
    }
    else
        bin.Length() += UInt32(p2 - p);
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<Nullable<Boolean>> const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length());            // List<Null<Bool>> 是数组态
}
INLINE void BinaryFormatter_Normal::Write(Binary &bin, List<Boolean> const &in) {
    Write(bin, in.Length());
    Write(bin, in.Buffer(), in.Length()); 
}

INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Boolean * const &out, UInt32 const &bits) {
    assert ( out );
    var bytes = (bits + 7) >> 3;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    UInt32 idx = 0;
    var p = bin.Buffer() + bin.Offset();
    bin.Offset() += bytes;
    var mod = bits % 8;
    if (mod)
        --bytes;
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        out[idx++] = (n & 128)  ? true : false;
        out[idx++] = (n & 64)   ? true : false;
        out[idx++] = (n & 32)   ? true : false;
        out[idx++] = (n & 16)   ? true : false;
        out[idx++] = (n & 8)    ? true : false;
        out[idx++] = (n & 4)    ? true : false;
        out[idx++] = (n & 2)    ? true : false;
        out[idx++] = (n & 1)    ? true : false;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        for (UInt32 i = 0; i < mod; ++i) {
            out[idx++] = (n & v) ? true : false;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, Nullable<Boolean> * const &out, UInt32 const &len) {
    assert ( out );
    if (len == 0)
        return ConvertState_Success;
    var bytes = (len + 7) >> 3;
    var p = bin.Buffer() + bin.Offset();
    var p2 = p + bytes;
    bin.Offset() += bytes;
    if (bin.Length() < bin.Offset() + bytes)
        return ConvertState_NotEnoughData;
    var mod = len % 8;
    if (mod)
        --bytes;
    Byte p2n = 0, p2v = 128;                // 值区域当前操作的 byte, 或 值
    for (UInt32 i = 0; i < bytes; ++i) {
        var n = p[i];
        var offset = i << 3;
        if (n & 128) { var &o = out[offset + 0]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 0].HasValue() = false;
        if (n & 64 ) { var &o = out[offset + 1]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 1].HasValue() = false;
        if (n & 32 ) { var &o = out[offset + 2]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 2].HasValue() = false;
        if (n & 16 ) { var &o = out[offset + 3]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 3].HasValue() = false;
        if (n & 8  ) { var &o = out[offset + 4]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 4].HasValue() = false;
        if (n & 4  ) { var &o = out[offset + 5]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 5].HasValue() = false;
        if (n & 2  ) { var &o = out[offset + 6]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 6].HasValue() = false;
        if (n & 1  ) { var &o = out[offset + 7]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + 7].HasValue() = false;
    }
    if (mod) {
        var n = p[bytes];
        Byte v = 128;
        var offset = bytes << 3;
        for (UInt32 i = 0; i < mod; ++i) {
            if (n & v) { var &o = out[offset + i]; o.HasValue() = true; if (p2v == 128) { var rtv = Read(bin, p2n); if (rtv) return rtv; } o.Value() = (p2n & p2v) > 0; p2v >>= 1; if (p2v == 0) p2v = 128; } else out[offset + i].HasValue() = false;
            v >>= 1;
        }
    }
    return ConvertState_Success;
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<Nullable<Boolean>> &out) {
    UInt32 len;
    var rtv = Read(bin, len);
    if (rtv)
        return rtv;
    out.Clear();
    out.EnsureLength(len);
    return Read(bin, out.Buffer(), len);            // List<Null<Bool>> 是数组态
}
INLINE ConvertStates BinaryFormatter_Normal::Read(Binary &bin, List<Boolean> &out) {
    UInt32 len; 
    var rtv = Read(bin, len); 
    if (rtv) return rtv; 
    out.Clear(); 
    out.EnsureLength(len); 
    return Read(bin, (Boolean *)out.Buffer(), len);
}



}
#endif
