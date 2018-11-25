#include "ControlUnit.h"
#include "jsonCpp\reader.h" /*此处需要依据自己的文件路径修改*/

int ControlUnit::exitFlag = 0;

/*初始化socket*/
bool ControlUnit::InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort)
{
	if (SocketService::Instance().InitSocket(sendIP, sendPort, receiveIP, receivePort)) {
		cout << "初始化socket环境成功,可以发送和接收消息了~~~" << endl;
		return true;
	}
	else {
		cout << "socket环境初始化失败,请重新检查程序和网络设置~~" << endl;
		return false;
	}
}

/*关闭socket*/
bool ControlUnit::ExitSocket()
{
	exitFlag = 1;
	if (SocketService::Instance().ExitSocket()) {
		cout << "关闭socket成功~" << endl;
		return true;
	}
	else {
		cout << "关闭socket失败" << endl;
		return false;
	}
	return false;
}

/*主动发起消息报文*/
bool ControlUnit::SendMessageToOtherRouter(string msg, int dstRouterID)
{
	int nextRouterID = 0;
	if (ROUTER_PROTOCOL == OSPF)
		nextRouterID = RouterWithLS::Instance().DecideForwardingAddress(dstRouterID);
	else if (ROUTER_PROTOCOL == RIP)
		nextRouterID = RouterWithRIP::Instance().DecideForwardingAddress(dstRouterID);

	char*  protoMsg = Message::Instance().CreateMessage(msg, LOCAL_HOST_NUM, dstRouterID, nextRouterID);

	if (SocketService::Instance().SendMessageToDst(protoMsg, dstRouterID)) {
		cout << "发送成功" << endl;
		return true;
	}
	else {
		cout << "发送失败，请检查网络状态" << endl;
		return false;
	}
}

/***************************************************************
函数功能：该函数用于向其他结点发送路由消息
函数输入： 线程参数
函数输出： 线程退出标志
函数依赖：  Message::Instance().CreateExchangeMessageWithOSPF
			Message::Instance().CreateExchangeMessageWithRIP
			RouterWithLS::Instance().GetRIPRouterTable
			RouterWithRIP::Instance().GetRIPRouterTable
			SocketService::Instance().SendMessageToDst
全局变量：  NumToAddrRouterMappingTable
Author: qyh
Date created: 18/11/21
Date finished: 18/11/21
***************************************************************/
bool ControlUnit::SendRouterInfoToOtherRouter()
{
	/*根据协议，准备向其他结点发送自己的路由信息*/
	if (ROUTER_PROTOCOL == OSPF) {
		for (int i = 0; i < RouterWithLS::Instance().GetOSPFRouterTable().size(); i++) {
			if (i == LOCAL_HOST_NUM)
				continue;

			// bug  ?? 
			char* msg = { 0 };
			if (RouterWithLS::Instance().GetOSPFRouterTable().at(LOCAL_HOST_NUM).at(i).linkState == false) {
				continue;
			}
			/*生成报文*/
			msg = Message::Instance().CreateOSPFMessage(&RouterWithLS::Instance().GetOSPFRouterTable(), i);
			if (SocketService::Instance().SendMessageToDst(msg, i)) {
				cout << "发送路由消息给其他结点" << endl;
				return true;
			}
			else {
				cout << "发送路由消息出错" << endl;
				return false;
			}

		}

	}
	else if (ROUTER_PROTOCOL == RIP) {

		for (int i = 0; i < RouterWithRIP::Instance().GetRIPRouterTable().size(); i++) {
			char* msg = { 0 };
			int dstRouterID = RouterWithRIP::Instance().GetRIPRouterTable().at(i).dstHostNum;
			if (!RouterWithRIP::Instance().GetRIPRouterTable().at(i).linkState) { /*TODO*/
				continue;
			}
			/*生成报文*/
			Message::Instance().CreateRIPMessage(&RouterWithRIP::Instance().GetRIPForwardingTable(), dstRouterID);
			if (SocketService::Instance().SendMessageToDst(msg, i)) {
				cout << "发送路由消息给其他结点" << endl;
				return true;
			}
			else {
				cout << "发送路由消息出错" << endl;
				return false;
			}
		}
	}
}


