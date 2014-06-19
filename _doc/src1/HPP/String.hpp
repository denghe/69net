#ifndef STRING_HPP
#define STRING_HPP
namespace Lib3 {



template<UInt32 len>
INLINE String::String(SByte const (&s)[len]) {
    if (len == 0)
        Init();
    else {
        Init(len);
        _length = Converts::Convert_Unicode(s, len, _buffer);
    }
}


template<UInt32 len>
INLINE String::String(Char const (&s)[len]) {
    Init(s, len);
}

template<typename T>
INLINE String::String(T const &v) {
    _length = Converts::Convert(v, _originalBuffer);
    _bufferLength = WString_OriginalBufferLength;
    _buffer = _originalBuffer;
    _bufferState = BufferState_Original;
}

template<typename T>
INLINE Boolean String::Contains(T const &v)  const {
    if (_length == 0)
        return false;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    return ContainsCore(buf, len);
}

template<typename T>
INLINE Int32 String::IndexOf(T const &v) const {
    if (_length == 0)
        return -1;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    return IndexOfCore(buf, len);
}

template<typename T>
INLINE Int32 String::IndexOf(UInt32 const &offset, T const &v) const {
    if (_length <= offset)
        return -1;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    return IndexOfCore(offset, buf, len);
}

template<typename T>
INLINE Boolean String::StartsWith(T const &v) const {
    if (_length == 0)
        return false;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len > _length)
        return false;
    return StartsWithCore(buf, len);
}

template<typename T>
INLINE Boolean String::EndsWith(T const &v) const {
    if (_length == 0)
        return false;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len > _length)
        return false;
    return EndsWithCore(buf, len);
}

template<typename T>
INLINE String & String::Insert(UInt32 const &offset, T const &v) {
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    return InsertCore(offset, buf, len);
}

template<typename T>
INLINE String & String::Append(T const &v) {
    EnsureSpace(Strings::ToString_GetMaxLength<T>());
    var len = Converts::Convert(v, _buffer + _length);
    _length += len;
    return *this;
}

template<typename T>
INLINE String & String::Append(UInt32 const &fixLen, T const &v) {
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    return AppendCore(fixLen, buf, len);
}

template<typename T>
INLINE String String::Replace(T const &v1, T const &v2) const {
    ALIGN8( Char buf1[64] );
    ALIGN8( Char buf2[64] );
    var len1 = Converts::Convert(v1, buf1);
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceCore(buf1, len1, buf2, len2);
}
template<typename T>
INLINE String String::Replace(Char const &c1, T const &v2) const {
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceCore(&c1, 1, buf2, len2);
}
template<typename T>
INLINE String String::Replace(T const &v1, Char const &c2) const {
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceCore(buf1, len1, &c2, 1);
}
template<typename T>
INLINE String String::Replace(String const &s1, T const &v2) const {
    if (s1.Length() == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceCore(s1.Buffer(), s1.Length(), buf2, len2);
}
template<typename T>
INLINE String String::Replace(T const &v1, String const &s2) const {
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceCore(buf1, len1, s2.Buffer(), s2.Length());
}
template<typename T>
INLINE String String::Replace(Char const * const &s1, UInt32 const &s1Len, T const &v2) const {
    if (s1Len == 0)
        s1Len == Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceCore(s1, s1Len, buf2, len2);
}
template<typename T>
INLINE String String::Replace(Char const * const &s1, T const &v2) const {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceCore(s1, s1Len, buf2, len2);
}
template<typename T>
INLINE String String::Replace(T const &v1, Char const * const &s2, UInt32 const &s2Len) const {
    if (s2Len == 0)
        s2Len == Strings::GetLength(s2);
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceCore(buf1, len1, s2, s2Len);
}
template<typename T>
INLINE String String::Replace(T const &v1, Char const * const &s2) const {
    var s2Len = Strings::GetLength(s2);
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceCore(buf1, len1, s2, s2Len);
}

template<typename T>
INLINE String & String::ReplaceSelf(T const &v1, T const &v2) {
    ALIGN8( Char buf1[64] );
    ALIGN8( Char buf2[64] );
    var len1 = Converts::Convert(v1, buf1);
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceSelfCore(buf1, len1, buf2, len2);
}
template<typename T>
INLINE String & String::ReplaceSelf(Char const &c1, T const &v2) {
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceSelfCore(&c1, 1, buf2, len2);
}
template<typename T>
INLINE String & String::ReplaceSelf(T const &v1, Char const &c2) {
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceSelfCore(buf1, len1, &c2, 1);
}
template<typename T>
INLINE String & String::ReplaceSelf(String const &s1, T const &v2) {
    if (s1.Length() == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceSelfCore(s1.Buffer(), s1.Length(), buf2, len2);
}
template<typename T>
INLINE String & String::ReplaceSelf(T const &v1, String const &s2) {
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceSelfCore(buf1, len1, s2.Buffer(), s2.Length());
}
template<typename T>
INLINE String & String::ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, T const &v2) {
    if (s1Len == 0)
        s1Len == Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceSelfCore(s1, s1Len, buf2, len2);
}
template<typename T>
INLINE String & String::ReplaceSelf(Char const * const &s1, T const &v2) {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    ALIGN8( Char buf2[64] );
    var len2 = Converts::Convert(v2, buf2);
    return ReplaceSelfCore(s1, s1Len, buf2, len2);
}
template<typename T>
INLINE String & String::ReplaceSelf(T const &v1, Char const * const &s2, UInt32 const &s2Len) {
    if (s2Len == 0)
        s2Len == Strings::GetLength(s2);
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceSelfCore(buf1, len1, s2, s2Len);
}
template<typename T>
INLINE String & String::ReplaceSelf(T const &v1, Char const * const &s2) {
    var s2Len = Strings::GetLength(s2);
    ALIGN8( Char buf1[64] );
    var len1 = Converts::Convert(v1, buf1);
    return ReplaceSelfCore(buf1, len1, s2, s2Len);
}


template<UInt32 len>
INLINE String & String::AppendFormat(SByte const (&s)[len]) {
    return Append(s);
}
template<UInt32 len, typename T0>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/StringAF.inc"
    #undef CS_P0
    return *this;
}
template<UInt32 len, typename T0, typename T1>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1) {
    #define CS_P1
    #include "../INC/StringAF.inc"
    #undef CS_P1
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/StringAF.inc"
    #undef CS_P2
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/StringAF.inc"
    #undef CS_P3
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/StringAF.inc"
    #undef CS_P4
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/StringAF.inc"
    #undef CS_P5
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/StringAF.inc"
    #undef CS_P6
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/StringAF.inc"
    #undef CS_P7
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/StringAF.inc"
    #undef CS_P8
    return *this;
}
template<UInt32 len, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
String & String::AppendFormat(SByte const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/StringAF.inc"
    #undef CS_P9
    return *this;
}



template<UInt32 len>
INLINE String & String::AppendFormat(Char const (&s)[len]) {
    return Append(s);
}
template<UInt32 len , typename T0>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0) {
    #define CS_P0
    #include "../INC/StringAFW.inc"
    #undef CS_P0
    return *this;
}
template<UInt32 len , typename T0, typename T1>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1) {
    #define CS_P1
    #include "../INC/StringAFW.inc"
    #undef CS_P1
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2) {
    #define CS_P2
    #include "../INC/StringAFW.inc"
    #undef CS_P2
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    #define CS_P3
    #include "../INC/StringAFW.inc"
    #undef CS_P3
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    #define CS_P4
    #include "../INC/StringAFW.inc"
    #undef CS_P4
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    #define CS_P5
    #include "../INC/StringAFW.inc"
    #undef CS_P5
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    #define CS_P6
    #include "../INC/StringAFW.inc"
    #undef CS_P6
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    #define CS_P7
    #include "../INC/StringAFW.inc"
    #undef CS_P7
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    #define CS_P8
    #include "../INC/StringAFW.inc"
    #undef CS_P8
    return *this;
}
template<UInt32 len , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
String & String::AppendFormat(Char const (&s)[len], T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    #define CS_P9
    #include "../INC/StringAFW.inc"
    #undef CS_P9
    return *this;
}



