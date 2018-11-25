#include "SocketService.h"



/********************************************
函数功能： 初始化发送socket和接收socket
函数输入： 发送 IP Port  接收IP Port.
函数输出： 返回真则初始化成功
Author:    qiuyihao
Date created:  18/11/20
Date finished: 18/11/20
**********************************************/
bool SocketService::InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort)
{
	/* 初始化环境**************************************8*/
	WSADATA wd1 = { 0 };
	int nStart1 = WSAStartup(MAKEWORD(SOCK_VER, 0), &wd1);
	if (nStart1 != 0) {
		cout << "初始化socket环境" << endl;
		return 0;
	}

	/*创建发送socket***********************************************/
	 sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET) {
		ErrMsg(WSAGetLastError());
		return 0;
	}
	cout << "创建socket成功" << endl;


	/***************发送socket绑定到端口 ****************/
	sockaddr_in addr1 = { 0 };
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(sendPort);
	addr1.sin_addr.S_un.S_addr = inet_addr(sendIP.c_str());
	int nBind1 = bind(sendSocket, (sockaddr *)&addr1, sizeof(addr1));

	if (nBind1 != 0) {
		cout << "绑定发送IP端口出错" << endl;
		return 0;
	}

	/* 取得发送socket已绑定的端口号****************************/
	int nLen1 = sizeof(addr1);
	getsockname(sendSocket, (sockaddr *)& addr1, &nLen1);
	cout << "发送Socket 成功绑定到端口" << ntohs(addr1.sin_port) << "" << endl;


	/* 初始化环境**************************************8*/
	WSADATA wd2 = { 0 };
	int nStart2 = WSAStartup(MAKEWORD(SOCK_VER, 0), &wd2);
	if (nStart2 != 0) {
		cout << "初始化socket环境" << endl;
		return 0;
	}

	/*创建receive socket***********************************************/
	receiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (receiveSocket == INVALID_SOCKET) {
		ErrMsg(WSAGetLastError());
		return 0;
	}
	cout << "创建接收socket成功" << endl;


	/***************发送socket绑定到端口 ****************/
	sockaddr_in addr2 = { 0 };
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(receivePort);
	addr2.sin_addr.S_un.S_addr = inet_addr(receiveIP.c_str());
	int nBind2 = bind(receiveSocket, (sockaddr *)&addr2, sizeof(addr2));

	if (nBind2 != 0) {
		cout << "绑定接收IP端口出错" << endl;
		return 0;
	}

	/* 取得发送socket已绑定的端口号****************************/
	int nLen2 = sizeof(addr2);
	getsockname(receiveSocket, (sockaddr *)& addr2, &nLen2);
	cout << "接收Socket 成功绑定到端口" << ntohs(addr2.sin_port) << "" << endl;


	/*设置recvfrom超时时间5秒，超过5秒返回*************************/
	struct timeval tv_out;
	tv_out.tv_sec = 5000;   //等待5秒
	tv_out.tv_usec = 0;
	setsockopt(sendSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
	setsockopt(receiveSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out));
}



/********************************************************************
函数功能： 关闭socket
函数输出： 返回真则关闭成功
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
		cout << "关闭socket时出错" << endl;
		return false;
	}
}

/***************************************************************
函数功能： 根据终点主机ID，配合转发表确定下一跳，发送消息报文
函数输入： Msg 结构化报文指针    dstRouterID 报文目的地
函数输出： 返回真则发送成功
全局变量： NumToAddrRouterMappingTable
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
函数功能：接收其他结点消息，解析得到一份报文
函数输出：返回接收的报文
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
		cout << "接包超时" << endl;
		return nullptr;
	}
	return msg;
}
