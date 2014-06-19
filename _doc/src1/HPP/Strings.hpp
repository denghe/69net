#ifndef STRINGS_HPP
#define STRINGS_HPP
namespace Lib3 {
namespace Strings {



template<typename T>
INLINE SByte const * ToString(T const &o) {
    var idx = ToString_CharBuferIndex;
    var p = ToString_CharBuffer[idx];
    Converts::Convert(o, p);
    ToString_CharBuferIndex = (idx + 1) % ToString_BuferCount;
    return p;
}

template<typename T>
INLINE Char const * ToWString(T const &o) {
    var idx = ToString_WCharBuferIndex;
    var p = ToString_WCharBuffer[idx++];
    Converts::Convert(o, p);
    ToString_CharBuferIndex = (idx + 1) % ToString_BuferCount;
    return p;
}

template<typename T>
INLINE SByte const * ToString_Hex(T const &o) {
    var idx = ToString_CharBuferIndex;
    var p = ToString_CharBuffer[idx];
    Converts::Convert_Hex(o, p);
    ToString_CharBuferIndex = (idx + 1) % ToString_BuferCount;
    return p;
}

template<typename T>
INLINE Char const * ToWString_Hex(T const &o) {
    var idx = ToString_WCharBuferIndex;
    var p = ToString_WCharBuffer[idx++];
    Converts::Convert_Hex(o, p);
    ToString_CharBuferIndex = (idx + 1) % ToString_BuferCount;
    return p;
}


template<typename T>
INLINE SByte const * ToString(T const &o, SByte * const &buf) {
    Converts::Convert(o, buf);
    return buf;
}

template<typename T>
INLINE Char const * ToWString(T const &o, Char * const &buf) {
    Converts::Convert(o, buf);
    return buf;
}

template<typename T>
INLINE SByte const * ToString_Hex(T const &o, SByte * const &buf) {
    Converts::Convert_Hex(o, buf);
    return buf;
}

template<typename T>
INLINE Char const * ToWString_Hex(T const &o, Char * const &buf) {
    Converts::Convert_Hex(o, buf);
    return buf;
}




template<typename T>
INLINE UInt32 ToString_GetMaxLength() {
    return sizeof(T) * 3;
}

template<typename T, UInt32 len>
INLINE UInt32 ToString_GetMaxLength() {
    if (sizeof(T) > 1)
        return len * 3;
    return len;
}






template<typename T>
INLINE void Write(SByte * const &buf, UInt32 &offset, T const &v) {
    offset += Converts::Convert(v, buf + offset);
}
template<typename T>
INLINE void WriteHex(SByte * const &buf, UInt32 &offset, T const &v) {
    offset += Converts::Convert_Hex(v, buf + offset);
}
template<typename T>
INLINE void Write(Char * const &buf, UInt32 &offset, T const &v) {
    offset += Converts::Convert(v, buf + offset);
}
template<typename T>
INLINE void WriteHex(Char * const &buf, UInt32 &offset, T const &v) {
    offset += Converts::Convert_Hex(v, buf + offset);
}









template<typename T>
INLINE void WriteNull(T * const &buf, UInt32 &offset) {
	Write(buf, offset, "(Null)", 6);
}

template <typename T>
INLINE void Write(SByte * const &buf, UInt32 &offset, Nullable<T> const &nv) {
    if (nv.HasValue())
        Write(buf, offset, nv.Value());
    else
        WriteNull(buf, offset);
}

template <typename T>
INLINE void WriteHex(SByte * const &buf, UInt32 &offset, Nullable<T> const &nv) {
    if (nv.HasValue())
        WriteHex(buf, offset, nv.Value());
    else
    	WriteNull(buf, offset);
}




template <typename T>
INLINE void Write(Char * const &buf, UInt32 &offset, Nullable<T> const &nv) {
    if (nv.HasValue())
        Write(buf, offset, nv.Value());
    else
        WriteNull(buf, offset);
}

template <typename T>
INLINE void WriteHex(Char * const &buf, UInt32 &offset, Nullable<T> const &nv) {
    if (nv.HasValue())
        WriteHex(buf, offset, nv.Value());
    else
        WriteNull(buf, offset);
}







template<UInt32 len, class T>
INLINE void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len]) {
    Write(s);
}
template<UInt32 len, class T, typename T0>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/StringsWF.inc"
    #undef CS_P0
}
template<UInt32 len, class T, typename T0, typename T1>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1) {
    #define CS_P1
    #include "../INC/StringsWF.inc"
    #undef CS_P1
}
template<UInt32 len, class T, typename T0, typename T1, typename T2>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/StringsWF.inc"
    #undef CS_P2
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/StringsWF.inc"
    #undef CS_P3
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/StringsWF.inc"
    #undef CS_P4
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/StringsWF.inc"
    #undef CS_P5
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/StringsWF.inc"
    #undef CS_P6
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/StringsWF.inc"
    #undef CS_P7
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/StringsWF.inc"
    #undef CS_P8
}
template<UInt32 len, class T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(SByte * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/StringsWF.inc"
    #undef CS_P9
}