template<typename T>
INLINE String & String::AppendFormat(T const &t) {
    return Append(t);
}
template<typename T0>
String & String::AppendFormat(SString const &str, T0 const &p0) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P0
    #include "../INC/StringAF.inc"
    #undef CS_P0
    return *this;
}
template<typename T0, typename T1>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P1
    #include "../INC/StringAF.inc"
    #undef CS_P1
    return *this;
}
template<typename T0, typename T1, typename T2>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P2
    #include "../INC/StringAF.inc"
    #undef CS_P2
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P3
    #include "../INC/StringAF.inc"
    #undef CS_P3
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P4
    #include "../INC/StringAF.inc"
    #undef CS_P4
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P5
    #include "../INC/StringAF.inc"
    #undef CS_P5
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P6
    #include "../INC/StringAF.inc"
    #undef CS_P6
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P7
    #include "../INC/StringAF.inc"
    #undef CS_P7
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P8
    #include "../INC/StringAF.inc"
    #undef CS_P8
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
String & String::AppendFormat(SString const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P9
    #include "../INC/StringAF.inc"
    #undef CS_P9
    return *this;
}




template<typename T0>
String & String::AppendFormat(String const &str, T0 const &p0) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P0
    #include "../INC/StringAFW.inc"
    #undef CS_P0
    return *this;
}
template<typename T0, typename T1>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P1
    #include "../INC/StringAFW.inc"
    #undef CS_P1
    return *this;
}
template<typename T0, typename T1, typename T2>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P2
    #include "../INC/StringAFW.inc"
    #undef CS_P2
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P3
    #include "../INC/StringAFW.inc"
    #undef CS_P3
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P4
    #include "../INC/StringAFW.inc"
    #undef CS_P4
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P5
    #include "../INC/StringAFW.inc"
    #undef CS_P5
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P6
    #include "../INC/StringAFW.inc"
    #undef CS_P6
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P7
    #include "../INC/StringAFW.inc"
    #undef CS_P7
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P8
    #include "../INC/StringAFW.inc"
    #undef CS_P8
    return *this;
}
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
String & String::AppendFormat(String const &str, T0 const &p0, T1 const &p1, T2 const &p2, T3 const &p3, T4 const &p4, T5 const &p5, T6 const &p6, T7 const &p7, T8 const &p8, T9 const &p9) {
    var s = str.Buffer();
    var len = str.Length();
    #define CS_P9
    #include "../INC/StringAFW.inc"
    #undef CS_P9
    return *this;
}





template<typename T>
INLINE String & operator<<(String &o, T const &in) {
    o.Append(in);
    return o;
}
template<typename T>
INLINE String && operator<<(String &&o, T const &in) {
    o.Append(in);
    return (String &&)o;
}
template<typename T>
INLINE String operator+(String const &s, T const &v) {
    String tmp(s.Length() + Strings::ToString_GetMaxLength<T>());
    memcpy(tmp.Buffer(), s.Buffer(), s.Length() * sizeof(Char));
    tmp.Length() = s.Length() + Converts::Convert(v, tmp.Buffer() + s.Length());
    return tmp;
}
template<typename T>
INLINE String operator+(T const &v, String const &s) {
    String tmp(s.Length() + Strings::ToString_GetMaxLength<T>());
    tmp.Length() = Converts::Convert(v, tmp.Buffer());
    memcpy(tmp.Buffer() + tmp.Length(), s.Buffer(), s.Length() * sizeof(Char));
    tmp.Length() += s.Length();
    return tmp;
}
template<typename T>
INLINE String && operator+(String &&s, T const &v) {
    s.EnsureSpace(Strings::ToString_GetMaxLength<T>());
    s.Length() += Converts::Convert(v, s.Buffer() + s.Length());
    return (String &&)s;
}
template<typename T>
INLINE Boolean operator==(String const &s, T const &v) {
    if (s.Length() == 0)
        return false;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len != s.Length())
        return false;
    return Strings::EqualsCore(s.Buffer(), buf, len);
}
template<typename T>
INLINE Boolean operator==(T const &v, String const &s) {
    if (s.Length() == 0)
        return false;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len != s.Length())
        return false;
    return Strings::EqualsCore(s.Buffer(), buf, len);
}
template<typename T>
INLINE Boolean operator!=(String const &s, T const &v) {
    if (s.Length() == 0)
        return true;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len != s.Length())
        return false;
    return !Strings::EqualsCore(s.Buffer(), s.Length(), buf, len);
}
template<typename T>
INLINE Boolean operator!=(T const &v, String const &s) {
    if (s.Length() == 0)
        return true;
    ALIGN8( Char buf[64] );
    var len = Converts::Convert(v, buf);
    if (len != s.Length())
        return false;
    return !Strings::EqualsCore(s.Buffer(), buf, len);
}





































// 内联区
//


INLINE Char * & String::Buffer() {
    return _buffer;
}
INLINE UInt32 & String::BufferLength() {
    return _bufferLength;
}
INLINE BufferStates & String::BufferState() {
    return _bufferState;
}
INLINE UInt32 & String::Length() {
    return _length;
}
INLINE Char * const & String::Buffer() const {
    return _buffer;
}
INLINE UInt32 const & String::BufferLength() const {
    return _bufferLength;
}
INLINE BufferStates const & String::BufferState() const {
    return _bufferState;
}
INLINE UInt32 const & String::Length() const {
    return _length;
}

INLINE void String::Init() {
    _length = 0;
    _buffer = _originalBuffer;
    _bufferLength = WString_OriginalBufferLength;
    _bufferState = BufferState_Original;
}

INLINE void String::Init(UInt32 const &len) {
    if (len <= WString_OriginalBufferLength) {
        _buffer = _originalBuffer;
        _bufferLength = WString_OriginalBufferLength;
        _bufferState = BufferState_Original;
    } else {
        _buffer = new Char[len];
        _bufferLength = len;
        _bufferState = BufferState_New;
    }
}

INLINE void String::Init(Char const * const &s, UInt32 const &len) {
    if (len == 0)
        Init();
    else {
        _length = len;
        Init(len);
        memcpy(_buffer, s, len * sizeof(Char));
    }
}

INLINE String::String() {
    Init();
}

INLINE String::String(UInt32 const &bufLen)
    : _length(0)
{
    Init(bufLen);
}
INLINE String::String(Int32 const &bufLen)
    : _length(0)
{
    Init(bufLen);
}

INLINE String::String(Char const * const &s, UInt32 const &len) {
    assert ( s );
    Init(s, len);
}
INLINE String::String(Char const * const &s) {
    assert ( s );
    Init(s, Strings::GetLength(s));
}

INLINE String::String(UInt32 const &inBufLen, Char * const &inBuf) {
    assert ( inBufLen );
    assert ( inBuf );
    _length = inBufLen;
    _buffer = inBuf;
    _bufferLength = inBufLen;
    _bufferState = BufferState_Custom;
}

INLINE String::String(String const &s) {
    _length = s.Length();
    if (_length > WString_OriginalBufferLength) {
        _buffer = new Char[s.Length()];
        _bufferState = BufferState_New;
        _bufferLength = s.Length();
    } else {
        _buffer = _originalBuffer;
        _bufferState = BufferState_Original;
        _bufferLength = WString_OriginalBufferLength;
    }
    memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
}
INLINE String::String(String &&s) {
    _length = s.Length();
    if (s.BufferState() == BufferState_New) {
        s.BufferState() = BufferState_Original;   // 不让 s 删 Buffer
        _buffer = s.Buffer();
        _bufferLength = s.BufferLength();
        _bufferState = BufferState_New;
    }
    else {
        if (_length > WString_OriginalBufferLength) {
            _buffer = new Char[s.Length()];
            _bufferState = BufferState_New;
            _bufferLength = s.Length();
        } else {
            _buffer = _originalBuffer;
            _bufferState = BufferState_Original;
            _bufferLength = WString_OriginalBufferLength;
        }
        memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
    }
}

