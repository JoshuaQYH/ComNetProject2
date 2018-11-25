#pragma once
#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_
#include "utils.h"
/**********************************************************8*

Author: qyh

Date created : 18/11/18

Date finished:

Description: ͨ�ŷ����࣬��ʼ��socket�͹رգ����պͷ��ͱ���

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

	/*��ӡͨ�Ŵ�����Ϣ*/
	void ErrMsg(DWORD dwErr) {
		char szErr[1024] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErr, 1024, NULL);
		cout << szErr << endl;
	}

	/*��ɳ�ʼ����������*/
	bool InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort);

	/*��������*/
	bool ExitSocket();

	/*���ͱ��ĵ�Ŀ�ĵ�*/
	bool SendMessageToDst(char* msg, int dstRouterID);

	/*���ձ��ĵ�Ŀ�ĵ�, ������Ϣ���Ĳ�����*/
	char* ReceiveMessageFromSrc(char* msg);

};

#endif