template<UInt32 len, class T>
INLINE void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len]) {
    Write(s);
}
template<UInt32 len, class T,  typename T0>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/StringsWF.inc"
    #undef CS_P0
}
template<UInt32 len, class T,  typename T0, typename T1>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0,      T1 const &p1) {
    #define CS_P1
    #include "../INC/StringsWF.inc"
    #undef CS_P1
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/StringsWF.inc"
    #undef CS_P2
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/StringsWF.inc"
    #undef CS_P3
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/StringsWF.inc"
    #undef CS_P4
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/StringsWF.inc"
    #undef CS_P5
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/StringsWF.inc"
    #undef CS_P6
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/StringsWF.inc"
    #undef CS_P7
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/StringsWF.inc"
    #undef CS_P8
}
template<UInt32 len, class T,  typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void WriteFormat(Char * const &buf, UInt32 &offset, T const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/StringsWF.inc"
    #undef CS_P9
}








template<UInt32 len>
UInt32 GetLength(SByte const (&s)[len]) {
    if (len == 0)
        return 0;
    // 如果 s 起始地址没有 4/8 倍对齐, 就先处理冒出来的
    UInt32 mod = (size_t)s & (sizeof(size_t) - 1), i = 0;
    if (mod) {
        if (len > mod) {
            for (; i < mod; ++i)
                if (s[i] == '\0')
                    return i + 1;
        } else {
            for (; i < len; ++i)
                if (s[i] == '\0')
                    return i + 1;
            return len;
        }
    }
    size_t himagic = 0x80808080, lomagic = 0x01010101;        // 设置 魔法数
    if (sizeof(size_t) == 8) {              // 兼容 x64. 左移2次16是防gcc报警
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    for (; i < len; i += sizeof(size_t)) {
        var n = *(size_t*)(s + i);
        if (((n - lomagic) & ~n & himagic) != 0) {      // 扫到含0, 进一步判断
            var cp = (SByte*)&n;
            if (cp[0] == 0)
                return i;
            if (cp[1] == 0)
                return i + 1;
            if (cp[2] == 0)
                return i + 2;
            if (cp[3] == 0)
                return i + 3;
            if (sizeof(size_t) == 8) {                  // x64
                if (cp[4] == 0)
                    return i + 4;
                if (cp[5] == 0)
                    return i + 5;
                if (cp[6] == 0)
                    return i + 6;
                if (cp[7] == 0)
                    return i + 7;
            }
        }
    }
    return len;
}

template<UInt32 len>
UInt32 GetLength(Char const (&s)[len]) {
    if (len == 0)
        return 0;
    // 如果 s 起始地址没有 4/8 倍对齐, 就先处理这几个字符
    UInt32 mod = (size_t)s & (sizeof(size_t) - 1), i = 0;
    if (mod) {
        mod >>= 1;
        if (len > mod) {
            for (; i < mod; ++i)
                if (s[i] == '\0')
                    return i + 1;
        } else {
            for (; i < len; ++i)
                if (s[i] == '\0')
                    return i + 1;
            return len;
        }
    }
    if (sizeof(size_t) == 4) {
        for (; i < len; i += (sizeof(size_t) >> 1)) {
            var n = *(size_t*)(s + i);
            var cp = (Char*)&n;
            if (cp[0] == 0)
                return i;
            if (cp[1] == 0)
                return i + 1;
        }
    }
    else if (sizeof(size_t) == 8) {
        size_t himagic = 0x80008000L, lomagic = 0x00010001L;
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
        for (; i < len; i += (sizeof(size_t) >> 1)) {
            var n = *(size_t*)(s + i);
            if (((n - lomagic) & ~n & himagic) != 0) {
                var cp = (Char*)&n;
                if (cp[0] == 0)
                    return i;
                if (cp[1] == 0)
                    return i + 1;
                if (cp[2] == 0)
                    return i + 2;
                if (cp[3] == 0)
                    return i + 3;
            }
        }
    }
    else {
        assert ( false );
    }
    return len;
}




template<typename T>
UInt32 TrimStartCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    if (bufLen == 0)
        return 0;
    UInt32 len = bufLen, i = 0, j;
Lab1:                           // for buf begin
    var c = buf[i];
    j = sLen - 1;
Lab2:                           // for s begin
    if (c == s[j]) {
        if (++i < len)
            goto Lab1;
        else {
            bufLen = 0;
            buf[0] = '\0';      // 全是排版符 故在第0个位置放 \0
            return i;
        }
    }
    if (j) {
        --j;
        goto Lab2;              // for s end, for buf end
    }
    if (i) {                    // 处理 0 个排版符
        len -= i;
        memmove (buf, buf + i, sizeof(T) * len);
    }
    bufLen = len;
    buf[len] = '\0';
    return i;
}