INLINE String::~String() {
    if (_bufferState == BufferState_New)
        delete [] _buffer;
}

INLINE String & String::EnsureSpace(UInt32 const &len) {
    if (_length + len > _bufferLength) {
        var bak_buff = _buffer;
        var bak_stat = _bufferState;
        _bufferLength = (_length + len) * 2;
        _buffer = new Char[_bufferLength];
        _bufferState = BufferState_New;
        memcpy(_buffer, bak_buff, _length * sizeof(Char));
        if (bak_stat == BufferState_New)
            delete [] bak_buff;
    }
    return *this;
}

INLINE String & String::EnsureLength() {
    if (_length > _bufferLength) {
        var bak_buff = _buffer;
        var bak_stat = _bufferState;
        _bufferLength = _length;
        _buffer = new Char[_length];
        _bufferState = BufferState_New;
        memcpy(_buffer, bak_buff, _length * sizeof(Char));
        if (bak_stat == BufferState_New)
            delete [] bak_buff;
    }
    return *this;
}

INLINE String & String::Clear() {
    if (_bufferState == BufferState_New)
        delete [] _buffer;
    Init();
    return *this;
}

INLINE String & String::AppendZero() {
    EnsureSpace(1);
    _buffer[_length] = 0;
    return *this;
}




INLINE String String::TrimCore(Char const * const &s, UInt32 const &len) const {
    String tmp(_buffer, _length);
    Strings::TrimCore(tmp.Buffer(), tmp.Length(), s, len);
    return tmp;
}
INLINE String String::Trim() const {
    if (_length == 0)
        return String();
    String tmp(_buffer, _length);
    Strings::TrimCore(tmp.Buffer(), tmp.Length());
    return tmp;
}
INLINE String String::Trim(String const &s) const {
    if (_length == 0)
        return String();
    if (s.Length() == 0)
        return *this;
    return TrimCore(s.Buffer(), s.Length());
}
INLINE String String::Trim(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return *this;
    return TrimCore(s, len_);
}
INLINE String String::Trim(Char const * const &s) const {
    assert ( s );
    return Trim(s, Strings::GetLength(s));
}





INLINE UInt32 String::TrimSelf() {
    if (_length == 0)
        return 0;
    return Strings::TrimCore(_buffer, _length);
}
INLINE UInt32 String::TrimSelf(String const &s) {
    if (_length == 0 || s.Length() == 0)
        return 0;
    return Strings::TrimCore(_buffer, _length, s.Buffer(), s.Length());
}
INLINE UInt32 String::TrimSelf(Char const * const &s, UInt32 const &len) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return 0;
    return Strings::TrimCore(_buffer, _length, s, len_);
}
INLINE UInt32 String::TrimSelf(Char const * const &s) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len = Strings::GetLength(s);
    if (len == 0)
        return 0;
    return Strings::TrimCore(_buffer, _length, s, len);
}







INLINE String String::TrimStartCore(Char const * const &s, UInt32 const &len) const {
    String tmp(_buffer, _length);
    Strings::TrimStartCore(tmp.Buffer(), tmp.Length(), s, len);
    return tmp;
}
INLINE String String::TrimStart() const {
    if (_length == 0)
        return String();
    String tmp(_buffer, _length);
    Strings::TrimStartCore(tmp.Buffer(), tmp.Length());
    return tmp;
}
INLINE String String::TrimStart(String const &s) const {
    if (_length == 0)
        return String();
    if (s.Length() == 0)
        return *this;
    return TrimStartCore(s.Buffer(), s.Length());
}
INLINE String String::TrimStart(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return *this;
    return TrimStartCore(s, len_);
}
INLINE String String::TrimStart(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len = Strings::GetLength(s);
    if (len == 0)
        return *this;
    return TrimStartCore(s, len);
}







INLINE UInt32 String::TrimStartSelf() {
    if (_length == 0)
        return 0;
    return Strings::TrimStartCore(_buffer, _length);
}
INLINE UInt32 String::TrimStartSelf(String const &s) {
    if (_length == 0 || s.Length() == 0)
        return 0;
    return Strings::TrimStartCore(_buffer, _length, s.Buffer(), s.Length());
}
INLINE UInt32 String::TrimStartSelf(Char const * const &s, UInt32 const &len) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return 0;
    return Strings::TrimStartCore(_buffer, _length, s, len_);
}
INLINE UInt32 String::TrimStartSelf(Char const * const &s) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len = Strings::GetLength(s);
    if (len == 0)
        return 0;
    return Strings::TrimStartCore(_buffer, _length, s, len);
}







INLINE String String::TrimEndCore(Char const * const &s, UInt32 const &len) const {
    String tmp(_buffer, _length);
    Strings::TrimEndCore(tmp.Buffer(), tmp.Length(), s, len);
    return tmp;
}
INLINE String String::TrimEnd() const {
    if (_length == 0)
        return String();
    String tmp(_buffer, _length);
    Strings::TrimEndCore(tmp.Buffer(), tmp.Length());
    return tmp;
}
INLINE String String::TrimEnd(String const &s) const {
    if (_length == 0)
        return String();
    if (s.Length() == 0)
        return *this;
    return TrimEndCore(s.Buffer(), s.Length());
}
INLINE String String::TrimEnd(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return *this;
    return TrimEndCore(s, len_);
}
INLINE String String::TrimEnd(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len = Strings::GetLength(s);
    if (len == 0)
        return *this;
    return TrimEndCore(s, len);
}






INLINE UInt32 String::TrimEndSelf() {
    if (_length == 0)
        return 0;
    return Strings::TrimEndCore(_buffer, _length);
}
INLINE UInt32 String::TrimEndSelf(String const &s) {
    if (_length == 0 || s.Length() == 0)
        return 0;
    return Strings::TrimEndCore(_buffer, _length, s.Buffer(), s.Length());
}
INLINE UInt32 String::TrimEndSelf(Char const * const &s, UInt32 const &len) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return 0;
    return Strings::TrimEndCore(_buffer, _length, s, len_);
}
INLINE UInt32 String::TrimEndSelf(Char const * const &s) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len = Strings::GetLength(s);
    if (len == 0)
        return 0;
    return Strings::TrimEndCore(_buffer, _length, s, len);
}








INLINE String String::TrimAllCore(Char const * const &s, UInt32 const &len) const {
    String tmp(_buffer, _length);
    Strings::TrimAllCore(tmp.Buffer(), tmp.Length(), s, len);
    return tmp;
}
INLINE String String::TrimAll() const {
    static var space = L" ";
    if (_length == 0)
        return String();
    String tmp(_buffer, _length);
    Strings::TrimAllCore(tmp.Buffer(), tmp.Length(), space, 1);
    return tmp;
}
INLINE String String::TrimAll(String const &s) const {
    if (_length == 0)
        return String();
    if (s.Length() == 0)
        return *this;
    return TrimAllCore(s.Buffer(), s.Length());
}
INLINE String String::TrimAll(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return *this;
    return TrimAllCore(s, len_);
}
INLINE String String::TrimAll(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return String();
    var len = Strings::GetLength(s);
    if (len == 0)
        return *this;
    return TrimAllCore(s, len);
}





