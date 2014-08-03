#include "Lib/All.h"
#include "PacketTestPackets.h"

namespace PacketTestPackets
{
    ushort Foo::getTypeID()
    {
        return 0;
    }
    void Foo::fill( FlatBuffer& fb
        , byte pByte
        , ushort pUshort
        , uint pUint
        , uint64 pUlong
        , sbyte pSbyte
        , short pShort
        , int pInt
        , int64 pLong
        , double pDouble
        , float pFloat
        , bool pBool )
    {
        fb.writes(
            pByte, 
            pUshort, 
            pUint, 
            pUlong, 
            pSbyte, 
            pShort, 
            pInt, 
            pLong, 
            pDouble, 
            pFloat, 
            pBool );
    }

    void Foo::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _byte, 
            _ushort, 
            _uint, 
            _ulong, 
            _sbyte, 
            _short, 
            _int, 
            _long, 
            _double, 
            _float, 
            _bool );
    }

    bool Foo::readBuffer( FlatBuffer& fb )
    {
        if( fb.size() < getWriteBufferSize() + fb.offset() ) return false;
        memcpy( this, fb.data() + fb.offset(), sizeof( Foo ) );
        fb.offset() += sizeof( Foo );

        //return fb.reads(
        //    _byte, 
        //    _ushort, 
        //    _uint, 
        //    _ulong, 
        //    _sbyte, 
        //    _short, 
        //    _int, 
        //    _long, 
        //    _double, 
        //    _float, 
        //    _bool );
        return true;
    }

    void Foo::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.reserve( fb.size() + sizeof( Foo ) );
        memcpy( fb.data() + fb.size(), this, sizeof( Foo ) );
        fb.size() += sizeof( Foo );
        //fb.writesDirect(
        //    _byte, 
        //    _ushort, 
        //    _uint, 
        //    _ulong, 
        //    _sbyte, 
        //    _short, 
        //    _int, 
        //    _long, 
        //    _double, 
        //    _float, 
        //    _bool );
    }

    int Foo::getWriteBufferSize() const
    {
        return sizeof( Foo );
        //return 
        //    sizeof( _byte ) + 
        //    sizeof( _ushort ) + 
        //    sizeof( _uint ) + 
        //    sizeof( _ulong ) + 
        //    sizeof( _sbyte ) + 
        //    sizeof( _short ) + 
        //    sizeof( _int ) + 
        //    sizeof( _long ) + 
        //    sizeof( _double ) + 
        //    sizeof( _float ) + 
        //    sizeof( _bool );
    }

    ushort Foo2::getTypeID()
    {
        return 1;
    }
    void Foo2::fill( FlatBuffer& fb
        , short pShort
        , int pInt
        , Foo pF
        , int64 pLong )
    {
        fb.writes(
            pShort, 
            pInt, 
            pF, 
            pLong );
    }

    void Foo2::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _short, 
            _int, 
            _f, 
            _long );
    }

    bool Foo2::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _short, 
            _int, 
            _f, 
            _long );
    }

    void Foo2::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _short, 
            _int, 
            _f, 
            _long );
    }

    int Foo2::getWriteBufferSize() const
    {
        return 
            sizeof( _short ) + 
            sizeof( _int ) + 
            _f.getWriteBufferSize() + 
            sizeof( _long );
    }

    ushort Foo3::getTypeID()
    {
        return 2;
    }
    Foo3::Foo3( Foo3 const& other )
        : _f( other._f )
        , _s( other._s )
    {
    }

    Foo3::Foo3( Foo3&& other )
        : _f( other._f )
        , _s( std::move( other._s ) )
    {
    }

    Foo3& Foo3::operator=( Foo3 const& other )
    {
        _f = other._f;
        _s = other._s;
        return *this;
    }

    Foo3& Foo3::operator=( Foo3&& other )
    {
        _f = other._f;
        _s = std::move( other._s );
        return *this;
    }


    void Foo3::fill( FlatBuffer& fb
        , Foo2 pF
        , String pS )
    {
        fb.writes(
            pF, 
            pS );
    }

    void Foo3::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _f, 
            _s );
    }

    bool Foo3::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _f, 
            _s );
    }

    void Foo3::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _f, 
            _s );
    }

    int Foo3::getWriteBufferSize() const
    {
        return 
            _f.getWriteBufferSize() + 
            _s.getWriteBufferSize();
    }

}
