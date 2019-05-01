#pragma once
#include <string>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "ws2_32.lib")
class SocketServer_Bufferevent
{
private:
	WSADATA data;
	WORD w;
	sockaddr_in serverAddr;
	//SOCKET socketServer;
	struct event_base *base;
	
	struct evconnlistener *listener;
public:
	static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
	static void read_cb(struct bufferevent *, void *);
	static void write_cb(struct bufferevent *, void *);
	static void event_cb(struct bufferevent *bev, short events, void *ctx);
	SocketServer_Bufferevent();
	~SocketServer_Bufferevent();
};