template<typename T>
INLINE UInt32 TrimStart(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    assert ( buf && s );
    assert ( sLen );
    if (bufLen == 0)            // 处理不传 bufLen 长
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;           // 如果自己判断长度为 0  直接返回
    return TrimStartCore(buf, bufLen, s, sLen);
}

template<typename T>
INLINE UInt32 TrimStart(T * const &buf, UInt32 &bufLen, T const * const &s) {
    assert ( buf && s );
    var sLen = GetLength(s);
    assert ( sLen );
    if (bufLen == 0)            // 处理不传 bufLen 长
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;           // 如果自己判断长度为 0  直接返回
    return TrimStartCore(buf, bufLen, s, sLen);
}


template<typename T>
UInt32 TrimEndCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    UInt32 len = bufLen, i = len - 1, j;
Lab1:
    var c = buf[i];
    j = sLen - 1;
Lab2:
    if (c == s[j]) {
        if (i) {
            --i;
            goto Lab1;
        }
        else {
            bufLen = 0;
            buf[0] = '\0';
            return len;
        }
    }
    if (j) {
        --j;
        goto Lab2;
    }
    bufLen = i + 1;
    buf[bufLen] = '\0';
    return len - bufLen;
}

template<typename T>
INLINE UInt32 TrimEnd(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    assert ( buf && s );
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimEndCore(buf, bufLen, s, sLen);
}
template<typename T>
INLINE UInt32 TrimEnd(T * const &buf, UInt32 &bufLen, T const * const &s) {
    assert ( buf && s );
    var sLen = GetLength(s);
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimEndCore(buf, bufLen, s, sLen);
}

template<typename T>
UInt32 TrimCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    UInt32 len, lenbak;
    len = lenbak = bufLen;
    UInt32 i = len - 1, j;
Lab1:
    var c = buf[i];
    j = sLen - 1;
Lab2:
    if (c == s[j]) {
        if (i) {
            --i;
            goto Lab1;
        }
        else {
            bufLen = 0;
            buf[0] = '\0';
            return len;
        }
    }
    if (j) {
        --j;
        goto Lab2;
    }
    // 到这步的时候 已确认一定有非排版字符的存在 故省掉一些处理
    len = i + 1;        // 存当前长度
    i = 0;
Lab3:
    c = buf[i];
    j = sLen - 1;
Lab4:
    if (c == s[j]) {
        ++i;
        goto Lab3;
    }
    if (j) {
        --j;
        goto Lab4;
    }

    if (i) {
        len -= i;
        memmove (buf, buf + i, sizeof(T) * len);
    } 
    else if (len == lenbak)
        return 0;

    lenbak -= len;
    bufLen = len;
    buf[len] = '\0';
    return lenbak;
}

template<typename T>
INLINE UInt32 Trim(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    assert ( buf && s );
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimCore(buf, bufLen, s, sLen);
}
template<typename T>
INLINE UInt32 Trim(T * const &buf, UInt32 &bufLen, T const * const &s) {
    assert ( buf && s );
    var sLen = GetLength(s);
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimCore(buf, bufLen, s, sLen);
}

template<typename T>
UInt32 TrimStartCore(T * const &buf, UInt32 &bufLen) {
    UInt32 len = bufLen, i = 0;
Lab1:
    var c = buf[i];
    if (c == ' ') {
        if (++i < len)
            goto Lab1;
        else {
            bufLen = 0;
            buf[0] = '\0';
            return i;
        }
    }
    if (i) {
        len -= i;
        memmove (buf, buf + i, sizeof(T) * len);
    }
    bufLen = len;
    buf[len] = '\0';
    return i;
}

template<typename T>
INLINE UInt32 TrimStart(T * const &buf, UInt32 &bufLen) {
    assert ( buf );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimStartCore(buf, bufLen);
}

template<typename T>
UInt32 TrimEndCore(T * const &buf, UInt32 &bufLen) {
    var len = bufLen;
    UInt32 i = len - 1;
Lab1:
    var c = buf[i];
    if (c == ' ') {
        if (i) {
            --i;
            goto Lab1;
        }
        else {
            bufLen = 0;
            buf[0] = '\0';
            return len;
        }
    }
    bufLen = i + 1;
    buf[bufLen] = '\0';
    return len - bufLen;
}

template<typename T>
INLINE UInt32 TrimEnd(T * const &buf, UInt32 &bufLen) {
    assert ( buf );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimEndCore(buf, bufLen);
}

