#pragma once
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description:

****************************************************/

class SocketService {

private:
	SocketService();                            // ctor hidden
	SocketService(SocketService const&);            // copy ctor hidden
	SocketService& operator=(SocketService const&); // assign op. hidden
	~SocketService();                           // dtor hidden

public:
	static SocketService& Instance() {
		static SocketService theSocketService;
		return theSocketService;
	}

	/*发送消息报文到目的地*/
	bool SendMessage(string message, int dstRouterID);

	/*转发消息报文到下一目的地*/
	void ForwardMessage(MyProtoMsg* msg, int nextRouterID);

	/*更新路由信息*/
	void UpdateRouterInfo(MyProtoMsg* msg, int nextRouterID);



};

#endif