INLINE UInt32 String::TrimAllSelf() {
    static var space = L" ";
    if (_length == 0)
        return 0;
    return Strings::TrimAllCore(_buffer, _length, space, 1);
}
INLINE UInt32 String::TrimAllSelf(String const &s) {
    if (_length == 0 || s.Length() == 0)
        return 0;
    return Strings::TrimAllCore(_buffer, _length, s.Buffer(), s.Length());
}
INLINE UInt32 String::TrimAllSelf(Char const * const &s, UInt32 const &len) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return 0;
    return Strings::TrimAllCore(_buffer, _length, s, len_);
}
INLINE UInt32 String::TrimAllSelf(Char const * const &s) {
    assert ( s );
    if (_length == 0)
        return 0;
    var len = Strings::GetLength(s);
    if (len == 0)
        return 0;
    return Strings::TrimAllCore(_buffer, _length, s, len);
}






INLINE Boolean String::ContainsCore(Char const * const &s, UInt32 const &len) const {
    return Strings::FindCore(_buffer, _length, s, len) >= 0;
}
INLINE Boolean String::Contains(Char const &c) const {
    if (_length == 0)
        return false;
    return Strings::FindCore(_buffer, _length, c) >= 0;
}
INLINE Boolean String::Contains(String const &s) const {
    if (_length == 0)
        return false;
    if (s.Length() == 0)
        return false;
    return ContainsCore(s.Buffer(), s.Length());
}
INLINE Boolean String::Contains(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return false;
    return ContainsCore(s, len_);
}
INLINE Boolean String::Contains(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len = Strings::GetLength(s);
    if (len == 0)
        return false;
    return ContainsCore(s, len);
}






INLINE Int32 String::IndexOfCore(Char const * const &s, UInt32 const &len) const {
    return Strings::FindCore(_buffer, _length, s, len);
}
INLINE Int32 String::IndexOf(Char const &c) const {
    if (_length == 0)
        return -1;
    return Strings::FindCore(_buffer, _length, c);
}
INLINE Int32 String::IndexOf(String const &s) const {
    if (_length == 0)
        return -1;
    if (s.Length() == 0)
        return -1;
    return IndexOfCore(s.Buffer(), s.Length());
}
INLINE Int32 String::IndexOf(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return -1;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return -1;
    return IndexOfCore(s, len_);
}
INLINE Int32 String::IndexOf(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return -1;
    var len = Strings::GetLength(s);
    if (len == 0)
        return -1;
    return IndexOfCore(s, len);
}

INLINE Int32 String::IndexOfCore(UInt32 const &offset, Char const * const &s, UInt32 const &len) const {
    if (offset + len > _length)
        return -1;
    var n = Strings::FindCore(_buffer + offset, _length - offset, s, len);
    if (n >= 0)
        return offset + n;
    return -1;
}
INLINE Int32 String::IndexOf(UInt32 const &offset, Char const &c) const {
    if (offset + 1 > _length)
        return -1;
    var n = Strings::FindCore(_buffer + offset, _length - offset, c);
    if (n >= 0)
        return offset + n;
    return -1;
}
INLINE Int32 String::IndexOf(UInt32 const &offset, String const &s) const {
    return IndexOfCore(offset, s.Buffer(), s.Length());
}
INLINE Int32 String::IndexOf(UInt32 const &offset, Char const * const &s, UInt32 const &len) const {
    assert ( s );
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    return IndexOfCore(offset, s, len_);
}
INLINE Int32 String::IndexOf(UInt32 const &offset, Char const * const &s) const {
    assert ( s );
    var len = Strings::GetLength(s);
    return IndexOfCore(offset, s, len);
}






INLINE Boolean String::StartsWithCore(Char const * const &s, UInt32 const &len) const {
    return Strings::EqualsCore(_buffer, s, len);
}
INLINE Boolean String::StartsWith(Char const &c) const {
    if (_length == 0)
        return false;
    return _buffer[0] == c;
}
INLINE Boolean String::StartsWith(String const &s) const {
    if (_length == 0 || s.Length() == 0)
        return false;
    return Strings::EqualsCore(_buffer, s.Buffer(), s.Length());
}
INLINE Boolean String::StartsWith(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_ == 0)
        return false;
    return StartsWithCore(s, len_);
}
INLINE Boolean String::StartsWith(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len = Strings::GetLength(s);
    if (len == 0)
        return false;
    return StartsWithCore(s, len);
}






INLINE Boolean String::EndsWithCore(Char const * const &s, UInt32 const &len) const {
    return Strings::EqualsCore(_buffer + _length - len, s, len);
}
INLINE Boolean String::EndsWith(Char const &c) const {
    if (_length == 0)
        return false;
    return _buffer[_length - 1] == c;
}
INLINE Boolean String::EndsWith(String const &s) const {
    if (s.Length() == 0 || _length < s.Length())
        return false;
    return EndsWithCore(s.Buffer(), s.Length());
}
INLINE Boolean String::EndsWith(Char const * const &s, UInt32 const &len) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (_length < len_)
        return false;
    return EndsWithCore(s, len_);
}
INLINE Boolean String::EndsWith(Char const * const &s) const {
    assert ( s );
    if (_length == 0)
        return false;
    var len = Strings::GetLength(s);
    if (_length < len)
        return false;
    return EndsWithCore(s, len);
}






INLINE String String::Substring(UInt32 const &offset, UInt32 const &len) const {
    if (offset >= _length)
        return String();
    if (len == 0 || len + offset > _length)
        return String(_buffer + offset, _length - offset);
    return String(_buffer + offset, len);
}
INLINE String String::Substring(UInt32 const &offset) const {
    if (offset >= _length)
        return String();
    return String(_buffer + offset, _length - offset);
}






INLINE String & String::SubstringSelf(UInt32 const &offset, UInt32 const &len) {
    if (offset >= _length) {
        _length = 0;
        return *this;
    }
    if (len == 0 || len + offset > _length) {
        memmove(_buffer, _buffer + offset, (_length - offset) * sizeof(Char));
        _length -= offset;
        return *this;
    }
    else {
        memmove(_buffer, _buffer + offset, len * sizeof(Char));
        _length = len;
        return *this;
    }
}
INLINE String & String::SubstringSelf(UInt32 const &offset) {
    return SubstringSelf(offset, 0);
}






INLINE String & String::InsertCore(UInt32 const &offset, Char const * const &s, UInt32 const &len) {
    if (offset >= _length) {
        if (var num = offset - _length) {
            EnsureSpace(num + len);
            Strings::Fill(_buffer + _length, num, ' ');
            memcpy(_buffer + _length + num, s, len * sizeof(Char));
            _length += num + len;
        } else
            AppendCore(s, len);
    }
    else {
        if (len) {
            EnsureSpace(len);
            memmove(_buffer + offset + len, _buffer + offset, (_length - offset) * sizeof(Char));
            memcpy(_buffer + offset, s, len * sizeof(Char));
            _length += len;
        }
    }
    return *this;
}
INLINE String & String::Insert(UInt32 const &offset, Char const &c) {
    if (offset >= _length) {
        if (var len = offset - _length) {
            Strings::Fill(_buffer + _length, len, ' ');
            _length += len;
        }
        Append(c);
    }
    else {
        EnsureSpace(1);
        memmove(_buffer + offset + 1, _buffer + offset, (_length - offset) * sizeof(Char));
        _buffer[offset] = c;
        ++_length;
    }
    return *this;
}
INLINE String & String::Insert(UInt32 const &offset, String const &s) {
    if (s.Length())
        return InsertCore(offset, s.Buffer(), s.Length());
    return *this;
}
INLINE String & String::Insert(UInt32 const &offset, String &&s) {
    if (_length == 0)
        return Append((String &&)s);
    else
        return Insert(offset, s);
}

INLINE String & String::Insert(UInt32 const &offset, Char const * const &s, UInt32 const &len) {
    assert ( s );
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    return InsertCore(offset, s, len_);
}
INLINE String & String::Insert(UInt32 const &offset, Char const * const &s) {
    assert ( s );
    var len = Strings::GetLength(s);
    return InsertCore(offset, s, len);
}