/*******************************************************************
函数功能：线程函数，用于接收消息，判断消息是应该转发还是更新路由消息
函数输入：线程必备的参数，实际未使用
函数输出：返回0
函数依赖：RouterWithLS::Instance().GetRIPRouterTable()
		  RouterWithLS::Instance().DecideForwardingAddress
		  RouterWithLS::Instance().GetRIPRouterTable
		  RouterWithRIP::Instance().ReceiveUpdatingRouterInfo
全局变量：NumToAddrRouterMappingTable  exitFlag
Author: qyh
Date created: 18/11/20
Date finished: 18/11/21
*********************************************************************/
DWORD __stdcall ControlUnit::StartReceivingThread(LPVOID lpParameter)
{
	while (1) {
		EnterCriticalSection(&Section);    /*进入临界区*/
		char* msg = { 0 };            /*等待报文*/
		msg = SocketService::Instance().ReceiveMessageFromSrc(msg);  /*等待，接收报文，将结果存在msg中*/
		Json::Value jsonBody = *Message::Instance().DecodeMessage(msg);

		if (jsonBody["Message_Type"] == FORWARDING_MESSAGE) {     /*接收到转发报文，下一步需要转发下一跳*/
			if (jsonBody["Router_Protocol"] == OSPF) {
				cout << "收到OSPF协议消息报文。消息来自" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl; /**************************/
				if (jsonBody["Dst_Addr"] == string(inet_ntoa(NumToAddrReceiveRouterMappingTable[LOCAL_HOST_NUM].sin_addr))) {
					cout << "收到OSPF协议消息报文。消息来自" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;
					cout << "消息内容是： " << jsonBody["Msg"] << endl;

					//TODO 报文回显
				}
				else {
					cout << "开始转发该报文" << endl;
					jsonBody["From_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_addr));
					jsonBody["From_Port"] = NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_port;
					jsonBody["From_Num"] = LOCAL_HOST_NUM;
					/*查询转发表,得到下一跳 IP PORT*/
					jsonBody["Next_Addr"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt())].sin_addr));
					jsonBody["Next_Port"] = NumToAddrReceiveRouterMappingTable[RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt)].sin_port;
					jsonBody["Cost"] = jsonBody["Cost"].asInt() + routerLinkTable[LOCAL_HOST_NUM][RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt())];
					/*发送报文*/

					//TODO 报文回显
					if (SocketService::Instance().SendMessageToDst(Message::Instance().EncodeMessage(&jsonBody), jsonBody["Dst_Num"].asInt())) {
						cout << "转发消息报文成功" << endl;
					}
					else {
						cout << "转发消息报文失败" << endl;
					}
				}

			}
			else if (jsonBody["Router_Protocol"] == RIP) {
				cout << "收到RIP协议消息报文。消息来自" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl;
				//TODO

				//TODO 报文回显
			}
			else {
				cout << "收到未知类型协议消息报文。消息来自" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl;

			}
		}
		else if (jsonBody["Message_Type"] == UPDATE_MESSAGE) { /*接收到路由更新报文*/
			if (jsonBody["Router_Protocol"] == OSPF) {
				vector<vector<RouterLink>> routerTable;
				for (int i = 0; i < (jsonBody["Router_Num"]).asInt(); i++) {
					vector<RouterLink> tmpTable;
					for (int j = 0; j < (jsonBody["Router_Num"]).asInt(); i++) {
						RouterLink rl;
						if (jsonBody["Info"][i][j]["timeStamp"].asInt() < RouterWithLS::Instance().GetOSPFRouterTable()[i][j].lastModified) {
							cout << "路由消息不是最新，不予更新" << endl;
							continue;
						}
						else {
							rl.localHostNum = jsonBody["info"][i][j]["Src_Num"].asInt();
							rl.dstHostNum = jsonBody["info"][i][j]["Dst_Num"].asInt();
							rl.linkCost = jsonBody["info"][i][j]["Cost"].asInt();
							rl.linkState = true;
							rl.lastModified = jsonBody["Info"][i][j]["timeStamp"].asInt();
							tmpTable.push_back(rl);
						}

					}
					routerTable.push_back(tmpTable);
				}
				if (RouterWithLS::Instance().ReceiveUpdatingRouterInfo(&routerTable)) {
					cout << "路由更新，消息来自" << " 路由信息来自" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;

				}
				else {
					cout << "路由消息不更新" << endl;
				}
			}
			else if (jsonBody["Router_Protocol"] == RIP) {
				vector<ForwardingRouterInfo> routerTable;
				for (int i = 0; i < (jsonBody["Router_Num"]).asInt(); i++) {
					ForwardingRouterInfo rl;
					rl.nextHostNum = jsonBody["info"][i]["Next_Num"].asInt();
					rl.targetedHostNum = jsonBody["info"][i]["Dst_Num"].asInt();
					rl.jumpNum = jsonBody["info"][i]["jumpNum"].asInt();
					routerTable.push_back(rl);
				}
				if (RouterWithRIP::Instance().ReceiveUpdatingRouterInfo(&routerTable)) {
					cout << "路由更新，消息来自" << " 路由信息来自" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;
				}
				else {
					cout << "路由消息不更新" << endl;
				}
			}
			else {
				cout << "收到未知报文" << endl;
			}
		}
		else {
			cout << "收到其他未知报文" << endl;
		}

		LeaveCriticalSection(&Section);   /*离开临界区*/

		if (exitFlag)
			return 0;
	}
	return 0;
}


