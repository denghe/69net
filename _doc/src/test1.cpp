#include "Precompile.h"

//#include <iostream>	    // cout
//#include <netdb.h>	    // addrinfo, getaddrinfo, socket, bind, freeaddrinfo, listen
//#include <unistd.h>	    // close sfd
//#include <string.h>	    // memset
//#include <fcntl.h>	    // fcntl
//#include <sys/epoll.h>	// epoll_create1, epoll_event, epoll_ctl

bool makeNonBlocking( int sfd )
{
	int flags = fcntl( sfd, F_GETFL, nullptr );
	if( flags == -1 )
	{
		std::cout << "fcntl get error.\n";
		return false;
	}
	flags |= O_NONBLOCK;
	int rtv = fcntl( sfd, F_SETFL, flags );
	if( rtv == -1 )
	{
		std::cout << "fcntl set error.\n";
		return false;
	}
	return true;
}

int createAndBind( char const * port )
{
	addrinfo ai;						// http://baike.baidu.com/view/6757218.htm
	memset( &ai, 0, sizeof(addrinfo) );	// 初始化
	ai.ai_family = AF_UNSPEC;			// 不限 ipv4/6
	ai.ai_socktype = SOCK_STREAM;		// tcp
	ai.ai_flags = AI_PASSIVE;			// 被动，用于 bind

	addrinfo *pai;						// getaddrinfo 的结果容器
	int rtv = getaddrinfo( nullptr, port, &ai, &pai );
	if( rtv )
	{
		std::cout << "getaddrinfo error:" << gai_strerror( rtv ) << "\n";
		return -1;
	}
	FreeHelper fh_pai( [&](){ freeaddrinfo( pai ); } );		// 确保出函数时 pai 得以回收

	int sfd;
	for( addrinfo *p = pai; p != nullptr; p = p->ai_next )
	{
		sfd = socket( p->ai_family, p->ai_socktype, p->ai_protocol );
		if( sfd == -1 )
		{
			continue;					// socket 创建失败，进入下一个循环
		}
		rtv = bind( sfd, p->ai_addr, p->ai_addrlen );
		if( rtv == 0 )
		{
			goto LabSuccess;			// 成功 bind, 跳到后续执行点
		}
		else
		{
			close( sfd );				// 失败立即关闭，进入下一个循环
		}
	}
	std::cout << "bind error.\n";
	return -1;

LabSuccess:
	FreeHelper fh_sfd( [&](){ close( sfd ); } );	// 确保出函数时 sfd 得以回收

    int opt = SO_REUSEADDR;
    rtv = setsockopt( sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) );
    if( rtv == -1 )
    {
		std::cout << "setsockopt error.\n";
		return -1;
    }
	if( !makeNonBlocking( sfd ) )
	{
		return -1;
	}

	fh_sfd.cancel();		// 取消保护, 要返回, 不需回收
	return sfd;
}

