#include "Lib/All.h"
#include <enet/enet.h>
using namespace std;

int test()
{
    if( auto rtv = enet_initialize() ) return rtv;
    ScopeGuard sg_enet_deinitialize( [] { enet_deinitialize(); } );

    ENetAddress address = { ENET_HOST_ANY, 1234 };
    // enet_address_set_host( &address, "x.x.x.x" );
    auto server = enet_host_create( &address, 32, 2, 0, 0 );    // num of clients, num of channels, incoming bandwidth, outgoing bandwidth
    if( !server ) return -1;
    ScopeGuard sg_server( [ = ] { enet_host_destroy( server ); } );

    ENetEvent event;
    while( true )
    {
        if( enet_host_service( server, &event, 1000 ) > 0 )     /* Wait up to 1000 milliseconds for an event. */
        {
            switch( event.type )
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf( "A new client connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port );
                /* Store any relevant client information here. */
                event.peer->data = "cccccc";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf( "A packet of length %u containing %s was received from %s on channel %u.\n",
                        event.packet->dataLength,
                        event.packet->data,
                        event.peer->data,
                        event.channelID );
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy( event.packet );

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf( "%s disconnected.\n", event.peer->data );
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
        }
    }
    return 0;
}

int main()
{
    if( auto rtv = test() )
    {
        cout << rtv << endl;
    }

    system( "pause" );
    return 0;
}

