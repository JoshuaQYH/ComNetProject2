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

	/*������Ϣ���ĵ�Ŀ�ĵ�*/
	bool SendMessage(string message, int dstRouterID);

	/*ת����Ϣ���ĵ���һĿ�ĵ�*/
	void ForwardMessage(MyProtoMsg* msg, int nextRouterID);

	/*����·����Ϣ*/
	void UpdateRouterInfo(MyProtoMsg* msg, int nextRouterID);



};

#endif