template<typename T>
UInt32 TrimCore(T * const &buf, UInt32 &bufLen) {
    assert ( buf );
    UInt32 len, lenbak;
    len = lenbak = bufLen;
    UInt32 i = len - 1;
Lab1:
    var c = buf[i];
    if (c == ' ') {
        if (i) {
            --i;
            goto Lab1;
        }
        else {
            bufLen = 0;
            buf[0] = '\0';
            return len;
        }
    }
    len = i + 1;
    i = 0;
Lab3:
    c = buf[i];
    if (c == ' ') {
        ++i;
        goto Lab3;
    }
    if (i) {
        len -= i;
        memmove (buf, buf + i, sizeof(T) * len);
    } 
    else if (len == lenbak)
        return 0;
    lenbak -= len;
    bufLen = len;
    buf[len] = '\0';
    return lenbak;
}

template<typename T>
INLINE UInt32 Trim(T * const &buf, UInt32 &bufLen) {
    assert ( buf );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimCore(buf, bufLen);
}




template<typename T>
UInt32 TrimAllCore(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    UInt32 len = bufLen;
    // 设计两个索引: i, k
    // 通过 i 遍历 buf, 判断字符属性
    // 当遇到保留字符时, 如果两个索引不相等, 赋值并累加两个索引. 
    // 当遇到要过滤的字符时, 继续扫
    UInt32 i = 0, k = 0, j;
Lab1:
    var c = buf[i];
    j = 0;
Lab2:
    if (c == s[j]) {    // 过滤字符, 继续扫
        if (++i < len)
            goto Lab1;
        goto Lab3;
    }
    if (++j < sLen)
        goto Lab2;
    if (i)              // 正常字符, 赋值并累加两个索引. if (i) 是避免 i , k 相同还复制
        buf[k] = buf[i];
    ++k;
    if (++i < len)
        goto Lab1;
Lab3:                   // 此时, k 即为 保留字符长
    bufLen = k;
    if (k < len)
        buf[k] = '\0';
    return len - k;
}

template<typename T>
INLINE UInt32 TrimAll(T * const &buf, UInt32 &bufLen, T const * const &s, UInt32 const &sLen) {
    assert ( buf && s );
    if (sLen == 0)
        sLen = GetLength(s);
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimAllCore(buf, bufLen, s, sLen);
}
template<typename T>
INLINE UInt32 TrimAll(T * const &buf, UInt32 &bufLen, T const * const &s) {
    assert ( buf && s );
    var sLen = GetLength(s);
    assert ( sLen );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen == 0)
        return 0;
    return TrimAllCore(buf, bufLen, s, sLen);
}




template<typename T>
INLINE Int32 Find(T const * const &buf, T const &c) {
    assert ( buf );
    UInt32 i = 0;
Lab1:
    if (buf[i] == c)
        return i;
    else if (buf[i] == '\0')
        return -1;
    ++i;
    goto Lab1;
}

template<typename T>
INLINE Int32 FindCore(T const * const &buf, UInt32 const &bufLen, T const &c) {
    for (UInt32 i = 0; i < bufLen; ++i) {
        if (buf[i] == c)
            return i;
    }
    return -1;
}

template<typename T>
INLINE Int32 Find(T const * const &buf, UInt32 const &bufLen, T const &c) {
    assert ( buf );
    if (bufLen == 0)
        bufLen = GetLength(buf);
    return FindCore(buf, bufLen, c);
}

template<typename T>
Int32 FindCore(T const * const &buf, UInt32 const &bufLen, T const * const &s, UInt32 const &sLen) {
    var maxIdx = bufLen - sLen;
    UInt32 i = 0;
Lab1:
    var p = buf + i;
    for (UInt32 j = 0; j < sLen; ++j) {
        if (p[j] == s[j])
            continue;
        else
            if (++i <= maxIdx)
                goto Lab1;
            else
                return -1;
    }
    return i;
}

template<typename T>
Int32 Find(T const * const &buf, UInt32 const &bufLen, T const * const &s, UInt32 const &sLen) {
    assert ( buf && s );
    if (sLen == 0) {
        sLen = GetLength(s);
        if (sLen == 0)
            return -1;
    }
    if (bufLen == 0)
        bufLen = GetLength(buf);
    if (bufLen < sLen)
        return -1;
    return FindCore(buf, bufLen, s, sLen);
}

template<typename T>
INLINE Int32 Find(T const * const &buf, UInt32 const &bufLen, T const * const &s) {
    return Find(buf, bufLen, s, 0);
}

template<typename T>
INLINE Int32 Find(T const * const &buf, T const * const &s, UInt32 const &sLen) {
    return Find(buf, 0, s, sLen);
}

template<typename T>
INLINE Int32 Find(T const * const &buf, T const * const &s) {
    return Find(buf, 0, s, 0);
}





template<typename T>
INLINE void ToLowerCore(T * const &buf, UInt32 const &bufLen) {
    for (UInt32 i = 0; i < bufLen; ++i) {
        var c = buf[i];
        if (c >= 'A' && c <= 'Z')
            buf[i] = c + ('a' - 'A');  // 32
    }
}
template<typename T>
INLINE void ToLower(T * const &buf, UInt32 const &bufLen) {
    assert ( buf );
    var len = bufLen;
    if (len == 0) {
        len = GetLength(buf);
        if (len == 0)
            return;
    }
    ToLowerCore(buf, len);
}