INLINE String & String::AppendCore(Char const * const &s, UInt32 const &len) {
    EnsureSpace(len);
    memcpy(_buffer + _length, s, len * sizeof(Char));
    _length += len;
    return *this;
}
INLINE String & String::Append(Char const &c) {
    EnsureSpace(1);
    _buffer[_length++] = c;
    return *this;
}
INLINE String & String::Append(Char const &c, UInt32 const &num) {
    if (num) {
        EnsureSpace(num);
        Strings::Fill(_buffer + _length, num, c);
        _length += num;
    }
    return *this;
}
INLINE String & String::Append(String const &s) {
    if (s.Length())
        return AppendCore(s.Buffer(), s.Length());
    return *this;
}
INLINE String & String::Append(String &&s) {
    if (_length || s.BufferState() == BufferState_Custom)
        return Append(s);
    else {
        _buffer          = s.Buffer();
        _bufferLength    = s.BufferLength();
        _bufferState     = s.BufferState();
        _length          = s.Length();
        s.BufferState()   = BufferState_Custom;
        return *this;
    }
}
INLINE String & String::Append(Char const * const &s, UInt32 const &len) {
    assert ( s );
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_)
        return AppendCore(s, len_);
    return *this;
}
INLINE String & String::Append(Char const * const &s) {
    assert ( s );
    var len = Strings::GetLength(s);
    if (len)
        return AppendCore(s, len);
    return *this;
}

INLINE String & String::AppendCore(UInt32 const &fixLen, Char const * const &s, UInt32 const &len) {
    if (fixLen > len) {
        EnsureSpace(fixLen);
        Strings::Fill(_buffer + _length, fixLen - len, ' ');
        memcpy(_buffer + _length + fixLen - len, s, len * sizeof(Char));
        _length += fixLen;
    }
    else
        return AppendCore(s, len);
    return *this;
}
INLINE String & String::Append(UInt32 const &fixLen, Char const &c) {
    if (fixLen > 1) {
        EnsureSpace(fixLen);
        Strings::Fill(_buffer + _length, fixLen - 1, ' ');
        _length += fixLen - 1;
        _buffer[_length++] = c;
        return *this;
    }
    else
        return Append(c);
}
INLINE String & String::Append(UInt32 const &fixLen, Char const &c, UInt32 const &num) {
    if (fixLen > num) {
        EnsureSpace(fixLen);
        Strings::Fill(_buffer + _length, fixLen - num, ' ');
        Strings::Fill(_buffer + _length + fixLen - num, num, c);
        _length += fixLen;
        return *this;
    }
    else
        return Append(c, num);
}
INLINE String & String::Append(UInt32 const &fixLen, String const &s) {
    if (s.Length())
        return AppendCore(fixLen, s.Buffer(), s.Length());
    return *this;
}
INLINE String & String::Append(UInt32 const &fixLen, Char const * const &s, UInt32 const &len) {
    assert ( s );
    var len_ = len;
    if (len_ == 0)
        len_ = Strings::GetLength(s);
    if (len_)
        return AppendCore(fixLen, s, len_);
    return *this;
}
INLINE String & String::Append(UInt32 const &fixLen, Char const * const &s) {
    assert ( s );
    var len = Strings::GetLength(s);
    if (len)
        return AppendCore(fixLen, s, len);
    return *this;
}

INLINE String String::Remove(UInt32 const &offset) const {
    return Remove(offset, 0);
}

INLINE String & String::RemoveSelf(UInt32 const &offset, UInt32 const &len) {
    if (offset >= _length)
        return *this;
    if (len == 0 || len >= _length - offset) {
        _length = offset;
        return *this;
    }
    memmove(_buffer + offset, _buffer + offset + len, (_length - offset - len) * sizeof(Char));
    _length -= len;
    return *this;
}
INLINE String & String::RemoveSelf(UInt32 const &offset) {
    if (offset >= _length)
        return *this;
    _length = offset;
    return *this;
}

INLINE String String::Replace(Char const &c1, Char const &c2) const {
    if (c1 == c2)
        return *this;
    String s(_buffer, _length);
    for (UInt32 i = 0; i < _length; ++i)
        if (s.Buffer()[i] == c1)
            s.Buffer()[i] = c2;
    return s;
}
INLINE String String::Replace(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) const {
    var s1Len_ = s1Len;
    var s2Len_ = s2Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    if (s1Len_ == s2Len_ && Strings::EqualsCore(s1, s2, s2Len_))
        return *this;
    return ReplaceCore(s1, s1Len_, s2, s2Len_);
}
INLINE String String::Replace(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len) const {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    var s2Len_ = s2Len;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    if (s1Len == s2Len_ && Strings::EqualsCore(s1, s2, s2Len_))
        return *this;
    return ReplaceCore(s1, s1Len, s2, s2Len_);
}
INLINE String String::Replace(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2) const {
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    if (s1Len_ == s2Len && Strings::EqualsCore(s1, s2, s2Len))
        return *this;
    return ReplaceCore(s1, s1Len_, s2, s2Len);
}
INLINE String String::Replace(Char const * const &s1, Char const * const &s2) const {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    if (s1Len == s2Len && Strings::EqualsCore(s1, s2, s2Len))
        return *this;
    return ReplaceCore(s1, s1Len, s2, s2Len);
}
INLINE String String::Replace(String const &s1, String const &s2) const {
    if (s1.Length() == 0 || (s1.Length() == s2.Length() && Strings::EqualsCore(s1.Buffer(), s2.Buffer(), s1.Length())))
        return *this;
    return ReplaceCore(s1.Buffer(), s1.Length(), s2.Buffer(), s2.Length());
}
INLINE String String::Replace(String const &s1, Char const &c2) const {
    if (s1.Length() == 0 || (s1.Length() == 1 && s1.Buffer()[0] == c2))
        return *this;
    return ReplaceCore(s1.Buffer(), s1.Length(), &c2, 1);
}
INLINE String String::Replace(Char const &c1, String const &s2) const {
    return ReplaceCore(&c1, 1, s2.Buffer(), s2.Length());
}
INLINE String String::Replace(Char const &c1, Char const * const &s2, UInt32 const &s2Len) const {
    assert ( s2 );
    var s2Len_ = s2Len;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    return ReplaceCore(&c1, 1, s2, s2Len_);
}
INLINE String String::Replace(Char const * const &s1, UInt32 const &s1Len, Char const &c2) const {
    assert ( s1 );
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    return ReplaceCore(s1, s1Len_, &c2, 1);
}
INLINE String String::Replace(Char const * const &s1, Char const &c2) const {
    assert ( s1 );
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    return ReplaceCore(s1, s1Len, &c2, 1);
}
INLINE String String::Replace(Char const * const &s1, UInt32 const &s1Len, String const &s2) const {
    assert ( s1 );
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    return ReplaceCore(s1, s1Len_, s2.Buffer(), s2.Length());
}
INLINE String String::Replace(Char const * const &s1, String const &s2) const {
    assert ( s1 );
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    return ReplaceCore(s1, s1Len, s2.Buffer(), s2.Length());
}
INLINE String String::Replace(String const &s1, Char const * const &s2, UInt32 const &s2Len) const {
    assert ( s2 );
    var s2Len_ = s2Len;
    if (s1.Length() == 0)
        return *this;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    return ReplaceCore(s1.Buffer(), s1.Length(), s2, s2Len_);
}
INLINE String String::Replace(String const &s1, Char const * const &s2) const {
    assert ( s2 );
    if (s1.Length() == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    return ReplaceCore(s1.Buffer(), s1.Length(), s2, s2Len);
}
INLINE String String::Replace(Char const &c1, Char const * const &s2) const {
    assert ( s2 );
    var s2Len = Strings::GetLength(s2);
    if (s2Len == 0)
        return *this;
    return ReplaceCore(&c1, 1, s2, s2Len);
}


INLINE String & String::ReplaceSelf(Char const &c1, Char const &c2) {
    if (c1 == c2)
        return *this;
    for (UInt32 i = 0; i < _length; ++i)
        if (_buffer[i] == c1)
            _buffer[i] = c2;
    return *this;
}
INLINE String & String::ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) {
    var s1Len_ = s1Len;
    var s2Len_ = s2Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    if (s1Len_ == s2Len_ && Strings::EqualsCore(s1, s2, s2Len_))
        return *this;
    return ReplaceSelfCore(s1, s1Len_, s2, s2Len_);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, Char const * const &s2, UInt32 const &s2Len) {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    var s2Len_ = s2Len;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    if (s1Len == s2Len_ && Strings::EqualsCore(s1, s2, s2Len_))
        return *this;
    return ReplaceSelfCore(s1, s1Len, s2, s2Len_);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2) {
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    if (s1Len_ == s2Len && Strings::EqualsCore(s1, s2, s2Len))
        return *this;
    return ReplaceSelfCore(s1, s1Len_, s2, s2Len);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, Char const * const &s2) {
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    if (s1Len == s2Len && Strings::EqualsCore(s1, s2, s2Len))
        return *this;
    return ReplaceSelfCore(s1, s1Len, s2, s2Len);
}
INLINE String & String::ReplaceSelf(String const &s1, String const &s2) {
    if (s1.Length() == 0 || (s1.Length() == s2.Length() && Strings::EqualsCore(s1.Buffer(), s2.Buffer(), s1.Length())))
        return *this;
    return ReplaceSelfCore(s1.Buffer(), s1.Length(), s2.Buffer(), s2.Length());
}
INLINE String & String::ReplaceSelf(String const &s1, Char const &c2) {
    if (s1.Length() == 0 || (s1.Length() == 1 && s1.Buffer()[0] == c2))
        return *this;
    return ReplaceSelfCore(s1.Buffer(), s1.Length(), &c2, 1);
}
INLINE String & String::ReplaceSelf(Char const &c1, String const &s2) {
    return ReplaceSelfCore(&c1, 1, s2.Buffer(), s2.Length());
}
INLINE String & String::ReplaceSelf(Char const &c1, Char const * const &s2, UInt32 const &s2Len) {
    assert ( s2 );
    var s2Len_ = s2Len;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    return ReplaceSelfCore(&c1, 1, s2, s2Len_);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, Char const &c2) {
    assert ( s1 );
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    return ReplaceSelfCore(s1, s1Len_, &c2, 1);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, Char const &c2) {
    assert ( s1 );
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    return ReplaceSelfCore(s1, s1Len, &c2, 1);
}
INLINE String & String::ReplaceSelf(Char const * const &s1, UInt32 const &s1Len, String const &s2) {
    assert ( s1 );
    var s1Len_ = s1Len;
    if (s1Len_ == 0)
        s1Len_ = Strings::GetLength(s1);
    if (s1Len_ == 0)
        return *this;
    return ReplaceSelfCore(s1, s1Len_, s2.Buffer(), s2.Length());
}
INLINE String & String::ReplaceSelf(Char const * const &s1, String const &s2) {
    assert ( s1 );
    var s1Len = Strings::GetLength(s1);
    if (s1Len == 0)
        return *this;
    return ReplaceSelfCore(s1, s1Len, s2.Buffer(), s2.Length());
}
INLINE String & String::ReplaceSelf(String const &s1, Char const * const &s2, UInt32 const &s2Len) {
    assert ( s2 );
    if (s1.Length() == 0)
        return *this;
    var s2Len_ = s2Len;
    if (s2Len_ == 0)
        s2Len_ = Strings::GetLength(s2);
    return ReplaceSelfCore(s1.Buffer(), s1.Length(), s2, s2Len_);
}
INLINE String & String::ReplaceSelf(String const &s1, Char const * const &s2) {
    assert ( s2 );
    if (s1.Length() == 0)
        return *this;
    var s2Len = Strings::GetLength(s2);
    return ReplaceSelfCore(s1.Buffer(), s1.Length(), s2, s2Len);
}
INLINE String & String::ReplaceSelf(Char const &c1, Char const * const &s2) {
    assert ( s2 );
    var s2Len = Strings::GetLength(s2);
    if (s2Len == 0)
        return *this;
    return ReplaceSelfCore(&c1, 1, s2, s2Len);
}





