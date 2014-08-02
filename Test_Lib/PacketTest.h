#ifndef __PACKETTEST_H__
#define __PACKETTEST_H__


namespace PacketTestPackets
{
    class Foo;
    class Foo2;
}
namespace PacketTestPackets
{
    class Foo : Memmoveable
    {
        PACKET_CLASS_HEADER_POD( Foo );
public:

        /*
         * Byte的备注
        */
        byte _byte = 0;
        /*
         * Ushort的备注
        */
        ushort _ushort = 0;
        /*
         * Uint的备注
        */
        uint _uint = 0;
        uint64 _ulong = 0;
        sbyte _sbyte = 0;
        short _short = 0;
        int _int = 0;
        int64 _long = 0;
        double _double = 0;
        float _float = 0;
        bool _bool = false;

        static void fill( FlatBuffer& fb
            , byte Byte
            , ushort Ushort
            , uint Uint
            , uint64 Ulong
            , sbyte Sbyte
            , short Short
            , int Int
            , int64 Long
            , double Double
            , float Float
            , bool Bool );

    };
    /*
     * pod class too
    */
    class Foo2 : Memmoveable
    {
        PACKET_CLASS_HEADER_POD( Foo2 );
public:

        short _short = 0;
        int _int = 0;
        Foo _f;
        int64 _long = 0;

        static void fill( FlatBuffer& fb
            , short Short
            , int Int
            , Foo f
            , int64 Long );

    };
}
#endif