template<typename T>
INLINE void ToUpperCore(T * const &buf, UInt32 const &bufLen) {
    for (UInt32 i = 0; i < bufLen; ++i) {
        var c = buf[i];
        if (c >= 'a' && c <= 'z')
            buf[i] = c - ('a' - 'A');  // 32
    }
}
template<typename T>
INLINE void ToUpper(T * const &buf, UInt32 const &bufLen) {
    assert ( buf );
    var len = bufLen;
    if (len == 0) {
        len = GetLength(buf);
        if (len == 0)
            return;
    }
    ToUpperCore(buf, len);
}





template<>
INLINE UInt32 ToString_GetMaxLength<Single>() {
    return 24;
}

template<>
INLINE UInt32 ToString_GetMaxLength<Double>() {
    return 32;
}




INLINE void Write(SByte * const &buf, UInt32 &offset, SByte const * const &s, UInt32 const &len) {
    assert ( buf );
    assert ( s );
    if (len) {
        memcpy(&buf[offset], s, len);
        offset += len;
    }
}
INLINE void Write(SByte * const &buf, UInt32 &offset, SByte const * const &s) {
    Write(buf, offset, s, Strings::GetLength(s));
}
INLINE void Write(SByte * const &buf, UInt32 &offset, Char const * const &s, UInt32 const &len) {
    if (len)
        offset += Converts::Convert_Utf8(s, len, buf + offset);
}
INLINE void Write(SByte * const &buf, UInt32 &offset, Char const * const &s) {
    Write(buf, offset, s, Strings::GetLength(s));
}


INLINE void Write(Char * const &buf, UInt32 &offset, SByte const * const &s, UInt32 const &len) {
    if (len)
        offset += Converts::Convert_Unicode(s, len, &buf[offset]);
}
INLINE void Write(Char * const &buf, UInt32 &offset, SByte const * const &s) {
    if (var len = Strings::GetLength(s))
        offset += Converts::Convert_Unicode(s, len, &buf[offset]);
}
INLINE void Write(Char * const &buf, UInt32 &offset, Char const * const &s, UInt32 const &len) {
    if (len) {
        memcpy(&buf[offset], s, sizeof(Char) * len);
        offset += len;
    }
}
INLINE void Write(Char * const &buf, UInt32 &offset, Char const * const &s) {
    if (var len = Strings::GetLength(s)) {
        memcpy(&buf[offset], s, sizeof(Char) * len);
        offset += len;
    }
}


INLINE void WriteHex(SByte * const &buf, UInt32 &offset, Byte const * const &s, UInt32 const &len) {
    for (UInt32 i = 0; i < len; ++i)
        offset += Converts::Convert_Hex(s[i], &buf[offset]);
}


INLINE void WriteHex(Char * const &buf, UInt32 &offset, Byte const * const &s, UInt32 const &len) {
    for (UInt32 i = 0; i < len; ++i)
        offset += Converts::Convert_Hex(s[i], &buf[offset]);
}


INLINE Boolean Equals(SByte const * const &s1, SByte const * const &s2, UInt32 const &s2Len) {
    return Equals(s1, 0, s2, s2Len);
}
INLINE Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2) {
    return Equals(s1, s1Len, s2, 0);
}
INLINE Boolean Equals(SByte const * const &s1, SByte const * const &s2) {
    return Equals(s1, 0, s2, 0);
}

INLINE Boolean Equals(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len) {
    return Equals(s1, 0, s2, s2Len);
}
INLINE Boolean Equals(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2) {
    return Equals(s1, s1Len, s2, 0);
}
INLINE Boolean Equals(Char const * const &s1, Char const * const &s2) {
    return Equals(s1, 0, s2, 0);
}
INLINE Boolean Equals(SByte const * const &s1, Char const * const &s2, UInt32 const &s2Len) {
    return Equals(s1, 0, s2, s2Len);
}
INLINE Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2) {
    return Equals(s1, s1Len, s2, 0);
}
INLINE Boolean Equals(SByte const * const &s1, Char const * const &s2) {
    return Equals(s1, 0, s2, 0);
}

INLINE Boolean Equals(Char const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len) {
    return Equals(s2, s2Len, s1, s1Len);
}
INLINE Boolean Equals(Char const * const &s1, SByte const * const &s2, UInt32 const &s2Len) {
    return Equals(s1, 0, s2, s2Len);
}
INLINE Boolean Equals(Char const * const &s1, UInt32 const &s1Len, SByte const * const &s2) {
    return Equals(s1, s1Len, s2, 0);
}
INLINE Boolean Equals(Char const * const &s1, SByte const * const &s2) {
    return Equals(s1, 0, s2, 0);
}