INLINE String String::ToLower() const {
    String s(_buffer, _length);
    Strings::ToLower(s.Buffer(), _length);
    return s;
}
INLINE String String::ToUpper() const {
    String s(_buffer, _length);
    Strings::ToUpper(s.Buffer(), _length);
    return s;
}

INLINE String & String::ToLowerSelf() {
    Strings::ToLower(_buffer, _length);
    return *this;
}
INLINE String & String::ToUpperSelf() {
    Strings::ToUpper(_buffer, _length);
    return *this;
}













INLINE String & operator<<(String &o, String const &in) {
    o.Append(in);
    return o;
}

INLINE String & operator<<(String &o, Char const *in) {
    o.Append(in);
    return o;
}


INLINE String && operator<<(String &&o, String const &in) {
    o.Append(in);
    return (String &&)o;
}

INLINE String && operator<<(String &&o, Char const *in) {
    o.Append(in);
    return (String &&)o;
}

INLINE void String::operator+=(String const &s) {
    if (s.Length() == 0)
        return;
    AppendCore(s.Buffer(), s.Length());
}

INLINE void String::operator+=(Char const * const &s) {
    assert ( s );
    if (var len = (UInt32)Strings::GetLength(s))
        AppendCore(s, len);
}


INLINE Char & String::operator[](UInt32 const &idx) {
    return _buffer[idx];
}
INLINE Char const & String::operator[](UInt32 const &idx) const {
    return _buffer[idx];
}



INLINE String operator+(String const &s1, String const &s2) {
    if (s1.Length() == 0) {
        if (s2.Length() == 0)
            return s1;
        else
            return s2;
    }
    else {
        if (s2.Length() == 0)
            return s1;
        else {
            String s(s1.Length() + s2.Length());
            memcpy(s.Buffer(), s1.Buffer(), s1.Length() * sizeof(Char));
            memcpy(s.Buffer() + s1.Length(), s2.Buffer(), s2.Length() * sizeof(Char));
            s.Length() = s1.Length() + s2.Length();
            return s;
        }
    }
}

INLINE String operator+(String const &s1, Char const * const &s2) {
    assert ( s2 );
    if (s1.Length() == 0)
        return String(s2);
    else {
        if (var len = Strings::GetLength(s2)) {
            String s(s1.Length() + len);
            memcpy(s.Buffer(), s1.Buffer(), s1.Length() * sizeof(Char));
            memcpy(s.Buffer() + s1.Length(), s2, len * sizeof(Char));
            s.Length() = s1.Length() + len;
            return s;
        } else
            return s1;
    }
}

INLINE String operator+(Char const * const &s1, String const &s2) {
    assert ( s1 );
    if (var len = Strings::GetLength(s1)) {
        if (s2.Length() == 0)
            return String(s1);
        else {
            String s(len + s2.Length());
            memcpy(s.Buffer(), s1, len * sizeof(Char));
            memcpy(s.Buffer() + len, s2.Buffer(), s2.Length() * sizeof(Char));
            s.Length() = len + s2.Length();
            return s;
        }
    } else
        return s2;
}

INLINE String && operator+(String &&s1, String const &s2) {
    if (s2.Length())
        s1 += s2;
    return (String &&)s1;
}

INLINE String && operator+(String &&s1, Char const * const &s2) {
    assert ( s2 );
    if (var len = Strings::GetLength(s2))
        s1.AppendCore(s2, len);
    return (String &&)s1;
}


INLINE String && operator+(String const &s1, String &&s2) {
    if (s1.Length())
        s2.Insert(0, s1);
    return (String &&)s2;
}
INLINE String && operator+(Char const * const &s1, String &&s2) {
    if (s1)
        s2.Insert(0, s1);
    return (String &&)s2;
}
INLINE String && operator+(String &&s1, String &&s2) {
    if (s2.Length())
        s1.AppendCore(s2.Buffer(), s2.Length());
    return (String &&)s1;
}









