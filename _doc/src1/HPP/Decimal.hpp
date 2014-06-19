#ifndef DECIMAL_HPP
#define DECIMAL_HPP
namespace Lib3 {



template<typename T>
T Decimal::ShiftRight(UInt32 const &n) {
         if (n < 10) return T(n) / T(10);
    else if (n < 100) return T(n) / T(100);
    else if (n < 1000) return T(n) / T(1000);
    else if (n < 10000) return T(n) / T(10000);
    else if (n < 100000) return T(n) / T(100000);
    else if (n < 1000000) return T(n) / T(1000000);
    else if (n < 10000000) return T(n) / T(10000000);
    else if (n < 100000000) return T(n) / T(100000000);
    else if (n < 1000000000) return T(n) / T(1000000000);
    else return T(n) / T(10000000000);
}


template<UInt32 len>
INLINE Decimal::Decimal(SByte const (&s)[len]) {
    Converts::Convert(s, *this);
}

template<UInt32 len>
INLINE void Decimal::Set(SByte const (&s)[len]) {
    Converts::Convert(s, *this);
}

template<UInt32 len>
INLINE Decimal & Decimal::operator=(SByte const (&s)[len]) {
    Converts::Convert(s, *this);
    return *this;
}

INLINE Decimal::Decimal() {
}
INLINE Decimal::Decimal(Int32 const &i, UInt32 const &digi) {
    Set(i, digi);
}
INLINE Decimal::Decimal(Int32 const &v) {
    Set(v);
}
INLINE Decimal::Decimal(Single const &v) {
    Set(v);
}
INLINE Decimal::Decimal(Double const &v) {
    Set(v);
}
INLINE Decimal::Decimal(SByte const * const &s, UInt32 const &len) {
    Set(s, len);
}
INLINE Decimal::Decimal(Byte const * const &buf) {
    Set(buf);
}
INLINE void Decimal::Set(Int32 const &i, UInt32 const &digi) {
    _high = i;
    _low = digi;
}
INLINE void Decimal::Set(Int32 const &v) {
    _high = v;
    _low = 0;
}
INLINE void Decimal::Set(Single const &v) {
    var buf = Strings::ToString(v);
    Converts::Convert(buf, *this);
}
INLINE void Decimal::Set(Double const &v) {
    var buf = Strings::ToString(v);
    Converts::Convert(buf, *this);
}
INLINE void Decimal::Set(SByte const * const &s, UInt32 const &len) {
    ALIGN8 ( SByte buf[32] );
    memcpy(buf, s, len);
    buf[len] = '\0';
    Converts::Convert(buf, *this);
}
INLINE void Decimal::Set(Byte const * const &buf) {
    memcpy(&_value, buf, 8);
}
INLINE Decimal & Decimal::operator=(Int32 const &v) {
    Set(v);
    return *this;
}                  
INLINE Decimal & Decimal::operator=(Single const &v) {
    Set(v);
    return *this;
}
INLINE Decimal & Decimal::operator=(Double const &v) {
    Set(v);
    return *this;
}
INLINE Decimal & Decimal::operator=(Byte const * const &buf) {
    Set(buf);
    return *this;
}
INLINE Int32 & Decimal::Int() {
    return _high;
}                      
INLINE UInt32 & Decimal::Digi() {
    return _low;
}                     
INLINE UInt64 & Decimal::Value() {
    return _value;
}                     
INLINE Int32 const & Decimal::Int() const {
    return _high;
}                      
INLINE UInt32 const & Decimal::Digi() const {
    return _low;
}                     
INLINE UInt64 const & Decimal::Value() const {
    return _value;
}                     
INLINE Boolean Decimal::operator==(Decimal const &v) const {
    return _value == v._value;
}
INLINE Boolean Decimal::operator!=(Decimal const &v) const {
    return _value != v._value;
}
INLINE Boolean Decimal::operator< (Decimal const &v) const {
    return Compare(v) < 0;
}
INLINE Boolean Decimal::operator<=(Decimal const &v) const {
    return Compare(v) <= 0;
}
INLINE Boolean Decimal::operator> (Decimal const &v) const {
    return Compare(v) > 0;
}
INLINE Boolean Decimal::operator>=(Decimal const &v) const {
    return Compare(v) >= 0;
}
INLINE Decimal::operator Int32() const {
    return _high;
}






INLINE Int32 Decimal::Compare(Decimal const &v) const {
    if (_high != v._high)                               // 先对比整数部分
        return _high < v._high ? -1 : 1;
    else {                                              // 再对比小数中一方含 0 的部分
        if (_low == v._low)
            return 0;
        else {
            if (_low == 0) {
                if (_high < 0)
                    return v._low > 0 ? 1: -1;
            }
            else if (v._low == 0) {
                if (_high < 0)
                    return _low > 0 ? -1: 1;
            }
            else {                                      // 最后按１０进制左对齐后比较大小
                var v1 = _low, v2 = v._low;
                ShiftLeft(v1);
                ShiftLeft(v2);
                if (_high < 0)
                    return v1 > v2 ? -1 : 1;
                else
                    return v1 < v2 ? -1 : 1;
            }
        }

    }
	return 0;

}
INLINE void Decimal::ShiftLeft(UInt32 &v) {
         if (v < 10) v *= 1000000000;
    else if (v < 100) v *= 100000000;
    else if (v < 1000) v *= 10000000;
    else if (v < 10000) v *= 1000000;
    else if (v < 100000) v *= 100000;
    else if (v < 1000000) v *= 10000;
    else if (v < 10000000) v *= 1000;
    else if (v < 100000000) v *= 100;
    else if (v < 1000000000) v *= 10;
}
INLINE Decimal::operator Single() const {
    Single f = (Single)std::abs(_high);
    f += ShiftRight<Single>(_low);
    if (_high < 0)
        f = -f;
    return f;
}
INLINE Decimal::operator Double() const {
    Double f = (Double)std::abs(_high);
    f += ShiftRight<Double>(_low);
    if (_high < 0)
        f = -f;
    return f;
}





}
#endif