INLINE void Fill(SByte * const &buf, UInt32 const &len, SByte const &c) {
    assert ( buf );
    if (len)
        memset(buf, c, len);
}

INLINE void Fill(Char * const &buf, UInt32 const &len, Char const &c) {
    assert ( buf );
    if (len)
        for (UInt32 i = 0; i < len; ++i)
            buf[i] = c;
}










INLINE UInt32 GetLength(SByte const * const &s) {
    assert ( s );
#ifdef __GNUC__
    return (UInt32)strlen(s);
#else
    // 如果 s 起始地址没有 4/8 倍对齐, 就先处理冒出来的
    UInt32 mod = (size_t)s & (sizeof(size_t) - 1), i = 0;
    if (mod)
        for (; i < mod; ++i)
            if (s[i] == '\0')
                return i;
    size_t himagic = 0x80808080, lomagic = 0x01010101;        // 设置 魔法数
    if (sizeof(size_t) == 8) {              // 兼容 x64. 左移2次16是防gcc报警
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }
    while(true) {
        var n = *(size_t*)(s + i);
        if (((n - lomagic) & ~n & himagic) != 0) {      // 扫到含0, 进一步判断
            var cp = (SByte*)&n;
            if (cp[0] == 0)
                return i;
            if (cp[1] == 0)
                return i + 1;
            if (cp[2] == 0)
                return i + 2;
            if (cp[3] == 0)
                return i + 3;
            if (sizeof(size_t) == 8) {                  // x64
                if (cp[4] == 0)
                    return i + 4;
                if (cp[5] == 0)
                    return i + 5;
                if (cp[6] == 0)
                    return i + 6;
                if (cp[7] == 0)
                    return i + 7;
            }
        }
        i += sizeof(size_t);
    }
#endif
}
INLINE UInt32 GetLength(Char const * const &s) {
    assert ( s );
    if (sizeof(Char) == 4)
        return (UInt32)wcslen(s);
    else if (sizeof(Char) == 2) {
#ifdef __linux
        // 处理起始指针未按 4/8 字节对齐的问题. 先扫冒出来的部分
        UInt32 mod = (size_t)s & (sizeof(size_t) - 1), i = 0;
        if (mod) {
            mod >>= 1;
            for (; i < mod; ++i)
                if (s[i] == '\0')
                    return i;
        }
        if (sizeof(size_t) == 4) {  // x86 模式，每 2 Char 1 读, 直接判断
            while(true) {
                var n = *(size_t*)(s + i);
                var cp = (Char*)&n;
                if (cp[0] == 0)
                    return i;
                if (cp[1] == 0)
                    return i + 1;
                i += 2;
            }
        }   // x64 模式，每 4 Char 1 读，用魔法数一次性判断是否含 0, 再分别判断
        else if (sizeof(size_t) == 8) {
            size_t himagic = 0x80008000, lomagic = 0x00010001;      // 这样写是防止 GCC 报常数过大警告
            himagic = ((himagic << 16) << 16) | himagic;           // 这样写是防止 GCC 报位移32 警告
            lomagic = ((lomagic << 16) << 16) | lomagic;
            while(true) {
                var n = *(size_t*)(s + i);
                if (((n - lomagic) & ~n & himagic) != 0) {          // 扫到含 0, 分别判断
                    var cp = (Char*)&n;
                    if (cp[0] == 0)
                        return i;
                    if (cp[1] == 0)
                        return i + 1;
                    if (cp[2] == 0)
                        return i + 2;
                    if (cp[3] == 0)
                        return i + 3;
                }
                i += sizeof(size_t) >> 1;
            }
        }
        else {
            assert ( false );
        }
#else
        return (UInt32)wcslen(s);
#endif
    }
    else {
        assert ( false );
    }
    return 0;   // 防警告
}






// SByte
// 只适用于起始内存按 1 对齐的情况
INLINE Boolean Equals1(SByte const * const &s1, SByte const * const &s2, UInt32 const &len) {
	for(UInt32 i = 0; i < len; ++i) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}
// 只适用于起始内存按 2 对齐的情况
INLINE Boolean Equals2(SByte const * const &s1, SByte const * const &s2, UInt32 const &len) {
	UInt32 i = 0, mod = len % 2;
	for(; i < len - mod; i += 2)
		if (*(UInt16*)(s1 + i) != *(UInt16*)(s2 + i))
			return false;
	for(; i < len; ++i) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}
// 只适用于起始内存按 4 对齐的情况
INLINE Boolean Equals4(SByte const * const &s1, SByte const * const &s2, UInt32 const &len) {
	UInt32 i = 0, mod = len % 4;
	for(; i < len - mod; i += 4)
		if (*(UInt32*)(s1 + i) != *(UInt32*)(s2 + i))
			return false;
	for(; i < len; ++i) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}