INLINE Boolean operator==(String const &s1, String const &s2) {
    if (s1.Length() == s2.Length()) {
        if (s1.Length() == 0)
            return true;
        else
            return Strings::EqualsCore(s1.Buffer(), s2.Buffer(), s2.Length());
    }
    return false;
}

INLINE Boolean operator==(String const &s1, Char const * const &s2) {
    assert ( s2 );
    var s2Len = Strings::GetLength(s2);
    if (s1.Length() == s2Len) {
        if (s2Len == 0)
            return true;
        else
            return Strings::EqualsCore(s1.Buffer(), s2, s2Len);
    }
    return false;
}

INLINE Boolean operator==(Char const * const &s1, String const &s2) {
    return operator==(s2, s1);
}

INLINE Boolean operator!=(String const &s1, String const &s2) {
    return !operator==(s1, s2);
}

INLINE Boolean operator!=(String const &s1, Char const * const &s2) {
    return !operator==(s1, s2);
}

INLINE Boolean operator!=(Char const * const &s1, String const &s2) {
    return !operator==(s2, s1);
}









INLINE List<String> String::SplitCore(Char const * const &s, UInt32 const &sLen) {
    List<String> a;
    Int32 idxStart = 0, idxEnd;
Lab1:
    idxEnd = IndexOfCore(idxStart, s, sLen);
    if (idxEnd >= 0) {
        if (idxEnd - idxStart)
            a.Add(String(_buffer + idxStart, idxEnd - idxStart));
        idxStart = idxEnd + sLen;
        if (idxStart < (Int32)_length)
            goto Lab1;
    }
    else
        if (_length - idxStart)
            a.Add(String(_buffer + idxStart, _length - idxStart));
    return a;
}
INLINE List<String> String::Split(Char const &c) {
    List<String> a;
    Int32 idxStart = 0, idxEnd;
Lab1:
    idxEnd = IndexOf(idxStart, c);
    if (idxEnd >= 0) {
        if (idxEnd - idxStart)
            a.Add(String(_buffer + idxStart, idxEnd - idxStart));
        idxStart = idxEnd + 1;
        if (idxStart < (Int32)_length)
            goto Lab1;
    }
    else
        if (_length - idxStart)
            a.Add(String(_buffer + idxStart, _length - idxStart));
    return a;
}
INLINE List<String> String::Split(Char const * const &s, UInt32 const &sLen) {
    if (_length == 0)
        return List<String>();
    var sLen_ = sLen;
    if (sLen_ == 0)
        sLen_ = Strings::GetLength(s);
    if (sLen_)
        return SplitCore(s, sLen_);
    return List<String>();
}
INLINE List<String> String::Split(Char const * const &s) {
    if (_length == 0)
        return List<String>();
    var sLen = Strings::GetLength(s);
    if (sLen)
        return SplitCore(s, sLen);
    return List<String>();
}
INLINE List<String> String::Split(String const &s) {
    if (_length == 0)
        return List<String>();
    if (s.Length())
        return SplitCore(s.Buffer(), s.Length());
    return List<String>();
}







INLINE String String::Remove(UInt32 const &offset, UInt32 const &len) const {
    if (len) {
        if (offset < _length) {
            if (len >= _length - offset)
                return String(_buffer, offset);
            String s(_length - len);
            if (offset)
                s.AppendCore(_buffer, offset);
            if (_length - offset - len)
                s.AppendCore(_buffer + offset + len, _length - offset - len);
            return s;
        }
    }
    else {
        if (offset < _length) {
            if (offset)
                return String(_buffer, offset);
            else
                return String();
        }
    }
    return *this;
}


INLINE String String::ReplaceCore(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) const {
    if (s1Len >= s2Len) {
        // 从开始范围扫 s1. 发现就往 s 追加 跨过的串和 s2. 否则追加剩下的串
        String s(_length);
        UInt32 idxStart = 0, idx;
Lab1:
        idx = (UInt32)IndexOfCore(idxStart, s1, s1Len);
        if (idx != (UInt32)-1) {
            if (idx - idxStart)
                s.AppendCore(_buffer + idxStart, idx - idxStart);
            if (s2Len)
                s.AppendCore(s2, s2Len);
            idxStart = idx + s1Len;
            if (idxStart < _length)
                goto Lab1;
        }
        else
            if (_length - idxStart)
                s.AppendCore(_buffer + idxStart, _length - idxStart);
        return s;
    } else {
        // 先扫一下有几处 s1. 根据 s1 s2 长度差值, 计算出最终 buff 长
        UInt32 idxStart = 0, idx, len = 0;
Lab2:
        idx = (UInt32)IndexOfCore(idxStart, s1, s1Len);
        if (idx != (UInt32)-1) {
            len += idx - idxStart;
            len += s2Len;
            idxStart = idx + s1Len;
            if (idxStart < _length)
                goto Lab2;
        }
        else
            len += _length - idxStart;

        String s(len);
        idxStart = 0;
Lab3:
        idx = IndexOfCore(idxStart, s1, s1Len);
        if (idx != (UInt32)-1) {
            if (idx - idxStart)
                s.AppendCore(_buffer + idxStart, idx - idxStart);
            if (s2Len)
                s.AppendCore(s2, s2Len);
            idxStart = idx + s1Len;
            if (idxStart < _length)
                goto Lab3;
        }
        else
            if (_length - idxStart)
                s.AppendCore(_buffer + idxStart, _length - idxStart);
        return s;
    }
}


INLINE String & String::ReplaceSelfCore(Char const * const &s1, UInt32 const &s1Len, Char const * const &s2, UInt32 const &s2Len) {
    if (s1Len >= s2Len) {
        // 从开始范围扫 s1. 发现就 移动 + copy s2
        UInt32 idxStart = 0, idx, len = 0;
Lab1:
        idx = (UInt32)IndexOfCore(idxStart, s1, s1Len);
        if (idx != (UInt32)-1) {
            if (idx - idxStart) {
                memmove(_buffer + len, _buffer + idxStart, (idx - idxStart) * sizeof(Char));
                len += idx - idxStart;
            }
            if (s2Len) {
                memcpy(_buffer + len, s2, s2Len * sizeof(Char));
                len += s2Len;
            }
            idxStart = idx + s1Len;
            if (idxStart < _length)
                goto Lab1;
        }
        else
            if (_length - idxStart) {
                memmove(_buffer + len, _buffer + idxStart, (_length - idxStart) * sizeof(Char));
                len += _length - idxStart;
            }
        _length = len;
        return *this;
    } else {
        // 扫完, 记录索引点, 然后分段复制
        // 从开始范围扫 s1. 发现就记录
        // List<UInt32> idxs(Length / s1Len + 1);      // 计算最多会出现多少个 idx 应该不能大于 4g / sizeof(Char) /s2Len
        List<UInt32> idxs(4096);
        idxs.Length() = 0;
        UInt32 idxStart = 0, idx, len = 0;
Lab2:
        idx = (UInt32)IndexOfCore(idxStart, s1, s1Len);
        if (idx != (UInt32)-1) {
            idxs.Add(idx);
            idxStart = idx + s1Len;
            if (idxStart < _length)
                goto Lab2;
        }
        if (idxs.Length()) {
            len = _length - s1Len * idxs.Length() + s2Len * idxs.Length();
            var len_bak = len;
            EnsureSpace(len - _length);
            for (Int32 i = idxs.Length() - 1; i >= 0; --i) {
                idxStart = idxs[i];
                idx = idxStart + s1Len;
                if (_length > idx)
                    if (var leftLen = _length - idx) {       // 有剩的字符要移
                        memmove(_buffer + len - leftLen, _buffer + idx, leftLen * sizeof(Char));
                        len -= leftLen;
                        _length -= leftLen;
                    }
                memcpy(_buffer + len - s2Len, s2, s2Len * sizeof(Char));
                len -= s2Len;
                _length -= s1Len;
            }
            _length = len_bak;
        }
        return *this;
    }
}



