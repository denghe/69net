#include "Lib/All.h"
#include <conio.h>
using namespace xxx;

#include <enet/enet.h>
int test()
{
    // 这个必须用前用后整体初始化和反初始化，放到这个函数这里不一定合适，理论上讲应该是在程序的开始和结束时分别搞一把
    if( auto rtv = enet_initialize() ) return rtv;
    ScopeGuard sg_enet_deinitialize( [] 
    {
        enet_deinitialize();
    } );

    // 创建一个地址对象, 打算在所有 IP 的 1234 上接受数据
    ENetAddress address = { ENET_HOST_ANY, 1234 };
    // 可以用下面这行具体的指定 IP
    // enet_address_set_host( &address, "x.x.x.x" );

    // 创建 host 对象( 出函数时析构 )
    auto server = enet_host_create( &address, 64, 2, 0, 0 );    // num of clients, num of channels, incoming bandwidth, outgoing bandwidth
    if( !server ) return -1;
    ScopeGuard sg_server( [ = ] { enet_host_destroy( server ); } );

    // 来个 counter 用于显示 以体现 循环速度
    size_t counter = 0;
    // timer 相关
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::seconds durationSec1( 1 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto firstTime = std::chrono::system_clock::now();


    while( true )
    {
        ENetEvent event;
        if( enet_host_service( server, &event, 1 ) )
        {
            switch( event.type )
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                printf( "A new client connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port );
                /* Store any relevant client information here. */
                event.peer->data = "cccccc";
                auto packet = enet_packet_create( "go", 3, ENET_PACKET_FLAG_RELIABLE );
                enet_peer_send( event.peer, 0, packet );
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                //printf( "A packet of length %u containing %s was received from %s on channel %u.\n",
                //        event.packet->dataLength,
                //        event.packet->data,
                //        event.peer->data,
                //        event.channelID );
                /* Clean up the packet now that we're done using it. */
                // todo: counter += packet len
                int len = (int)event.packet->dataLength;
                counter += event.packet->dataLength;
                enet_packet_destroy( event.packet );
                auto packet = enet_packet_create( &len, 4, ENET_PACKET_FLAG_RELIABLE );
                enet_peer_send( event.peer, 0, packet );
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                printf( "%s disconnected.\n", event.peer->data );
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
            }
        }
        //else std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        //++counter;    // counter 直接用于加收到的字节数


        // 低频输出 now 免得控制台输出耗 cpu 太多
        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
            if( !elapsedSeconds ) elapsedSeconds = 1;
            CoutPos( 0, 0, counter, ", ", counter / (int64)elapsedSeconds );
            lastTime = now;

            if( _kbhit() ) break;
        }


        // other logic here
    }
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