// 适用于起始内存按 4或8 对齐的情况
INLINE Boolean Equals8(SByte const * const &s1, SByte const * const &s2, UInt32 const &len) {
	UInt32 i = 0, mod = len % sizeof(size_t);
	for(; i < len - mod; i += sizeof(size_t))
		if (*(size_t*)(s1 + i) != *(size_t*)(s2 + i))		//size_t在32位下是32位,64位下是64位
			return false;
	for(; i < len; ++i) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}
// Char
// 只适用于起始内存按 2 对齐的情况
INLINE Boolean Equals2(Char const * const &s1, Char const * const &s2, UInt32 const &len) {
	for(UInt32 i = 0; i < len; ++i) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}
// 只适用于起始内存按 4 对齐的情况
INLINE Boolean Equals4(Char const * const &s1, Char const * const &s2, UInt32 const &len) {
    if (sizeof(Char) == 4) {
	    for(UInt32 i = 0; i < len; ++i) {
		    if (s1[i] != s2[i])
			    return false;
	    }
	    return true;
    }
    else if (sizeof(Char) == 2) {
	    UInt32 i = 0, mod = len % 2;
	    for(; i < len - mod; i += 2)
		    if (*(UInt32*)(s1 + i) != *(UInt32*)(s2 + i))
			    return false;
	    for(; i < len; ++i) {
		    if (s1[i] != s2[i])
			    return false;
	    }
	    return true;
    }
    else {
        assert ( false );
    }

    return false;   // 防警告
}
// 适用于起始内存按 8 对齐的情况
INLINE Boolean Equals8(Char const * const &s1, Char const * const &s2, UInt32 const &len) {
    if (sizeof(Char) == 4) {
	    UInt32 i = 0, mod = len % 2;
	    for(; i < len - mod; i += 2)
		    if (*(UInt64*)(s1 + i) != *(UInt64*)(s2 + i))
			    return false;
	    for(; i < len; ++i) {
		    if (s1[i] != s2[i])
			    return false;
	    }
	    return true;
    }
    else if (sizeof(Char) == 2) {
	    UInt32 i = 0, mod = len % 4;
	    for(; i < len - mod; i += 4)
		    if (*(UInt64*)(s1 + i) != *(UInt64*)(s2 + i))
			    return false;
	    for(; i < len; ++i) {
		    if (s1[i] != s2[i])
			    return false;
	    }
	    return true;
    }
    else {
        assert ( false );
    }

    return false;   // 防警告
}


INLINE Boolean EqualsCore(SByte const * const &s1, SByte const * const &s2, UInt32 const &s1Len) {
    // 判断起始地址的对齐关系
    UInt32 mod1, mod2;
    if (sizeof(size_t) == 8) {              // x64
        mod1 = (size_t)s1 % 8;
        mod2 = (size_t)s2 % 8;
        if (mod1 == mod2) {
            if (mod1 == 0)                  // 8 字节对齐
                return Equals8(s1, s2, s1Len);
            else {                          // 先判断掉 mod1 个字符再转入字节对齐版
                if (Equals1(s1, s2, mod1)) {
                    if (UInt32 len = s1Len - mod1)
                        return Equals8(s1 + mod1, s2 + mod1, len);
                    return true;
                }
                return false;
            }
        }
        else {                              // 余数是否均为 4, 2 的倍数
            if (mod1 % 4 == 0 && mod2 % 4 == 0)
                return Equals4(s1, s2, s1Len);
            else if (mod1 % 2 == 0 && mod2 % 2 == 0)
                return Equals2(s1, s2, s1Len);
            else
                return Equals1(s1, s2, s1Len);  // 无法 SIMD, 逐个判断
        }
    }
    else if (sizeof(size_t) == 4) {         // x86
        mod1 = (size_t)s1 % 4;
        mod2 = (size_t)s2 % 4;
        if (mod1 == mod2) {
            if (mod1 == 0)                  // 4 字节对齐
                return Equals4(s1, s2, s1Len);
            else {                          // 先判断掉 mod1 个字符再转入字节对齐版
                if (Equals1(s1, s2, mod1)) {
                    if (UInt32 len = s1Len - mod1)
                        return Equals4(s1 + mod1, s2 + mod1, len);
                    return true;
                }
                return false;
            }
        }
        else {                              // 余数是否均为 2 的倍数
            if (mod1 % 2 == 0 && mod2 % 2 == 0)
                return Equals2(s1, s2, s1Len);
            else
                return Equals1(s1, s2, s1Len);  // 无法 SIMD, 逐个判断
        }
    }
    assert ( false );   // 不该执行到此

    return false;   // 防警告
}

