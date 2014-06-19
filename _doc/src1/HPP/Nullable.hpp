#ifndef NULLABLE_HPP
#define NULLABLE_HPP
namespace Lib3 {




template<typename T>
INLINE Nullable<T>::Nullable() {
    NullableBase<T>::_hasValue = false;
}
template<typename T>
INLINE Nullable<T>::Nullable(T const &v) {
    new (NullableBase<T>::_buffer) T(v);
    NullableBase<T>::_hasValue = true;
}
template<typename T>
INLINE Nullable<T>::Nullable(T &&v) {
    new (NullableBase<T>::_buffer) T((T&&)v);
    NullableBase<T>::_hasValue = true;
}


template<typename T>
INLINE Nullable<T>::Nullable(NullType const &n) {
    NullableBase<T>::_hasValue = false;
}
template<typename T>
INLINE Nullable<T>::Nullable(Nullable<T> const &v) {
    if (v.NullableBase<T>::_hasValue) {
        NullableBase<T>::_hasValue = true;
        new (NullableBase<T>::_buffer) T(*(T*)v.NullableBase<T>::_buffer);
    }
    else
        NullableBase<T>::_hasValue = false;
}
template<typename T>
INLINE Nullable<T>::Nullable(Nullable<T> &&v) {
    if (v.NullableBase<T>::_hasValue) {
        NullableBase<T>::_hasValue = true;
        new (NullableBase<T>::_buffer) T((T&&)v.Value());
    }
    else
        NullableBase<T>::_hasValue = false;
}


template<typename T>
INLINE Nullable<T> & Nullable<T>::operator=(T const &v) {
    if (NullableBase<T>::_hasValue)
        Value().~T();
	new (NullableBase<T>::_buffer) T(v);
	NullableBase<T>::_hasValue = true;
	return *this;
}
template<typename T>
INLINE Nullable<T> & Nullable<T>::operator=(T &&v) {
    if (NullableBase<T>::_hasValue)
        Value().~T();
	new (NullableBase<T>::_buffer) T((T&&)v);
	NullableBase<T>::_hasValue = true;
	return *this;
}

template<typename T>
INLINE Nullable<T> & Nullable<T>::operator=(Nullable<T> const &v) {
    if (NullableBase<T>::_hasValue)
        Value().~T();
    if (v.NullableBase<T>::_hasValue) {
        NullableBase<T>::_hasValue = true;
        new (NullableBase<T>::_buffer) T(v.Value());
    }
    else
        NullableBase<T>::_hasValue = false;
	return *this;
}
template<typename T>
INLINE Nullable<T> & Nullable<T>::operator=(Nullable<T> &&v) {
    if (NullableBase<T>::_hasValue)
        Value().~T();
    if (v.NullableBase<T>::_hasValue) {
        NullableBase<T>::_hasValue = true;
        new (NullableBase<T>::_buffer) T((T&&)v.Value());
    }
    else
        NullableBase<T>::_hasValue = false;
	return *this;
}
template<typename T>
INLINE Nullable<T> & Nullable<T>::operator=(NullType const &v) {
    if (NullableBase<T>::_hasValue)
        Value().~T();
	NullableBase<T>::_hasValue = false;
	return *this;
}


template<typename T> INLINE Boolean Nullable<T>::operator==(T const &v) const { if (NullableBase<T>::_hasValue) return Value() == v; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator!=(T const &v) const { if (NullableBase<T>::_hasValue) return Value() != v; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator< (T const &v) const { if (NullableBase<T>::_hasValue) return Value() <  v; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator<=(T const &v) const { if (NullableBase<T>::_hasValue) return Value() <= v; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator> (T const &v) const { if (NullableBase<T>::_hasValue) return Value() >  v; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator>=(T const &v) const { if (NullableBase<T>::_hasValue) return Value() >= v; return false; }


template<typename T> INLINE Boolean Nullable<T>::operator==(Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() == v.Value; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator!=(Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() != v.Value; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator< (Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() <  v.Value; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator<=(Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() <= v.Value; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator> (Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() >  v.Value; return false; }
template<typename T> INLINE Boolean Nullable<T>::operator>=(Nullable<T> const &v) const { if (NullableBase<T>::_hasValue && v.NullableBase<T>::_hasValue) return Value() >= v.Value; return false; }

template<typename T> INLINE Boolean Nullable<T>::operator==(NullType const &v) const {
    return !NullableBase<T>::_hasValue;
}

template<typename T>
INLINE Boolean & Nullable<T>::HasValue() {
    return NullableBase<T>::_hasValue;
}
template<typename T>
INLINE T & Nullable<T>::Value() {
    assert ( NullableBase<T>::_hasValue );
    return *(T*)NullableBase<T>::_buffer;
}
template<typename T>
INLINE Boolean const & Nullable<T>::HasValue() const {
    return NullableBase<T>::_hasValue;
}
template<typename T>
INLINE T const & Nullable<T>::Value() const {
    assert ( NullableBase<T>::_hasValue );
    return *(T*)NullableBase<T>::_buffer;
}
template<typename T>
INLINE Nullable<T>::operator T() const {
    assert ( NullableBase<T>::_hasValue );
    return *(T*)NullableBase<T>::_buffer;
}




}   // namespace
#endif
