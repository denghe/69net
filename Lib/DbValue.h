#pragma once

namespace xxx
{
    struct DbValue
    {
        DbRow*           parent;
        int              columnIndex;

    private:
        bool Nullable() const;
        List<bool>& GetNullflag();
        List<bool> const& GetNullflag() const;
        DbArray& GetColumnData();
        DbArray const& GetColumnData() const;
        DbColumn& GetColumn();
        DbColumn const& GetColumn() const;

    public:
        bool IsNull() const;
        void SetNull();

        template<typename T>
        void Assign( T v );

        template<typename T>
        T GetValue() const;

        template<typename T>
        T& RefValue();

        template<typename T>
        void SetValue( T&& v );

        inline explicit operator uint8() { return GetValue<uint8>(); }
        inline explicit operator uint16() { return GetValue<uint16>(); }
        inline explicit operator uint32() { return GetValue<uint32>(); }
        inline explicit operator uint64() { return GetValue<uint64>(); }
        inline explicit operator int8() { return GetValue<int8>(); }
        inline explicit operator int16() { return GetValue<int16>(); }
        inline explicit operator int32() { return GetValue<int32>(); }
        inline explicit operator int64() { return GetValue<int64>(); }
        inline explicit operator float() { return GetValue<float>(); }
        inline explicit operator double() { return GetValue<double>(); }
        inline explicit operator bool() { return GetValue<bool>(); }
        inline explicit operator DateTime() { return GetValue<DateTime>(); }
        inline explicit operator ByteBuffer() { return GetValue<ByteBuffer>(); }
        inline explicit operator String() { return GetValue<String>(); }


        void Dump( String& output ) const;
    };

}
