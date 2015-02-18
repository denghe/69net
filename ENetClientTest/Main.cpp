#include "Lib/All.h"
#include <enet/enet.h>
using namespace xxx;
int test()
{
    if( auto rtv = enet_initialize() ) return rtv;
    ScopeGuard sg_enet_deinitialize( [] { enet_deinitialize(); } );

    auto client = enet_host_create( nullptr, 1, 2, 0, 0 ); //57600 / 8, 14400 / 8 );
    if( !client ) return -1;
    ScopeGuard sg_client( [ = ] { enet_host_destroy( client ); } );

    ENetAddress address = { ENET_HOST_ANY, 1234 };
    enet_address_set_host( &address, "localhost" );

    /* Initiate the connection, allocating the two channels 0 and 1. */
    auto peer = enet_host_connect( client, &address, 2, 0 );
    if( !peer ) return -2;

    int sendLen = 1024;
    ByteBuffer buf( sendLen );
    buf.Resize( sendLen );
    for( int i = 0; i < sendLen; ++i )
    {
        buf[ i ] = (byte)i;
    }

    ENetEvent event;
    while( true )
    {
        /* Wait up to 1000 milliseconds for an event. */
        if( enet_host_service( client, &event, 1 ) )
        {
            switch( event.type )
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf( "A server connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port );
                /* Store any relevant client information here. */
                event.peer->data = "server information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                //printf( "A packet of length %u containing %s was received from %s on channel %u.\n",
                //        event.packet->dataLength,
                //        event.packet->data,
                //        event.peer->data,
                //        event.channelID );
                /* Clean up the packet now that we're done using it. */
                // todo: check
                enet_packet_destroy( event.packet );

                auto packet = enet_packet_create( buf.buf, buf.dataLen, ENET_PACKET_FLAG_RELIABLE );
                enet_peer_send( peer, 0, packet );
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                printf( "%s disconnected.\n", event.peer->data );
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
        }
        //else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

        //printf( "Say > " );
        //char message[ 1024 ];
        //gets( message );
        //if( strlen( message ) > 0 )
        //{
        //    // �ⶫ���ƺ����� service() �� flush() ��ʱ��ŷ����һ��Զ�ɾ������
        //    auto packet = enet_packet_create( message, strlen( message ) + 1, ENET_PACKET_FLAG_RELIABLE );
        //    enet_peer_send( peer, 0, packet );
        //}
    }









    //ENetEvent event;
    ///* Wait up to 5 seconds for the connection attempt to succeed. */
    //if( enet_host_service( client, &event, 5000 ) > 0 &&
    //    event.type == ENET_EVENT_TYPE_CONNECT )
    //{
    //    puts( "Connection to server:1234 succeeded." );
    //    // ...
    //}
    //else
    //{
    //    /* Either the 5 seconds are up or a disconnect event was */
    //    /* received. Reset the peer in the event the 5 seconds   */
    //    /* had run out without any significant event.            */
    //    enet_peer_reset( peer );
    //    puts( "Connection to server:1234 failed." );
    //}

    ///* Create a reliable packet of size 7 containing "packet\0" */
    //ENetPacket * packet = enet_packet_create( "packet",
    //                                          strlen( "packet" ) + 1,
    //                                          ENET_PACKET_FLAG_RELIABLE );
    ///* Extend the packet so and append the string "foo", so it now */
    ///* contains "packetfoo\0"                                      */
    //enet_packet_resize( packet, strlen( "packetfoo" ) + 1 );
    //strcpy( &packet->data[ strlen( "packet" ) ], "foo" );

    ///* Send the packet to the peer over channel id 0. */
    ///* One could also broadcast the packet by         */
    ///* enet_host_broadcast (host, 0, packet);         */
    //enet_peer_send( peer, 0, packet );
    ///* One could just use enet_host_service() instead. */
    //enet_host_flush( client );

    return 0;
}



int main()
{
    if( auto rtv = test() )
    {
        Cout( rtv );
    }

    system( "pause" );
    return 0;
}

