#pragma once
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_
#include "utils.h"
/**********************************************************8*

Author: qyh

Date created : 18/11/18

Date finished:

Description: 通信服务类，初始化socket和关闭，接收和发送报文

**************************************************************/

class SocketService {

private:
	SocketService();                            // ctor hidden
	SocketService(SocketService const&);            // copy ctor hidden
	SocketService& operator=(SocketService const&); // assign op. hidden
	~SocketService();                           // dtor hidden

	SOCKET sendSocket = 0;
	SOCKET receiveSocket = 1;

public:
	static SocketService& Instance() {
		static SocketService theSocketService;
		return theSocketService;
	}

	/*打印通信错误信息*/
	void ErrMsg(DWORD dwErr) {
		char szErr[1024] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErr, 1024, NULL);
		cout << szErr << endl;
	}

	/*完成初始的网络配置*/
	bool InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort);

	/*结束连接*/
	bool ExitSocket();

	/*发送报文到目的地*/
	bool SendMessageToDst(char* msg, int dstRouterID);

	/*接收报文到目的地, 接收消息报文并返回*/
	char* ReceiveMessageFromSrc(char* msg);

};

#endif