const int _maxEvents = 999;
int test1()
{
	int sfd = createAndBind( "12345" );
	if( sfd == -1 )
	{
		return -1;
	}
	FreeHelper fh_sfd( [&](){ close( sfd ); } );	// 确保回收 sfd

	int rtv = listen( sfd, SOMAXCONN );		// 系统设置的 /proc/sys/net/core/somaxconn 也是一层限制, 有文章建议设为 1024
	if( rtv == -1 )							// http://www.hackbase.com/tech/2008-06-18/41054.html
	{
		std::cout << "listen error.\n";
		return -1;
	}

	int efd = epoll_create( _maxEvents );
	if( efd == -1 )
	{
		std::cout << "epoll_create1 error.\n";
		return -1;
	}
	FreeHelper fh_efd( [&](){ close( efd ); } );	// 确保回收 efd

	epoll_event e;
	e.data.fd = sfd;
	e.events = EPOLLIN | EPOLLET;
	rtv = epoll_ctl( efd, EPOLL_CTL_ADD, sfd, &e );
	if( rtv == -1 )
	{
		std::cout << "epoll_ctl error.\n";
		return -1;
	}
	auto es = (epoll_event*)calloc( _maxEvents, sizeof(e) );
	FreeHelper fh_es( [&](){ free( es ); } );

	Dict<int, Binary*> tokens;
	FreeHelper fh_tokens( [&]() {
		for( int i = 0; i < tokens._nodes._len; ++i )
		{
			close( tokens._nodes[ i ]->_key );
			delete tokens._nodes[ i ]->_value;
		}
	} );

	while( true )
	{
		int n = epoll_wait( efd, es, _maxEvents, 0 );	// -1
		if( n == -1 )
		{
			std::cout << "epoll_wait error.\n";
			// if( errno == EINTR )	// 当前不确定这个错误要不要无视
			return -1;
		}
		for( int i = 0; i < n; ++i )
		{
			auto &ce = es[ i ];
			if( ( ce.events & EPOLLERR )
			 || ( ce.events & EPOLLHUP )
			 || !( ce.events & EPOLLIN  ) )
			{
				std::cout << "epoll error.\n";
				continue;
			}
			else if( sfd == ce.data.fd )
			{
				while( true )
				{
					sockaddr sa;
					auto salen = (socklen_t)sizeof(sa);
					int ifd = accept( sfd, &sa, &salen );
					if( ifd == -1 )
					{
						if( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) )
						{
							break;	// 执行到这里的时候已经处理完所有连接请求，跳出
						}
						else
						{
							std::cout << "accept error.\n";
						}
						break;
					}

					char hbuf[ NI_MAXHOST ], sbuf[ NI_MAXSERV ];	// 这段代码是取创建的连接的客户端地址信息
					rtv = getnameinfo( &sa, salen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV );
					if( rtv == 0 )
					{
						std::cout << "accepted. in_fd = " << ifd << ", host = " << hbuf << ", port = " << sbuf << "\n";
					}

					rtv = makeNonBlocking( ifd );
					if( rtv == -1 )
					{
						close( ifd );
						return -1;
					}
					e.data.fd = ifd;
					e.events = EPOLLIN | EPOLLET;
					rtv = epoll_ctl( efd, EPOLL_CTL_ADD, ifd, &e );
					if( rtv == -1 )
					{
						std::cout << "epoll_ctl error.\n";
						close( ifd );
						return -1;
					}
					std::cout << "tokens.add( ifd : " << ifd << " ).\n";
					tokens.add( ifd, new Binary() );
				}
				continue;
			}
			else
			{
				auto ifd = ce.data.fd;
				Binary* bin;
				if( !tokens.tryGet( ifd, bin ) )
				{
					std::cout << "cannot get ifd: " << ifd << " from tokens.\n";
					continue;
				}
				bool done = false;		// 有数据未读，必须读光（ edge triggered mode )
				do
				{
					bin->ensure( 4096 );
					rtv = recv( ifd, bin->_buf + bin->_len, 4096, 0 );
					if( rtv == -1 )
					{
						if( errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK )
						{
							break;
						}
						std::cout << "read error.\n";
						done = true;
						break;
					}
					else if( rtv == 0 )		// 0 长度表示连接已断开
					{
						done = true;
						break;
					}
					bin->_len += rtv;
				}
				while( rtv == 4096 );	// 按某文章的说法，读到的字节数小于 buf 长，就表明已经读完了

				if( !done )	// 顺利读完数据到此
				{
				LabBegin:	// 这里使用 readIdx 来存上一次的扫描点
					for( ; bin->_readIdx < bin->_len; ++bin->_readIdx )
					{
						if( bin->_buf[ bin->_readIdx ] == '\n' )
						{
							++bin->_readIdx;	// 跳过空格
							if( bin->_buf[ 0 ] == '`' )		// 退出
							{
								return 0;
							}
							else if( bin->_buf[ 0 ] == ' ' )	// 广播
							{
								for( int i = 0; i < tokens._nodes._len; ++i )
								{
									auto cifd = tokens._nodes[ i ]->_key;
									if( cifd == ifd ) continue;

									rtv = write( cifd, bin->_buf, bin->_readIdx );
									if( rtv == -1 )
									{
										close( cifd );
										Binary* bin;
										if( !tokens.tryRemove( cifd, bin ) )
										{
											std::cout << "cannot remove cifd: " << cifd << " from tokens.\n";
										}
										else
										{
											delete bin;
										}
									}
								}
							}
							rtv = send( ifd, bin->_buf, bin->_readIdx, 0 );	// 回发收到的数据 \n 截止
							if( rtv == -1 )
							{
								std::cout << "write error.\n";
								done = true;
								break;
							}
							memmove( bin->_buf, bin->_buf + bin->_readIdx, bin->_len - bin->_readIdx );
							bin->_len = bin->_len - bin->_readIdx;
							bin->_readIdx = 0;
							if( bin->_len ) goto LabBegin;
							break;
						}
					}
				}
				if( done ) 	// 出问题需要关闭 ifd
				{
					std::cout << "closed. ce.data.fd = " << ifd << "\n";
					close( ifd );
					Binary* bin;
					if( !tokens.tryRemove( ifd, bin ) )
					{
						std::cout << "cannot remove ifd: " << ifd << " from tokens.\n";
					}
					else
					{
						delete bin;
					}
				}
			}
		}
		// todo: logic code here
		usleep( 1 );
	}
    return 0;
}

// 有可能发生的 fd 丢失现象，听说可以改这个: /proc/sys/net/ipv4/tcp_keepalive_time
/*
# include <netinet/tcp.h>
int keepalive; // 打开TCP KEEPALIVE开关
int keepidle; // 系统等待多长时间开始检测
int keepintvl; // 系统每次探测时间
int keepcnt; // 系统探测几次后执行关闭操作。
setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepalive, sizeof(keepalive))；
isetsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (char *)&keepidle, sizeof(keepidle))；
setsockopt(fd, SOL_TCP, TCP_KEEPINTVL,(char *)&keepintvl, sizeof(keepintvl))；
setsockopt(fd, SOL_TCP, TCP_KEEPCNT,(char *)&keepcnt, sizeof(keepcnt))；

*/