/***************************************************************
函数功能：该函数用于周期性向其他结点发送路由消息,30s发送一次
函数输入： 线程参数
函数输出： 线程退出标志
函数依赖：  Message::Instance().CreateExchangeMessageWithOSPF
			Message::Instance().CreateExchangeMessageWithRIP
			RouterWithOSPF::Instance().GetRIPRouterTable
			RouterWithRIP::Instance().GetRIPRouterTable
			SocketService::Instance().SendMessageToDst
全局变量：NumToAddrRouterMappingTable exitFlag
Author: qyh
Date created: 18/11/21
Date finished: 18/11/21
***************************************************************/
DWORD __stdcall ControlUnit::StartSendRouterInfoThread(LPVOID lpParameter)
{
	while (1) {
		Sleep(30000);   /*30秒*/
		EnterCriticalSection(&Section);        /*进入临界区*/
		m_staticSelf->SendRouterInfoToOtherRouter();       /*向其他结点发送本地路由消息*/
		LeaveCriticalSection(&Section);  /*退出临界区*/
		if (exitFlag)
			return 0;
	}
}

/***************************************
函数功能：每隔180s用于检查那些老化的路由
函数输入：线程参数
函数输出：线程参数
函数依赖：RouterWithLS::Instance().CheckRouterTable(); RouterWithRIP::Instance().CheckRouterTable();
全局变量：exitFlag
Author: qyh
Date created: 18/11/21
Date finished: 18/11/22
****************************************/
DWORD __stdcall ControlUnit::StartCheckRouterInfoThread(LPVOID lpParameter)
{
	while (1) {
		Sleep(180000);  /*暂停180s*/
		EnterCriticalSection(&Section);        /*进入临界区*/

		if (ROUTER_PROTOCOL == RIP) {
			RouterWithLS::Instance().CheckRouterTable();
		}
		else if (ROUTER_PROTOCOL == OSPF) {
			RouterWithRIP::Instance().CheckRouterTable();
		}

		LeaveCriticalSection(&Section);        /*退出临界区*/
		if (exitFlag)
			return 0;
	}
}

/***************************
函数功能：该函数为程序的顶层入口。开启多个线程用于，接收消息，发送消息，更新消息。。。
Author: qyh
Date created: 18/11/21
Date finished:
*****************************/
bool ControlUnit::Run()
{
	cout << "***************************************" << endl << endl;
	cout << "Welcome to the Virtual Routing World ! " << endl;
	cout << "               .                       " << endl;
	cout << "__________2018-11-31___________________" << endl;

	//TODO
	InitRouterLinkTable();
	InitRouterMappingTable();

	if (InitSocket(IP, SEND_PORT, IP, RECEIVE_PORT)) {
		cout << "创建socket成功" << endl;
	}
	else {
		cout << "创建socket失败,程序退出" << endl;
		return 0;
	}

	/*创建三个线程*/
	//TODO
	InitializeCriticalSection(&Section);
	receiveMessageThread = ::CreateThread(NULL, 0, StartReceivingThread, NULL, 0, NULL);
	checkRouterThread = ::CreateThread(NULL, 0, StartCheckRouterInfoThread, NULL, 0, NULL);
	sendRouterMessageThread = ::CreateThread(NULL, 0, StartSendRouterInfoThread, NULL, 0, NULL);

	cout << "网路拓扑图如下" << endl;
	DrawRouterTable();

	while (1) {
		cout << "请输入以下序号选择对应的功能：" << endl;
		cout << "1. 发送消息到某一路由器。" << endl;
		cout << "2. 显示网路拓扑图 " << endl;
		cout << "3. 退出系统 " << endl;
		string choose;
		cin >> choose;

		if (choose == "1") {
			int routerID;
			cout << "输入你发送的目的路由器" << endl;
			cin >> routerID;
			string msg;
			cout << "输入你想要发送的内容" << endl;
			cin >> msg;
			if (SendMessageToOtherRouter(msg, routerID)) {
				cout << "发送成功" << endl;
			}
			else {
				cout << "发送失败" << endl;
			}
		}
		else if (choose == "2") {
			DrawRouterTable();
		}
		else if (choose == "3") {
			ExitSocket();
			exitFlag = 1;
		}
		else {
			cout << "请输入正确的序号" << endl;
		}

	}
	/*关闭子进程*/
	::CloseHandle(receiveMessageThread);
	::CloseHandle(checkRouterThread);
	::CloseHandle(sendRouterMessageThread);

	DeleteCriticalSection(&Section);  /*删除临界区对象*/
	cout << "退出程序" << endl;
	return 0;
}