INLINE Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, SByte const * const &s2, UInt32 const &s2Len) {
    // 各种 同指针, 0长, 1长, 长度不等 直接判断
    assert ( s1 && s2 );
    if (s1 == s2)
        return true;
    UInt32 s1Len_ = s1Len, s2Len_ = s2Len;
    if (s1Len_ == 0)
        s1Len_ = GetLength(s1);
    if (s2Len_ == 0)
        s2Len_ = GetLength(s2);
    if (s1Len_ != s2Len_)
        return false;
    // 从这开始, 已知长度相等 故可省掉一部分判断
    if (s1Len_ == 0)                         // 0 长
        return true;
    if (s1Len_ == 1 && s1[0] == s2[0])       // 1 长
        return true;
    return EqualsCore(s1, s2, s1Len_);
}




INLINE Boolean EqualsCore(Char const * const &s1, Char const * const &s2, UInt32 const &s1Len) {
    // 判断起始地址的对齐关系
    UInt32 mod1, mod2;
    if (sizeof(size_t) == 8) {              // x64
        mod1 = (size_t)s1 % 8;
        mod2 = (size_t)s2 % 8;
        if (mod1 == mod2) {
            if (mod1 == 0)                  // 8 字节对齐
                return Equals8(s1, s2, s1Len);
            else {                          // 先判断掉 mod1 个字符再转入字节对齐版
                if (sizeof(Char) == 4) {   // 如果是 Linux 的 4 字节版 Char, 只能余 4
                    if (s1[0] == s2[0]) {
                        if (UInt32 len = s1Len - 1)
                            return Equals8(s1 + 1, s2 + 1, len);
                        return true;
                    }
                }
                else if (sizeof(Char) == 2) {
                    if (Equals2(s1, s2, mod1 >> 1)) {
                        if (UInt32 len = s1Len - (mod1 >> 1))
                            return Equals8(s1 + mod1, s2 + mod1, len);
                        return true;
                    }
                }
                else {
                    assert ( false );       // 未处理的 Char 长度
                }
                return false;
            }
        }
        else {                              // 余数是否均为 4, 2 的倍数
            if (sizeof(Char) == 4) {
                return Equals4(s1, s2, s1Len);
            }
            else if (sizeof(Char) == 2) {
                if (mod1 % 4 == 0 && mod2 % 4 == 0)
                    return Equals4(s1, s2, s1Len);
                else
                    return Equals2(s1, s2, s1Len);
            }
            else {
                assert ( false );
            }
        }
    }
    else if (sizeof(size_t) == 4) {         // x86
        mod1 = (size_t)s1 % 4;
        mod2 = (size_t)s2 % 4;
        if (mod1 == mod2) {
            if (mod1 == 0)                  // 4 字节对齐 (如果是 Linux 的 4 字节版 Char, 必然也是 4 字节对齐)
                return Equals4(s1, s2, s1Len);
            else {                          // 先判断掉 mod1 个字符再转入字节对齐版
                if (s1[0] == s2[0]) {           // mod1 只可能等于 2
                    if (UInt32 len = s1Len - 1)
                        return Equals4(s1 + 1, s2 + 1, len);
                    return true;
                }
                return false;
            }
        }
        else                                // 余数必然为 2 的倍数
            return Equals2(s1, s2, s1Len);
    }
    assert ( false );   // 不该执行到此

    return false;   // 防警告
}

INLINE Boolean Equals(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) {
    // 各种 同指针, 0长, 1长, 长度不等 直接判断
    assert ( s1 && s2 );
    if (s1 == s2)
        return true;
    UInt32 s1Len_ = s1Len, s2Len_ = s2Len;
    if (s1Len_ == 0)
        s1Len_ = GetLength(s1);
    if (s2Len_ == 0)
        s2Len_ = GetLength(s2);
    if (s1Len_ != s2Len_)
        return false;
    // 从这开始, 已知长度相等 故可省掉一部分判断
    if (s1Len_ == 0)                         // 0 长
        return true;
    if (s1Len_ == 1 && s1[0] == s2[0])       // 1 长
        return true;
    return EqualsCore(s1, s2, s1Len_);
}




INLINE Boolean EqualsCore(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) {
    char buf[3];            // UCS2 : 3 字节够了
    UInt32 i = 0;           // i: s1 idx    j: s2 idx    k: buf idx
    for (UInt32 j = 0; j < s2Len; ++j) {        
        var len = Converts::Convert_Utf8(s2[j], buf);
        for (UInt32 k = 0; k < len; k++) {
            if (buf[k] != s1[i++])
                return false;
            if (i == s1Len)
                return false;
        }
    }
    return true;
}
INLINE Boolean Equals(SByte const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) {
    assert ( s1 && s2 );
    UInt32 s1Len_ = s1Len, s2Len_ = s2Len;
    if (s1Len_ == 0)
        s1Len_ = GetLength(s1);
    if (s2Len_ == 0)
        s2Len_ = GetLength(s2);
    if (s1Len_ == 0)
        if (s2Len_ == 0)
            return true;
        else
            return false;
    else
        if (s2Len_ == 0)
            return false;
    return EqualsCore(s1, s1Len_, s2, s2Len_);
}













}}
#endif
