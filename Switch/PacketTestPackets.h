#ifndef __PACKETTEST_H__
#define __PACKETTEST_H__


namespace PacketTestPackets
{
    class Obj_t1;
    class Obj_t2;
    class Obj_t3;
}
namespace PacketTestPackets
{
    class Obj_t1 : Memmoveable
    {
        PACKET_CLASS_HEADER( Obj_t1 );
public:

        int _i_ = 0;
        String _s;

        static void fill( FlatBuffer& fb
            , int i_
            , String s );

    };
    class Obj_t2 : Memmoveable
    {
        PACKET_CLASS_HEADER_POD( Obj_t2 );
public:

        ::PacketTestPackets::Obj_t1 _tl[12];

        static void fill( FlatBuffer& fb
            , ::PacketTestPackets::Obj_t1( &tl )[12] );

    };
    class Obj_t3 : Memmoveable
    {
        PACKET_CLASS_HEADER_POD( Obj_t3 );
public:

        ::PacketTestPackets::Obj_t2 _tl[12];

        static void fill( FlatBuffer& fb
            , ::PacketTestPackets::Obj_t2( &tl )[12] );

    };
}
#endif