INLINE String & String::operator=(Char const * const &s) {
    var len = Strings::GetLength(s);
    _length = len;
    switch (_bufferState) {
    case BufferState_Original:
        if (len > WString_OriginalBufferLength) {
            _buffer = new Char[len];
            _bufferLength = len;
            _bufferState = BufferState_New;
            memcpy(_buffer, s, len * sizeof(Char));
        }
        else
            memcpy(_originalBuffer, s, len * sizeof(Char));
        break;
    case BufferState_New:
        if (len > _bufferLength) {
            delete [] _buffer;
            _buffer = new Char[len];
            _bufferLength = len;
            _bufferState = BufferState_New;
            memcpy(_buffer, s, len * sizeof(Char));
        }
        else
            memcpy(_buffer, s, len * sizeof(Char));
        break;
    case BufferState_Custom:
        if (len > WString_OriginalBufferLength) {
            _buffer = new Char[len];
            _bufferLength = len;
            _bufferState = BufferState_New;
            memcpy(_buffer, s, len * sizeof(Char));
        }
        else {
            memcpy(_originalBuffer, s, len * sizeof(Char));
            _buffer = _originalBuffer;
            _bufferState = BufferState_Original;
            _bufferLength = WString_OriginalBufferLength;
        }
        break;
    }
    return *this;
}

INLINE String & String::operator=(String const &s) {
    _length = s.Length();
    switch (_bufferState) {
    case BufferState_Original:
        if (s.Length() > WString_OriginalBufferLength) {
            _buffer = new Char[s.Length()];
            _bufferLength = s.Length();
            _bufferState = BufferState_New;
            memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
        } else
            memcpy(_originalBuffer, s.Buffer(), s.Length() * sizeof(Char));
        break;
    case BufferState_New:
        if (s.Length() > _bufferLength) {
            delete [] _buffer;
            _buffer = new Char[s.Length()];
            _bufferLength = s.Length();
        }
        memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
        break;
    case BufferState_Custom:
        if (s.Length() > WString_OriginalBufferLength) {
            _buffer = new Char[s.Length()];
            _bufferState = BufferState_New;
            memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
        } else {
            _bufferLength = WString_OriginalBufferLength;
            _buffer = _originalBuffer;
            _bufferState = BufferState_Original;
            memcpy(_originalBuffer, s.Buffer(), s.Length() * sizeof(Char));
        }
        break;
    }
    return *this;
}

INLINE String & String::operator=(String &&s) {
    _length = s.Length();
    switch (s.BufferState()) {
    case BufferState_New:                       // 这段是和上面的代码不同的处理
        s.BufferState() = BufferState_Custom;     // 不让 s 删 Buffer
        _buffer = s.Buffer();
        _bufferLength = s.BufferLength();
        _bufferState = BufferState_New;
        break;
    case BufferState_Original:
        if (s.Length() > WString_OriginalBufferLength) {
            _buffer = new Char[s.Length()];
            _bufferLength = s.Length();
            _bufferState = BufferState_New;
            memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
        } else
            memcpy(_originalBuffer, s.Buffer(), s.Length() * sizeof(Char));
        break;
    case BufferState_Custom:
        if (s.Length() > WString_OriginalBufferLength) {
            _buffer = new Char[s.Length()];
            _bufferState = BufferState_New;
            memcpy(_buffer, s.Buffer(), s.Length() * sizeof(Char));
        } else {
            _bufferLength = WString_OriginalBufferLength;
            _buffer = _originalBuffer;
            _bufferState = BufferState_Original;
            memcpy(_originalBuffer, s.Buffer(), s.Length() * sizeof(Char));
        }
        break;
    }
    return *this;
}












// 下面是和 SString/SByte 的互日区
//


INLINE String::String(SByte const * const &s, UInt32 const &len) {
    assert ( s );
    if (len == 0)
        Init();
    else {
        Init(len);
        _length = Converts::Convert_Unicode(s, len, _buffer);
    }
}
INLINE String::String(SByte const * const &s) {
    assert ( s );
    if (var len = Strings::GetLength(s)) {
        Init(len);
        _length = Converts::Convert_Unicode(s, len, _buffer);
    }
    else
        Init();
}
INLINE String::String(SString const &s) {
    var len = s.Length();
    if (len == 0)
        Init();
    else {
        Init(len);
        _length = Converts::Convert_Unicode(s.Buffer(), len, _buffer);
    }
}
INLINE String &String::Append(SByte const &c) {
    return Append((Char)c);
}
INLINE String &String::Append(SString const &s) {
    return Append(s.Buffer(), s.Length());
}
INLINE String &String::Append(SByte const * const &s, UInt32 const &len) {
    assert ( s );
    if (len == 0)
        return *this;
    EnsureSpace(len);
    _length += Converts::Convert_Unicode(s, len, _buffer + _length);
    return *this;
}
INLINE String &String::Append(SByte const * const &s) {
    assert ( s );
    var len = Strings::GetLength(s);
    return Append(s, len);
}
INLINE String &String::Insert(UInt32 const &offset, SByte const &c) {
    return Insert(offset, (Char)c);
}
INLINE String &String::Insert(UInt32 const &offset, SString const &s) {
    return Insert(offset, String(s));
}
INLINE String &String::Insert(UInt32 const &offset, SByte const * const &s, UInt32 const &len) {
    return Insert(offset, String(s, len));
}
INLINE String &String::Insert(UInt32 const &offset, SByte const * const &s) {
    return Insert(offset, String(s));
}
INLINE String &String::operator=(SByte const * const &s) {
    operator=(String(s));
    return *this;
}
INLINE String &String::operator=(SString const &s) {
    operator=(String(s));
    return *this;
}
INLINE void String::operator+=(SString const &s) {
    Append(s);
}
INLINE void String::operator+=(SByte const * const &s) {
    Append(s);
}
INLINE String & operator<<(String &o, SString const &in) {
    o.Append(in);
    return o;
}
INLINE String & operator<<(String &o, SByte const * const &in) {
    o.Append(in);
    return o;
}
INLINE String && operator<<(String &&o, SString const &in) {
    o.Append(in);
    return (String &&)o;
}
INLINE String && operator<<(String &&o, SByte const * const &in) {
    o.Append(in);
    return (String &&)o;
}
INLINE String operator+(String const &s1, SString const &s2) {
    return operator+(s1, String(s2));
}
INLINE String operator+(String const &s1, SByte const * const &s2) {
    return operator+(s1, String(s2));
}
INLINE String operator+(SByte const * const &s1, String const &s2) {
    return operator+(String(s1), s2);
}
INLINE String && operator+(SByte const * const &s1, String &&s2) {
    return (String &&)operator+(String(s1), (String &&)s2);
}
INLINE String && operator+(String &&s1, SString const &s2) {
    return (String &&)operator+((String &&)s1, String(s2));
}
INLINE String && operator+(String &&s1, SByte const * const &s2) {
    return (String &&)operator+((String &&)s1, String(s2));
}
INLINE Boolean operator==(String const &s1, SString const &s2) {
    return operator==(s1, String(s2));
}
INLINE Boolean operator==(String const &s1, SByte const * const &s2) {
    return operator==(s1, String(s2));
}
INLINE Boolean operator==(SByte const * const &s1, String const &s2) {
    return operator==(String(s1), s2);
}
INLINE Boolean operator!=(String const &s1, SString const &s2) {
    return !operator==(s1, s2);
}
INLINE Boolean operator!=(String const &s1, SByte const * const &s2) {
    return !operator==(s1, s2);
}
INLINE Boolean operator!=(SByte const * const &s1, String const &s2) {
    return !operator==(s1, s2);
}



//
// 上面是和 String/Char 的互日区













}
#endif
