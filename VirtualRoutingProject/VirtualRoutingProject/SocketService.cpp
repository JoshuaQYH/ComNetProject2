#include "SocketService.h"



/********************************************
�������ܣ� ��ʼ������socket�ͽ���socket
�������룺 ���� IP Port  ����IP Port.
��������� ���������ʼ���ɹ�
Author:    qiuyihao
Date created:  18/11/20
Date finished: 18/11/20
**********************************************/
bool SocketService::InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort)
{
	/* ��ʼ������**************************************8*/
	WSADATA wd1 = { 0 };
	int nStart1 = WSAStartup(MAKEWORD(SOCK_VER, 0), &wd1);
	if (nStart1 != 0) {
		cout << "��ʼ��socket����" << endl;
		return 0;
	}

	/*��������socket***********************************************/
	 sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET) {
		ErrMsg(WSAGetLastError());
		return 0;
	}
	cout << "����socket�ɹ�" << endl;


	/***************����socket�󶨵��˿� ****************/
	sockaddr_in addr1 = { 0 };
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(sendPort);
	addr1.sin_addr.S_un.S_addr = inet_addr(sendIP.c_str());
	int nBind1 = bind(sendSocket, (sockaddr *)&addr1, sizeof(addr1));

	if (nBind1 != 0) {
		cout << "�󶨷���IP�˿ڳ���" << endl;
		return 0;
	}

	/* ȡ�÷���socket�Ѱ󶨵Ķ˿ں�****************************/
	int nLen1 = sizeof(addr1);
	getsockname(sendSocket, (sockaddr *)& addr1, &nLen1);
	cout << "����Socket �ɹ��󶨵��˿�" << ntohs(addr1.sin_port) << "" << endl;


	/* ��ʼ������**************************************8*/
	WSADATA wd2 = { 0 };
	int nStart2 = WSAStartup(MAKEWORD(SOCK_VER, 0), &wd2);
	if (nStart2 != 0) {
		cout << "��ʼ��socket����" << endl;
		return 0;
	}

	/*����receive socket***********************************************/
	receiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (receiveSocket == INVALID_SOCKET) {
		ErrMsg(WSAGetLastError());
		return 0;
	}
	cout << "��������socket�ɹ�" << endl;


	/***************����socket�󶨵��˿� ****************/
	sockaddr_in addr2 = { 0 };
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(receivePort);
	addr2.sin_addr.S_un.S_addr = inet_addr(receiveIP.c_str());
	int nBind2 = bind(receiveSocket, (sockaddr *)&addr2, sizeof(addr2));

	if (nBind2 != 0) {
		cout << "�󶨽���IP�˿ڳ���" << endl;
		return 0;
	}

	/* ȡ�÷���socket�Ѱ󶨵Ķ˿ں�****************************/
	int nLen2 = sizeof(addr2);
	getsockname(receiveSocket, (sockaddr *)& addr2, &nLen2);
	cout << "����Socket �ɹ��󶨵��˿�" << ntohs(addr2.sin_port) << "" << endl;


	/*����recvfrom��ʱʱ��5�룬����5�뷵��*************************/
	struct timeval tv_out;
	tv_out.tv_sec = 5000;   //�ȴ�5��
	tv_out.tv_usec = 0;
	setsockopt(sendSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	setsockopt(receiveSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
}



/********************************************************************
�������ܣ� �ر�socket
��������� ��������رճɹ�
Author:    qiuyihao
Date created:  18/11/20
Date finished: 18/11/20
********************************************************************/
bool SocketService::ExitSocket()
{
	try
	{
		closesocket(sendSocket);
		closesocket(receiveSocket);
		WSACleanup();
		return true;
	}
	catch (const std::exception& e)
	{
		cout << "�ر�socketʱ����" << endl;
		return false;
	}
}

/***************************************************************
�������ܣ� �����յ�����ID�����ת����ȷ����һ����������Ϣ����
�������룺 Msg �ṹ������ָ��    dstRouterID ����Ŀ�ĵ�
��������� ���������ͳɹ�
ȫ�ֱ����� NumToAddrRouterMappingTable
Author:    qiuyihao
Date created:  18/11/20
Date finished: 18/11/20
**************************************************************/
bool SocketService::SendMessageToDst(char * msg, int dstRouterID)
{
	if (ROUTER_PROTOCOL == RIP) {
		if (!RouterWithRIP::Instance().CheckLinkState(dstRouterID))
			return false;
	}
	else if (ROUTER_PROTOCOL == OSPF) {
		if (!RouterWithLS::Instance().CheckLinkState(dstRouterID))
			return false;
	}

	
	sockaddr_in addr = NumToAddrReceiveRouterMappingTable[dstRouterID];

	int nSent = sendto(sendSocket, msg, strlen(msg) + 1, 0, (sockaddr*)&addr, sizeof(addr));
	
	if (nSent == 0) {
		ErrMsg(WSAGetLastError());
		return false;
	}

	return true;
}

/***************************************************************
�������ܣ��������������Ϣ�������õ�һ�ݱ���
������������ؽ��յı���
Author: qiuyihao
Date created: 18/11/20
Date finished: 18/11/20
*****************************************************************/
char * SocketService::ReceiveMessageFromSrc(char* msg)
{
	sockaddr_in otherRouterAddr = { 0 };
	int nFromLen = sizeof(otherRouterAddr);
	int nRecv = recvfrom(receiveSocket,msg, BUFF_SIZE, 0, (sockaddr*)& otherRouterAddr, &nFromLen);

	if (nRecv == SOCKET_ERROR) {
		cout << "�Ӱ���ʱ" << endl;
		return nullptr;
	}
	return msg;
}
