#ifndef __PACKETTEST_H__
#define __PACKETTEST_H__


namespace PacketTestPackets
{
    class Foo;
}
namespace PacketTestPackets
{
    class Foo : Memmoveable
    {
        PACKET_CLASS_HEADER_POD( Foo );
public:

        int _isss[3][2][1];
        Dict<String,List<byte[8]>> _eee[5][3];

        static void fill( FlatBuffer& fb
            , int( &isss )[3][2][1]
            , Dict<String,List<byte[8]>>( &eee )[5][3] );

    };
}
#endif
