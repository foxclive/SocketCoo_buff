#include "pch.h"
#include <iostream>
#include <stdio.h>
#include "SocketServer_Bufferevent.h"

using namespace std;

SocketServer_Bufferevent::SocketServer_Bufferevent()
{
	//初始化windows套接字
	w = MAKEWORD(2, 0);//定义套接字版本
	WSAStartup(w,&data);//初始化套接字

	//初始化sock地址
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(70);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//初始化Socket
	//socketServer = socket(AF_INET, SOCK_STREAM, 0);//bufferevent 只支持面向流的协议, 即TCP 

	//TCP bufferevent 流程
	base = event_base_new();
	
	listener = evconnlistener_new_bind(base, &SocketServer_Bufferevent::listener_cb, base,
		LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
		(struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
	event_base_dispatch(base);

}

void SocketServer_Bufferevent::listener_cb(struct evconnlistener *, evutil_socket_t fd, struct sockaddr * sockaddr, int socklen, void * data)
{
	//listener callback

	std::string *init_info = new std::string("成功建立连接!");
	struct event_base * base = (event_base*)data;
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_enable(bev,EV_READ);
	bufferevent_setcb(bev, read_cb, write_cb, event_cb,bev);
	//cout << init_info->data()<< " "<< init_info->length()<<endl;
	bufferevent_write(bev,init_info->data(), init_info->length());
}

void SocketServer_Bufferevent::read_cb(bufferevent *, void * bev)
{
	//read callback
	char tmp[255] = {0};
	bufferevent *b = (bufferevent*)bev;
	bufferevent_read(b, tmp, size(tmp));
	cout << tmp << endl;
}


void SocketServer_Bufferevent::write_cb(struct bufferevent *, void * bev)
{
	//write callback
	bufferevent *b = (bufferevent* )bev;
	bufferevent_write(b, NULL, NULL);
}

void SocketServer_Bufferevent::event_cb(struct bufferevent *bev, short events, void *ctx)
{
	//event callback
}

SocketServer_Bufferevent::~SocketServer_Bufferevent()
{
	
	WSACleanup();
	evconnlistener_free(listener);
	event_base_free(base);
}
