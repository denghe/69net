#ifndef LIST_HPP
#define LIST_HPP
namespace Lib3 {



template<> INLINE Byte              * List<Byte             >::Buffer() { return (Byte             *)_buffer; }
template<> INLINE UInt16            * List<UInt16           >::Buffer() { return (UInt16           *)_buffer; }
template<> INLINE UInt32            * List<UInt32           >::Buffer() { return (UInt32           *)_buffer; }
template<> INLINE SByte             * List<SByte            >::Buffer() { return (SByte            *)_buffer; }
template<> INLINE Int16             * List<Int16            >::Buffer() { return (Int16            *)_buffer; }
template<> INLINE Int32             * List<Int32            >::Buffer() { return (Int32            *)_buffer; }
template<> INLINE Single            * List<Single           >::Buffer() { return (Single           *)_buffer; }
template<> INLINE Char              * List<Char             >::Buffer() { return (Char             *)_buffer; }
template<> INLINE Boolean           * List<Boolean          >::Buffer() { return (Boolean          *)_buffer; }
template<> INLINE Nullable<Byte   > * List<Nullable<Byte   >>::Buffer() { return (Nullable<Byte   >*)_buffer; }
template<> INLINE Nullable<UInt16 > * List<Nullable<UInt16 >>::Buffer() { return (Nullable<UInt16 >*)_buffer; }
template<> INLINE Nullable<SByte  > * List<Nullable<SByte  >>::Buffer() { return (Nullable<SByte  >*)_buffer; }
template<> INLINE Nullable<Int16  > * List<Nullable<Int16  >>::Buffer() { return (Nullable<Int16  >*)_buffer; }
template<> INLINE Nullable<Char   > * List<Nullable<Char   >>::Buffer() { return (Nullable<Char   >*)_buffer; }
template<> INLINE Nullable<Boolean> * List<Nullable<Boolean>>::Buffer() { return (Nullable<Boolean>*)_buffer; }
#ifdef __X64
template<> INLINE UInt64            * List<UInt64           >::Buffer() { return (UInt64           *)_buffer; }
template<> INLINE Int64             * List<Int64            >::Buffer() { return (Int64            *)_buffer; }
template<> INLINE Double            * List<Double           >::Buffer() { return (Double           *)_buffer; }
template<> INLINE Decimal           * List<Decimal          >::Buffer() { return (Decimal          *)_buffer; }
template<> INLINE Nullable<UInt32 > * List<Nullable<UInt32 >>::Buffer() { return (Nullable<UInt32 >*)_buffer; }
template<> INLINE Nullable<Int32  > * List<Nullable<Int32  >>::Buffer() { return (Nullable<Int32  >*)_buffer; }
template<> INLINE Nullable<Single > * List<Nullable<Single >>::Buffer() { return (Nullable<Single >*)_buffer; }
#endif



template<> INLINE Byte              * const List<Byte             >::Buffer() const { return (Byte             *)_buffer; }
template<> INLINE UInt16            * const List<UInt16           >::Buffer() const { return (UInt16           *)_buffer; }
template<> INLINE UInt32            * const List<UInt32           >::Buffer() const { return (UInt32           *)_buffer; }
template<> INLINE SByte             * const List<SByte            >::Buffer() const { return (SByte            *)_buffer; }
template<> INLINE Int16             * const List<Int16            >::Buffer() const { return (Int16            *)_buffer; }
template<> INLINE Int32             * const List<Int32            >::Buffer() const { return (Int32            *)_buffer; }
template<> INLINE Single            * const List<Single           >::Buffer() const { return (Single           *)_buffer; }
template<> INLINE Char              * const List<Char             >::Buffer() const { return (Char             *)_buffer; }
template<> INLINE Boolean           * const List<Boolean          >::Buffer() const { return (Boolean          *)_buffer; }
template<> INLINE Nullable<Byte   > * const List<Nullable<Byte   >>::Buffer() const { return (Nullable<Byte   >*)_buffer; }
template<> INLINE Nullable<UInt16 > * const List<Nullable<UInt16 >>::Buffer() const { return (Nullable<UInt16 >*)_buffer; }
template<> INLINE Nullable<SByte  > * const List<Nullable<SByte  >>::Buffer() const { return (Nullable<SByte  >*)_buffer; }
template<> INLINE Nullable<Int16  > * const List<Nullable<Int16  >>::Buffer() const { return (Nullable<Int16  >*)_buffer; }
template<> INLINE Nullable<Char   > * const List<Nullable<Char   >>::Buffer() const { return (Nullable<Char   >*)_buffer; }
template<> INLINE Nullable<Boolean> * const List<Nullable<Boolean>>::Buffer() const { return (Nullable<Boolean>*)_buffer; }
#ifdef __X64
template<> INLINE UInt64            * const List<UInt64           >::Buffer() const { return (UInt64           *)_buffer; }
template<> INLINE Int64             * const List<Int64            >::Buffer() const { return (Int64            *)_buffer; }
template<> INLINE Double            * const List<Double           >::Buffer() const { return (Double           *)_buffer; }
template<> INLINE Decimal           * const List<Decimal          >::Buffer() const { return (Decimal          *)_buffer; }
template<> INLINE Nullable<UInt32 > * const List<Nullable<UInt32 >>::Buffer() const { return (Nullable<UInt32 >*)_buffer; }
template<> INLINE Nullable<Int32  > * const List<Nullable<Int32  >>::Buffer() const { return (Nullable<Int32  >*)_buffer; }
template<> INLINE Nullable<Single > * const List<Nullable<Single >>::Buffer() const { return (Nullable<Single >*)_buffer; }
#endif


template<> INLINE void List<Byte             >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<UInt16           >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<UInt32           >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<SByte            >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Int16            >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Int32            >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Single           >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Char             >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Boolean          >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Byte   >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<UInt16 >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<SByte  >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Int16  >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Char   >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Boolean>>::Init(UInt32 const &len) { InitArray(len); }
#ifdef __X64
template<> INLINE void List<UInt64           >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Int64            >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Double           >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Decimal          >::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<UInt32 >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Int32  >>::Init(UInt32 const &len) { InitArray(len); }
template<> INLINE void List<Nullable<Single >>::Init(UInt32 const &len) { InitArray(len); }
#endif


template<> INLINE List<Byte             >::List(List<Byte               > const &o) { ConsFromArray(o); }
template<> INLINE List<UInt16           >::List(List<UInt16             > const &o) { ConsFromArray(o); }
template<> INLINE List<UInt32           >::List(List<UInt32             > const &o) { ConsFromArray(o); }
template<> INLINE List<SByte            >::List(List<SByte              > const &o) { ConsFromArray(o); }
template<> INLINE List<Int16            >::List(List<Int16              > const &o) { ConsFromArray(o); }
template<> INLINE List<Int32            >::List(List<Int32              > const &o) { ConsFromArray(o); }
template<> INLINE List<Single           >::List(List<Single             > const &o) { ConsFromArray(o); }
template<> INLINE List<Char             >::List(List<Char               > const &o) { ConsFromArray(o); }
template<> INLINE List<Boolean          >::List(List<Boolean            > const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Byte   >>::List(List<Nullable<Byte     >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<UInt16 >>::List(List<Nullable<UInt16   >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<SByte  >>::List(List<Nullable<SByte    >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Int16  >>::List(List<Nullable<Int16    >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Char   >>::List(List<Nullable<Char     >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Boolean>>::List(List<Nullable<Boolean  >> const &o) { ConsFromArray(o); }
#ifdef __X64
template<> INLINE List<UInt64           >::List(List<UInt64             > const &o) { ConsFromArray(o); }
template<> INLINE List<Int64            >::List(List<Int64              > const &o) { ConsFromArray(o); }
template<> INLINE List<Double           >::List(List<Double             > const &o) { ConsFromArray(o); }
template<> INLINE List<Decimal          >::List(List<Decimal            > const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<UInt32 >>::List(List<Nullable<UInt32   >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Int32  >>::List(List<Nullable<Int32    >> const &o) { ConsFromArray(o); }
template<> INLINE List<Nullable<Single >>::List(List<Nullable<Single   >> const &o) { ConsFromArray(o); }
#endif


template<> template<UInt32 len> INLINE List<Byte             >::List(Byte              const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<UInt16           >::List(UInt16            const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<UInt32           >::List(UInt32            const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<SByte            >::List(SByte             const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Int16            >::List(Int16             const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Int32            >::List(Int32             const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Single           >::List(Single            const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Char             >::List(Char              const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Boolean          >::List(Boolean           const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Byte   >>::List(Nullable<Byte   > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<UInt16 >>::List(Nullable<UInt16 > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<SByte  >>::List(Nullable<SByte  > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Int16  >>::List(Nullable<Int16  > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Char   >>::List(Nullable<Char   > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Boolean>>::List(Nullable<Boolean> const (&arr)[len]) { ConsFromFixedArray(arr); }
#ifdef __X64
template<> template<UInt32 len> INLINE List<UInt64           >::List(UInt64            const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Int64            >::List(Int64             const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Double           >::List(Double            const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Decimal          >::List(Decimal           const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<UInt32 >>::List(Nullable<UInt32 > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Int32  >>::List(Nullable<Int32  > const (&arr)[len]) { ConsFromFixedArray(arr); }
template<> template<UInt32 len> INLINE List<Nullable<Single >>::List(Nullable<Single > const (&arr)[len]) { ConsFromFixedArray(arr); }
#endif


template<> INLINE List<Byte             >::~List() { FreeArray(); }
template<> INLINE List<UInt16           >::~List() { FreeArray(); }
template<> INLINE List<UInt32           >::~List() { FreeArray(); }
template<> INLINE List<SByte            >::~List() { FreeArray(); }
template<> INLINE List<Int16            >::~List() { FreeArray(); }
template<> INLINE List<Int32            >::~List() { FreeArray(); }
template<> INLINE List<Single           >::~List() { FreeArray(); }
template<> INLINE List<Char             >::~List() { FreeArray(); }
template<> INLINE List<Boolean          >::~List() { FreeArray(); }
template<> INLINE List<Nullable<Byte   >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<UInt16 >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<SByte  >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<Int16  >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<Char   >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<Boolean>>::~List() { FreeArray(); }
#ifdef __X64
template<> INLINE List<UInt64           >::~List() { FreeArray(); }
template<> INLINE List<Int64            >::~List() { FreeArray(); }
template<> INLINE List<Double           >::~List() { FreeArray(); }
template<> INLINE List<Decimal          >::~List() { FreeArray(); }
template<> INLINE List<Nullable<UInt32 >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<Int32  >>::~List() { FreeArray(); }
template<> INLINE List<Nullable<Single >>::~List() { FreeArray(); }
#endif


template<> INLINE void List<Byte             >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<UInt16           >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<UInt32           >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<SByte            >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Int16            >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Int32            >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Single           >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Char             >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Boolean          >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Byte   >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<UInt16 >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<SByte  >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Int16  >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Char   >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Boolean>>::Resize(UInt32 const &len) { ResizeArray(len); }
#ifdef __X64
template<> INLINE void List<UInt64           >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Int64            >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Double           >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Decimal          >::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<UInt32 >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Int32  >>::Resize(UInt32 const &len) { ResizeArray(len); }
template<> INLINE void List<Nullable<Single >>::Resize(UInt32 const &len) { ResizeArray(len); }
#endif


template<> INLINE List<Byte             > & List<Byte             >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<UInt16           > & List<UInt16           >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<UInt32           > & List<UInt32           >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<SByte            > & List<SByte            >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Int16            > & List<Int16            >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Int32            > & List<Int32            >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Single           > & List<Single           >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Char             > & List<Char             >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Boolean          > & List<Boolean          >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Int64            > & List<Int64            >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Double           > & List<Double           >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Decimal          > & List<Decimal          >::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::RemoveAt(UInt32 const &idx) { return RemoveAtArray(idx); }
#endif


template<> INLINE Byte              & List<Byte             >::operator[](UInt32 const &idx) { return ((Byte             *)_buffer)[idx]; }
template<> INLINE UInt16            & List<UInt16           >::operator[](UInt32 const &idx) { return ((UInt16           *)_buffer)[idx]; }
template<> INLINE UInt32            & List<UInt32           >::operator[](UInt32 const &idx) { return ((UInt32           *)_buffer)[idx]; }
template<> INLINE SByte             & List<SByte            >::operator[](UInt32 const &idx) { return ((SByte            *)_buffer)[idx]; }
template<> INLINE Int16             & List<Int16            >::operator[](UInt32 const &idx) { return ((Int16            *)_buffer)[idx]; }
template<> INLINE Int32             & List<Int32            >::operator[](UInt32 const &idx) { return ((Int32            *)_buffer)[idx]; }
template<> INLINE Single            & List<Single           >::operator[](UInt32 const &idx) { return ((Single           *)_buffer)[idx]; }
template<> INLINE Char              & List<Char             >::operator[](UInt32 const &idx) { return ((Char             *)_buffer)[idx]; }
template<> INLINE Boolean           & List<Boolean          >::operator[](UInt32 const &idx) { return ((Boolean          *)_buffer)[idx]; }
template<> INLINE Nullable<Byte   > & List<Nullable<Byte   >>::operator[](UInt32 const &idx) { return ((Nullable<Byte   >*)_buffer)[idx]; }
template<> INLINE Nullable<UInt16 > & List<Nullable<UInt16 >>::operator[](UInt32 const &idx) { return ((Nullable<UInt16 >*)_buffer)[idx]; }
template<> INLINE Nullable<SByte  > & List<Nullable<SByte  >>::operator[](UInt32 const &idx) { return ((Nullable<SByte  >*)_buffer)[idx]; }
template<> INLINE Nullable<Int16  > & List<Nullable<Int16  >>::operator[](UInt32 const &idx) { return ((Nullable<Int16  >*)_buffer)[idx]; }
template<> INLINE Nullable<Char   > & List<Nullable<Char   >>::operator[](UInt32 const &idx) { return ((Nullable<Char   >*)_buffer)[idx]; }
template<> INLINE Nullable<Boolean> & List<Nullable<Boolean>>::operator[](UInt32 const &idx) { return ((Nullable<Boolean>*)_buffer)[idx]; }
#ifdef __X64
template<> INLINE UInt64            & List<UInt64           >::operator[](UInt32 const &idx) { return ((UInt64           *)_buffer)[idx]; }
template<> INLINE Int64             & List<Int64            >::operator[](UInt32 const &idx) { return ((Int64            *)_buffer)[idx]; }
template<> INLINE Double            & List<Double           >::operator[](UInt32 const &idx) { return ((Double           *)_buffer)[idx]; }
template<> INLINE Decimal           & List<Decimal          >::operator[](UInt32 const &idx) { return ((Decimal          *)_buffer)[idx]; }
template<> INLINE Nullable<UInt32 > & List<Nullable<UInt32 >>::operator[](UInt32 const &idx) { return ((Nullable<UInt32 >*)_buffer)[idx]; }
template<> INLINE Nullable<Int32  > & List<Nullable<Int32  >>::operator[](UInt32 const &idx) { return ((Nullable<Int32  >*)_buffer)[idx]; }
template<> INLINE Nullable<Single > & List<Nullable<Single >>::operator[](UInt32 const &idx) { return ((Nullable<Single >*)_buffer)[idx]; }
#endif


template<> INLINE Byte              const & List<Byte             >::operator[](UInt32 const &idx) const { return ((Byte             *)_buffer)[idx]; }
template<> INLINE UInt16            const & List<UInt16           >::operator[](UInt32 const &idx) const { return ((UInt16           *)_buffer)[idx]; }
template<> INLINE UInt32            const & List<UInt32           >::operator[](UInt32 const &idx) const { return ((UInt32           *)_buffer)[idx]; }
template<> INLINE SByte             const & List<SByte            >::operator[](UInt32 const &idx) const { return ((SByte            *)_buffer)[idx]; }
template<> INLINE Int16             const & List<Int16            >::operator[](UInt32 const &idx) const { return ((Int16            *)_buffer)[idx]; }
template<> INLINE Int32             const & List<Int32            >::operator[](UInt32 const &idx) const { return ((Int32            *)_buffer)[idx]; }
template<> INLINE Single            const & List<Single           >::operator[](UInt32 const &idx) const { return ((Single           *)_buffer)[idx]; }
template<> INLINE Char              const & List<Char             >::operator[](UInt32 const &idx) const { return ((Char             *)_buffer)[idx]; }
template<> INLINE Boolean           const & List<Boolean          >::operator[](UInt32 const &idx) const { return ((Boolean          *)_buffer)[idx]; }
template<> INLINE Nullable<Byte   > const & List<Nullable<Byte   >>::operator[](UInt32 const &idx) const { return ((Nullable<Byte   >*)_buffer)[idx]; }
template<> INLINE Nullable<UInt16 > const & List<Nullable<UInt16 >>::operator[](UInt32 const &idx) const { return ((Nullable<UInt16 >*)_buffer)[idx]; }
template<> INLINE Nullable<SByte  > const & List<Nullable<SByte  >>::operator[](UInt32 const &idx) const { return ((Nullable<SByte  >*)_buffer)[idx]; }
template<> INLINE Nullable<Int16  > const & List<Nullable<Int16  >>::operator[](UInt32 const &idx) const { return ((Nullable<Int16  >*)_buffer)[idx]; }
template<> INLINE Nullable<Char   > const & List<Nullable<Char   >>::operator[](UInt32 const &idx) const { return ((Nullable<Char   >*)_buffer)[idx]; }
template<> INLINE Nullable<Boolean> const & List<Nullable<Boolean>>::operator[](UInt32 const &idx) const { return ((Nullable<Boolean>*)_buffer)[idx]; }
#ifdef __X64
template<> INLINE UInt64            const & List<UInt64           >::operator[](UInt32 const &idx) const { return ((UInt64           *)_buffer)[idx]; }
template<> INLINE Int64             const & List<Int64            >::operator[](UInt32 const &idx) const { return ((Int64            *)_buffer)[idx]; }
template<> INLINE Double            const & List<Double           >::operator[](UInt32 const &idx) const { return ((Double           *)_buffer)[idx]; }
template<> INLINE Decimal           const & List<Decimal          >::operator[](UInt32 const &idx) const { return ((Decimal          *)_buffer)[idx]; }
template<> INLINE Nullable<UInt32 > const & List<Nullable<UInt32 >>::operator[](UInt32 const &idx) const { return ((Nullable<UInt32 >*)_buffer)[idx]; }
template<> INLINE Nullable<Int32  > const & List<Nullable<Int32  >>::operator[](UInt32 const &idx) const { return ((Nullable<Int32  >*)_buffer)[idx]; }
template<> INLINE Nullable<Single > const & List<Nullable<Single >>::operator[](UInt32 const &idx) const { return ((Nullable<Single >*)_buffer)[idx]; }
#endif


template<> INLINE List<Byte             > & List<Byte             >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<UInt16           > & List<UInt16           >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<UInt32           > & List<UInt32           >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<SByte            > & List<SByte            >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Int16            > & List<Int16            >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Int32            > & List<Int32            >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Single           > & List<Single           >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Char             > & List<Char             >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Boolean          > & List<Boolean          >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Int64            > & List<Int64            >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Double           > & List<Double           >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Decimal          > & List<Decimal          >::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::Remove(UInt32 const &offset) { return RemoveArray(offset); }
#endif


template<> INLINE List<Byte             > & List<Byte             >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<UInt16           > & List<UInt16           >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<UInt32           > & List<UInt32           >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<SByte            > & List<SByte            >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Int16            > & List<Int16            >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Int32            > & List<Int32            >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Single           > & List<Single           >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Char             > & List<Char             >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Boolean          > & List<Boolean          >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Int64            > & List<Int64            >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Double           > & List<Double           >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Decimal          > & List<Decimal          >::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::Remove(UInt32 const &offset, UInt32 const &len) { return RemoveArray(offset, len); }
#endif


template<> INLINE List<Byte             > & List<Byte             >::Clear() { _count = 0; return *this; }
template<> INLINE List<UInt16           > & List<UInt16           >::Clear() { _count = 0; return *this; }
template<> INLINE List<UInt32           > & List<UInt32           >::Clear() { _count = 0; return *this; }
template<> INLINE List<SByte            > & List<SByte            >::Clear() { _count = 0; return *this; }
template<> INLINE List<Int16            > & List<Int16            >::Clear() { _count = 0; return *this; }
template<> INLINE List<Int32            > & List<Int32            >::Clear() { _count = 0; return *this; }
template<> INLINE List<Single           > & List<Single           >::Clear() { _count = 0; return *this; }
template<> INLINE List<Char             > & List<Char             >::Clear() { _count = 0; return *this; }
template<> INLINE List<Boolean          > & List<Boolean          >::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::Clear() { _count = 0; return *this; }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::Clear() { _count = 0; return *this; }
template<> INLINE List<Int64            > & List<Int64            >::Clear() { _count = 0; return *this; }
template<> INLINE List<Double           > & List<Double           >::Clear() { _count = 0; return *this; }
template<> INLINE List<Decimal          > & List<Decimal          >::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::Clear() { _count = 0; return *this; }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::Clear() { _count = 0; return *this; }
#endif


template<> INLINE List<Byte             > & List<Byte             >::Insert(UInt32 const &idx, Byte              const &v) { return InsertArray(idx, v); }
template<> INLINE List<UInt16           > & List<UInt16           >::Insert(UInt32 const &idx, UInt16            const &v) { return InsertArray(idx, v); }
template<> INLINE List<UInt32           > & List<UInt32           >::Insert(UInt32 const &idx, UInt32            const &v) { return InsertArray(idx, v); }
template<> INLINE List<SByte            > & List<SByte            >::Insert(UInt32 const &idx, SByte             const &v) { return InsertArray(idx, v); }
template<> INLINE List<Int16            > & List<Int16            >::Insert(UInt32 const &idx, Int16             const &v) { return InsertArray(idx, v); }
template<> INLINE List<Int32            > & List<Int32            >::Insert(UInt32 const &idx, Int32             const &v) { return InsertArray(idx, v); }
template<> INLINE List<Single           > & List<Single           >::Insert(UInt32 const &idx, Single            const &v) { return InsertArray(idx, v); }
template<> INLINE List<Char             > & List<Char             >::Insert(UInt32 const &idx, Char              const &v) { return InsertArray(idx, v); }
template<> INLINE List<Boolean          > & List<Boolean          >::Insert(UInt32 const &idx, Boolean           const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::Insert(UInt32 const &idx, Nullable<Byte   > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Insert(UInt32 const &idx, Nullable<UInt16 > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::Insert(UInt32 const &idx, Nullable<SByte  > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::Insert(UInt32 const &idx, Nullable<Int16  > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::Insert(UInt32 const &idx, Nullable<Char   > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::Insert(UInt32 const &idx, Nullable<Boolean> const &v) { return InsertArray(idx, v); }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::Insert(UInt32 const &idx, UInt64            const &v) { return InsertArray(idx, v); }
template<> INLINE List<Int64            > & List<Int64            >::Insert(UInt32 const &idx, Int64             const &v) { return InsertArray(idx, v); }
template<> INLINE List<Double           > & List<Double           >::Insert(UInt32 const &idx, Double            const &v) { return InsertArray(idx, v); }
template<> INLINE List<Decimal          > & List<Decimal          >::Insert(UInt32 const &idx, Decimal           const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Insert(UInt32 const &idx, Nullable<UInt32 > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::Insert(UInt32 const &idx, Nullable<Int32  > const &v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::Insert(UInt32 const &idx, Nullable<Single > const &v) { return InsertArray(idx, v); }
#endif


template<> INLINE List<Byte             > & List<Byte             >::Insert(UInt32 const &idx, Byte              &&v) { return InsertArray(idx, v); }
template<> INLINE List<UInt16           > & List<UInt16           >::Insert(UInt32 const &idx, UInt16            &&v) { return InsertArray(idx, v); }
template<> INLINE List<UInt32           > & List<UInt32           >::Insert(UInt32 const &idx, UInt32            &&v) { return InsertArray(idx, v); }
template<> INLINE List<SByte            > & List<SByte            >::Insert(UInt32 const &idx, SByte             &&v) { return InsertArray(idx, v); }
template<> INLINE List<Int16            > & List<Int16            >::Insert(UInt32 const &idx, Int16             &&v) { return InsertArray(idx, v); }
template<> INLINE List<Int32            > & List<Int32            >::Insert(UInt32 const &idx, Int32             &&v) { return InsertArray(idx, v); }
template<> INLINE List<Single           > & List<Single           >::Insert(UInt32 const &idx, Single            &&v) { return InsertArray(idx, v); }
template<> INLINE List<Char             > & List<Char             >::Insert(UInt32 const &idx, Char              &&v) { return InsertArray(idx, v); }
template<> INLINE List<Boolean          > & List<Boolean          >::Insert(UInt32 const &idx, Boolean           &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Byte   >> & List<Nullable<Byte   >>::Insert(UInt32 const &idx, Nullable<Byte   > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<UInt16 >> & List<Nullable<UInt16 >>::Insert(UInt32 const &idx, Nullable<UInt16 > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<SByte  >> & List<Nullable<SByte  >>::Insert(UInt32 const &idx, Nullable<SByte  > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Int16  >> & List<Nullable<Int16  >>::Insert(UInt32 const &idx, Nullable<Int16  > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Char   >> & List<Nullable<Char   >>::Insert(UInt32 const &idx, Nullable<Char   > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Boolean>> & List<Nullable<Boolean>>::Insert(UInt32 const &idx, Nullable<Boolean> &&v) { return InsertArray(idx, v); }
#ifdef __X64
template<> INLINE List<UInt64           > & List<UInt64           >::Insert(UInt32 const &idx, UInt64            &&v) { return InsertArray(idx, v); }
template<> INLINE List<Int64            > & List<Int64            >::Insert(UInt32 const &idx, Int64             &&v) { return InsertArray(idx, v); }
template<> INLINE List<Double           > & List<Double           >::Insert(UInt32 const &idx, Double            &&v) { return InsertArray(idx, v); }
template<> INLINE List<Decimal          > & List<Decimal          >::Insert(UInt32 const &idx, Decimal           &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<UInt32 >> & List<Nullable<UInt32 >>::Insert(UInt32 const &idx, Nullable<UInt32 > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Int32  >> & List<Nullable<Int32  >>::Insert(UInt32 const &idx, Nullable<Int32  > &&v) { return InsertArray(idx, v); }
template<> INLINE List<Nullable<Single >> & List<Nullable<Single >>::Insert(UInt32 const &idx, Nullable<Single > &&v) { return InsertArray(idx, v); }
#endif




template<typename T>
INLINE void List<T>::Init() {                                                            // 重载: 不初始化 _length
    _buffer        = NULL;
    _count         = 0;
    _length        = 0;
}
template<typename T>
void List<T>::Init(UInt32 const &len) {                                             // 重载: Array
    _count = 0;
    var bytes = sizeof(Byte*) + sizeof(T*) * len + sizeof(T) * len;                 // 计算要请求的 buf 大小 ( buffer 指针 + 索引区 + data )
    var sizets = (bytes + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1);     // 换算成 size_t 的个数
    var buf = (Byte*)new size_t[sizets];                                            // 以 size_t 方式 new 确保 8 字节对齐 (x64下)
    *(Byte**)buf = NULL;                                                            // 设 buf 链表头 (NULL)
    _buffer = buf + sizeof(Byte*);                                                  // _buffer 指向数据区
    _length = len;                                                                  // 存索引总数（也是当前Buffer中的索引数）
    var idxs = (T**)_buffer;                                                        // 指向 buf 中的 Indexs 区
    var datas = (T*)(idxs + len);                                                   // 指到 buf 中的 数据 区
    for (UInt32 i = 0; i < len; ++i)                                                // 存数据区指针到Indexs区
        idxs[i] = datas + i;
}
template<typename T>
INLINE List<T>::List() {
    Init();
}
template<typename T>
INLINE List<T>::List(UInt32 const &len) {
    if (len)
        Init(len);
    else
        Init();
}
template<typename T>
INLINE List<T>::List(List const &o) {                                                // 重载：Array
    Init(o._count);
    _count = o._count;
    for (UInt32 i = 0; i < o._count; ++i)
        new (((T**)_buffer)[i]) T(*((T**)o._buffer)[i]);                            // 逐个调复制构造
}
template<typename T>
template<UInt32 len>
INLINE List<T>::List(T const (&arr)[len]) {                                         // 重载：Array
    Init(len);
    _count = len;
    for (UInt32 i = 0; i < len; ++i)
        new (((T**)_buffer)[i]) T(arr[i]);
}
template<typename T>
List<T>::~List() {                                                                  // 重载：Array
    if (_buffer == NULL)
        return;
    for (UInt32 i = 0; i < _count; ++i)                                             // 依次回收已使用的元素( 调析构 )
        (*this)[i].~T();
    var p = _buffer - sizeof(Byte*);
Lab1:
    if (p == NULL)
        return;
    var next = *(Byte**)p;                                                          // 取下一个内存块地址
    delete [] p;
    p = next;
    goto Lab1;
}
template<typename T>
void List<T>::Resize(UInt32 const &len) {                                         // 重载：Array
    if (_buffer == NULL) {
        Init(len);
        return;
    }
    UInt32 len_ = len;
    var incLen = len_ - _length;                                            // 计算 resize 的元素个数
    var idxAreaItemsLen = (UInt32)(sizeof(T*) * _length / sizeof(T));       // 计算原 Buffer 存放 Indexs 的区域能存多少个元素
    if (idxAreaItemsLen > incLen) {                                         // 如果 Indexs区域 可以创建元素则创建 (修正 resize 元素个数, 请求内存，复制，再创建)
        len_ += idxAreaItemsLen - incLen;                                   // 避免浪费, 令请求的 len 变长
        incLen = 0;
    } else
        incLen -= idxAreaItemsLen;
    var bytes = sizeof(Byte*) + sizeof(T*) * len_ + sizeof(T) * incLen;
    var sizets = (bytes + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1);
    var buf = (Byte*)new size_t[ sizets ];                                  // 请求新内存 buf
    *(Byte**)buf = _buffer - sizeof(Byte*);                                 // buf 头 链接到老 Buffer
    buf += sizeof(Byte*);                                                   // buf 指向 Indexs
    memcpy(buf, _buffer, sizeof(T*) * _length);                             // 复制 Buffer 中的 Indexs 区到 buf 中的 Indexs 区
    var idxs = ((T**)buf) + _length;                                        // 令 idxs 指向 buf 中的 Indexs 区的 space 头
    if (idxAreaItemsLen) {
        var items = (T*)_buffer;                                            // 令 items 指向 Buffer 中的 Indexs 区
        for (UInt32 i = 0; i < idxAreaItemsLen; ++i)                        // 将 Indexs 区中的元素指针写入 idxs
            idxs[i] = items + i;
        idxs += idxAreaItemsLen;                                            // 将 idxs 的位置指向未使用的 Indexs
    }
    if (incLen) {
        var items = (T*)(((T**)buf) + len_);                                // 指到 buf 中的元素数据区
        for (UInt32 i = 0; i < incLen; ++i)                                 // 将 Indexs 区中的元素指针写入 idxs
            idxs[i] = items + i;
    }
    _length = len_;                                                         // 更新 IndexsLength
    _buffer = buf;                                                          // 更新 Buffer
}
template<typename T>
INLINE List<T> & List<T>::Add(T const &v) {
    if (_count == _length)
        Resize((_count + 1) * 2);
    new (&(*this)[_count++]) T(v);
    return *this;
}
template<typename T>
INLINE List<T> & List<T>::Add(T &&v) {
    if (_count == _length)
        Resize((_count + 1) * 2);
    new (&(*this)[_count++]) T((T&&)v);
    return *this;
}
template<typename T>
List<T> & List<T>::RemoveAt(UInt32 const &idx) {                                // 重载：Array
    if (idx >= _count)
        return *this;
    if (_count == 0)
        return *this;
    --_count;
    (*this)[idx].~T();
    if (idx == _count)
        return *this;
    var idxs = (T**)_buffer;
    var tmp = idxs[idx];
    memmove(&idxs[idx], &idxs[idx + 1], sizeof(T*) * (_count - idx));
    idxs[_count] = tmp;
    return *this;
}
template<typename T>
List<T> & List<T>::Remove(UInt32 const &offset, UInt32 const &len) {            // 重载：Array
    if (len == 0)
        return Remove(offset);
    if (offset >= _count)
        return *this;
    UInt32 len_ = len;
    if (len_ > _count - offset)
        len_ = _count - offset;
    _count -= len_;
    for (var i = offset; i < offset + len_; ++i)
        (*this)[i].~T();
    var idxs = (T**)_buffer;
    memmove(&idxs[offset], &idxs[offset + len_], sizeof(T*) * (_count - offset));
    return *this;
}
template<typename T>
INLINE List<T> & List<T>::Remove(UInt32 const &offset) {                               // 重载：Array
    for (var i = offset; i < _count; ++i)
        (*this)[i].~T();
    _count = offset;
    return *this;
}
template<typename T>
INLINE List<T> & List<T>::Clear() {                                                    // 重载：Array
    for (UInt32 i = 0; i < _count; ++i)                                             // 依次回收已使用的节点( 调析构 )
        (*this)[i].~T();
    _count = 0;
    return *this;
}
template<typename T>
List<T> & List<T>::Insert(UInt32 const &idx, T const &v) {                      // 重载：Array
    if (idx >= _count)
        return Add(v);
    if (_count == _length)
        Resize(_count * 2);
    var idxs = (T**)_buffer;
    var tmp = idxs[_count];
    memmove(&idxs[idx + 1], &idxs[idx], sizeof(T*) * (_count - idx));
    idxs[idx] = tmp;
    new (tmp) T(v);
    ++_count;
    return *this;
}
template<typename T>
List<T> & List<T>::Insert(UInt32 const &idx, T &&v) {                           // 重载：Array
    if (idx >= _count)
        return Add((T&&)v);
    if (_count == _length)
        Resize(_count * 2);
    var idxs = (T**)_buffer;
    var tmp = idxs[_count];
    memmove(&idxs[idx + 1], &idxs[idx], sizeof(T*) * (_count - idx));
    idxs[idx] = tmp;
    new (tmp) T((T&&)v);
    ++_count;
    return *this;
}
template<typename T>
INLINE Boolean List<T>::Contains(T const &t) const {
    for (UInt32 i = 0; i < _count; ++i)
        if ((*this)[i] == t)
            return true;
    return false;
}

template<typename T>
Boolean List<T>::operator==(List const &o) const {
    if (_count == 0 && o.Length == 0)
        return true;
    if (_count != o.Length)
        return false;
    for (UInt32 i = 0; i < _count; ++i) {
        if ((*this)[i] == o[i])
            continue;
        return false;
    }
    return true;
}
template<typename T>
INLINE T & List<T>::operator[](UInt32 const &idx) {                                      // 重载 Array
    return *((T**)_buffer)[idx];
}
template<typename T>
INLINE T const & List<T>::operator[](UInt32 const &idx) const {                          // 重载 Array
    return *((T**)_buffer)[idx];
}

template <typename T>
INLINE List<T> & List<T>::operator<<(T const &t) {
    return Add(t);
}
template <typename T>
INLINE List<T> & List<T>::operator<<(T &&t) {
    return Add((T&&)t);
}


template<typename T>
INLINE UInt32 & List<T>::Length() {
    return _count;
}
template<typename T>
INLINE UInt32 const & List<T>::Length() const {
    return _count;
}

template<typename T>
INLINE void List<T>::EnsureLength(UInt32 const &len) {
    if (len > _length)
        Resize(len);
    _count = len;
}

template<typename T>
INLINE void List<T>::EnsureSpace(UInt32 const &len) {
    if (_count + len > _length)
        Resize((_count + len) * 2);
}








template<typename T>
INLINE void List<T>::InitArray(UInt32 const &len) {
    _count = 0;
    var bytes = sizeof(T) * len;                                                    // 计算要请求的 buf 大小 (data)
    var sizets = (bytes + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1);     // 换算成 size_t 的个数
    _buffer = (Byte*)new size_t[sizets];                                            // 以 size_t 方式 new 确保 8 字节对齐 (x64下)
    _length = len;
}
template<typename T>
INLINE void List<T>::ConsFromArray(List<T> const &o) {
    Init(o.Length());
    _count = o.Length();
    memcpy(_buffer, o._buffer, sizeof(T) * o.Length());
}
template<typename T>
template<UInt32 len>
INLINE void List<T>::ConsFromFixedArray(T const (&arr)[len]) {
    Init(len);
    _count = len;
    memcpy(_buffer, arr, sizeof(T) * len);
}
template<typename T>
INLINE void List<T>::FreeArray() {
    if (_buffer)
        delete [] _buffer;
}
template<typename T>
void List<T>::ResizeArray(UInt32 const &len) {
    if (_buffer == NULL) {
        Init(len);
        return;
    }
    var bytes = sizeof(T) * len;
    var sizets = (bytes + (sizeof(size_t) - 1)) >> ((sizeof(size_t) >> 2) + 1);
    var buf = (Byte*)new size_t[ sizets ];
    memcpy(buf, _buffer, sizeof(T) * _count);
    delete [] _buffer;
    _buffer = buf;
    _length = len;
}
template<typename T>
List<T> & List<T>::RemoveAtArray(UInt32 const &idx) {
    if (idx >= _count)
        return *this;
    if (_count == 0)
        return *this;
    --_count;
    (*this)[idx].~T();
    if (idx == _count)
        return *this;
    var items = (T*)_buffer;
    memmove(&items[idx], &items[idx + 1], sizeof(T) * (_count - idx));
    return *this;
}
template<typename T>
List<T> & List<T>::RemoveArray(UInt32 const &offset, UInt32 const &len) {            // 重载：Array
    if (len == 0)
        return Remove(offset);
    if (offset >= _count)
        return *this;
    UInt32 len_ = len;
    if (len_ > _count - offset)
        len_ = _count - offset;
    _count -= len_;
    var items = (T*)_buffer;
    memmove(&items[offset], &items[offset + len_], sizeof(T) * (_count - offset));
    return *this;
}
template<typename T>
INLINE List<T> & List<T>::RemoveArray(UInt32 const &offset) {                               // 重载：Array
    _count = offset;
    return *this;
}
template<typename T>
List<T> & List<T>::InsertArray(UInt32 const &idx, T const &v) {                      // 重载：Array
    if (idx >= _count)
        return Add(v);
    if (_count == _length)
        Resize(_count * 2);
    var items = (T*)_buffer;
    memmove(&items[idx + 1], &items[idx], sizeof(T) * (_count - idx));
    new (&items[idx]) T(v);
    ++_count;
    return *this;
}




}
#endif
