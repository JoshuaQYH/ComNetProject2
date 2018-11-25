#include "ControlUnit.h"
#include "jsonCpp\reader.h" /*�˴���Ҫ�����Լ����ļ�·���޸�*/

int ControlUnit::exitFlag = 0;

/*��ʼ��socket*/
bool ControlUnit::InitSocket(string sendIP, int sendPort, string receiveIP, int receivePort)
{
	if (SocketService::Instance().InitSocket(sendIP, sendPort, receiveIP, receivePort)) {
		cout << "��ʼ��socket�����ɹ�,���Է��ͺͽ�����Ϣ��~~~" << endl;
		return true;
	}
	else {
		cout << "socket������ʼ��ʧ��,�����¼��������������~~" << endl;
		return false;
	}
}

/*�ر�socket*/
bool ControlUnit::ExitSocket()
{
	exitFlag = 1;
	if (SocketService::Instance().ExitSocket()) {
		cout << "�ر�socket�ɹ�~" << endl;
		return true;
	}
	else {
		cout << "�ر�socketʧ��" << endl;
		return false;
	}
	return false;
}

/*����������Ϣ����*/
bool ControlUnit::SendMessageToOtherRouter(string msg, int dstRouterID)
{
	int nextRouterID = 0;
	if (ROUTER_PROTOCOL == OSPF)
		nextRouterID = RouterWithLS::Instance().DecideForwardingAddress(dstRouterID);
	else if (ROUTER_PROTOCOL == RIP)
		nextRouterID = RouterWithRIP::Instance().DecideForwardingAddress(dstRouterID);

	char*  protoMsg = Message::Instance().CreateMessage(msg, LOCAL_HOST_NUM, dstRouterID, nextRouterID);

	if (SocketService::Instance().SendMessageToDst(protoMsg, dstRouterID)) {
		cout << "���ͳɹ�" << endl;
		return true;
	}
	else {
		cout << "����ʧ�ܣ���������״̬" << endl;
		return false;
	}
}

/***************************************************************
�������ܣ��ú���������������㷢��·����Ϣ
�������룺 �̲߳���
��������� �߳��˳���־
����������  Message::Instance().CreateExchangeMessageWithOSPF
			Message::Instance().CreateExchangeMessageWithRIP
			RouterWithLS::Instance().GetRIPRouterTable
			RouterWithRIP::Instance().GetRIPRouterTable
			SocketService::Instance().SendMessageToDst
ȫ�ֱ�����  NumToAddrRouterMappingTable
Author: qyh
Date created: 18/11/21
Date finished: 18/11/21
***************************************************************/
bool ControlUnit::SendRouterInfoToOtherRouter()
{
	/*����Э�飬׼����������㷢���Լ���·����Ϣ*/
	if (ROUTER_PROTOCOL == OSPF) {
		for (int i = 0; i < RouterWithLS::Instance().GetOSPFRouterTable().size(); i++) {
			if (i == LOCAL_HOST_NUM)
				continue;

			// bug  ?? 
			char* msg = { 0 };
			if (RouterWithLS::Instance().GetOSPFRouterTable().at(LOCAL_HOST_NUM).at(i).linkState == false) {
				continue;
			}
			/*���ɱ���*/
			msg = Message::Instance().CreateOSPFMessage(&RouterWithLS::Instance().GetOSPFRouterTable(), i);
			if (SocketService::Instance().SendMessageToDst(msg, i)) {
				cout << "����·����Ϣ���������" << endl;
				return true;
			}
			else {
				cout << "����·����Ϣ����" << endl;
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
			/*���ɱ���*/
			Message::Instance().CreateRIPMessage(&RouterWithRIP::Instance().GetRIPForwardingTable(), dstRouterID);
			if (SocketService::Instance().SendMessageToDst(msg, i)) {
				cout << "����·����Ϣ���������" << endl;
				return true;
			}
			else {
				cout << "����·����Ϣ����" << endl;
				return false;
			}
		}
	}
}


/*******************************************************************
�������ܣ��̺߳��������ڽ�����Ϣ���ж���Ϣ��Ӧ��ת�����Ǹ���·����Ϣ
�������룺�̱߳ر��Ĳ�����ʵ��δʹ��
�������������0
����������RouterWithLS::Instance().GetRIPRouterTable()
		  RouterWithLS::Instance().DecideForwardingAddress
		  RouterWithLS::Instance().GetRIPRouterTable
		  RouterWithRIP::Instance().ReceiveUpdatingRouterInfo
ȫ�ֱ�����NumToAddrRouterMappingTable  exitFlag
Author: qyh
Date created: 18/11/20
Date finished: 18/11/21
*********************************************************************/
DWORD __stdcall ControlUnit::StartReceivingThread(LPVOID lpParameter)
{
	while (1) {
		EnterCriticalSection(&Section);    /*�����ٽ���*/
		char* msg = { 0 };            /*�ȴ�����*/
		msg = SocketService::Instance().ReceiveMessageFromSrc(msg);  /*�ȴ������ձ��ģ����������msg��*/
		Json::Value jsonBody = *Message::Instance().DecodeMessage(msg);

		if (jsonBody["Message_Type"] == FORWARDING_MESSAGE) {     /*���յ�ת�����ģ���һ����Ҫת����һ��*/
			if (jsonBody["Router_Protocol"] == OSPF) {
				cout << "�յ�OSPFЭ����Ϣ���ġ���Ϣ����" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl; /**************************/
				if (jsonBody["Dst_Addr"] == string(inet_ntoa(NumToAddrReceiveRouterMappingTable[LOCAL_HOST_NUM].sin_addr))) {
					cout << "�յ�OSPFЭ����Ϣ���ġ���Ϣ����" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;
					cout << "��Ϣ�����ǣ� " << jsonBody["Msg"] << endl;

					//TODO ���Ļ���
				}
				else {
					cout << "��ʼת���ñ���" << endl;
					jsonBody["From_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_addr));
					jsonBody["From_Port"] = NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_port;
					jsonBody["From_Num"] = LOCAL_HOST_NUM;
					/*��ѯת����,�õ���һ�� IP PORT*/
					jsonBody["Next_Addr"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt())].sin_addr));
					jsonBody["Next_Port"] = NumToAddrReceiveRouterMappingTable[RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt)].sin_port;
					jsonBody["Cost"] = jsonBody["Cost"].asInt() + routerLinkTable[LOCAL_HOST_NUM][RouterWithLS::Instance().DecideForwardingAddress(jsonBody["Dst_Num"].asInt())];
					/*���ͱ���*/

					//TODO ���Ļ���
					if (SocketService::Instance().SendMessageToDst(Message::Instance().EncodeMessage(&jsonBody), jsonBody["Dst_Num"].asInt())) {
						cout << "ת����Ϣ���ĳɹ�" << endl;
					}
					else {
						cout << "ת����Ϣ����ʧ��" << endl;
					}
				}

			}
			else if (jsonBody["Router_Protocol"] == RIP) {
				cout << "�յ�RIPЭ����Ϣ���ġ���Ϣ����" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl;
				//TODO

				//TODO ���Ļ���
			}
			else {
				cout << "�յ�δ֪����Э����Ϣ���ġ���Ϣ����" << jsonBody["From_Addr"] << ":" << jsonBody["From_Port"] << endl;

			}
		}
		else if (jsonBody["Message_Type"] == UPDATE_MESSAGE) { /*���յ�·�ɸ��±���*/
			if (jsonBody["Router_Protocol"] == OSPF) {
				vector<vector<RouterLink>> routerTable;
				for (int i = 0; i < (jsonBody["Router_Num"]).asInt(); i++) {
					vector<RouterLink> tmpTable;
					for (int j = 0; j < (jsonBody["Router_Num"]).asInt(); i++) {
						RouterLink rl;
						if (jsonBody["Info"][i][j]["timeStamp"].asInt() < RouterWithLS::Instance().GetOSPFRouterTable()[i][j].lastModified) {
							cout << "·����Ϣ�������£��������" << endl;
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
					cout << "·�ɸ��£���Ϣ����" << " ·����Ϣ����" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;

				}
				else {
					cout << "·����Ϣ������" << endl;
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
					cout << "·�ɸ��£���Ϣ����" << " ·����Ϣ����" << jsonBody["Src_Addr"] << ":" << jsonBody["Src_Port"] << endl;
				}
				else {
					cout << "·����Ϣ������" << endl;
				}
			}
			else {
				cout << "�յ�δ֪����" << endl;
			}
		}
		else {
			cout << "�յ�����δ֪����" << endl;
		}

		LeaveCriticalSection(&Section);   /*�뿪�ٽ���*/

		if (exitFlag)
			return 0;
	}
	return 0;
}


/***************************************************************
�������ܣ��ú���������������������㷢��·����Ϣ,30s����һ��
�������룺 �̲߳���
��������� �߳��˳���־
����������  Message::Instance().CreateExchangeMessageWithOSPF
			Message::Instance().CreateExchangeMessageWithRIP
			RouterWithOSPF::Instance().GetRIPRouterTable
			RouterWithRIP::Instance().GetRIPRouterTable
			SocketService::Instance().SendMessageToDst
ȫ�ֱ�����NumToAddrRouterMappingTable exitFlag
Author: qyh
Date created: 18/11/21
Date finished: 18/11/21
***************************************************************/
DWORD __stdcall ControlUnit::StartSendRouterInfoThread(LPVOID lpParameter)
{
	while (1) {
		Sleep(30000);   /*30��*/
		EnterCriticalSection(&Section);        /*�����ٽ���*/
		m_staticSelf->SendRouterInfoToOtherRouter();       /*��������㷢�ͱ���·����Ϣ*/
		LeaveCriticalSection(&Section);  /*�˳��ٽ���*/
		if (exitFlag)
			return 0;
	}
}

/***************************************
�������ܣ�ÿ��180s���ڼ����Щ�ϻ���·��
�������룺�̲߳���
����������̲߳���
����������RouterWithLS::Instance().CheckRouterTable(); RouterWithRIP::Instance().CheckRouterTable();
ȫ�ֱ�����exitFlag
Author: qyh
Date created: 18/11/21
Date finished: 18/11/22
****************************************/
DWORD __stdcall ControlUnit::StartCheckRouterInfoThread(LPVOID lpParameter)
{
	while (1) {
		Sleep(180000);  /*��ͣ180s*/
		EnterCriticalSection(&Section);        /*�����ٽ���*/

		if (ROUTER_PROTOCOL == RIP) {
			RouterWithLS::Instance().CheckRouterTable();
		}
		else if (ROUTER_PROTOCOL == OSPF) {
			RouterWithRIP::Instance().CheckRouterTable();
		}

		LeaveCriticalSection(&Section);        /*�˳��ٽ���*/
		if (exitFlag)
			return 0;
	}
}

/***************************
�������ܣ��ú���Ϊ����Ķ�����ڡ���������߳����ڣ�������Ϣ��������Ϣ��������Ϣ������
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
		cout << "����socket�ɹ�" << endl;
	}
	else {
		cout << "����socketʧ��,�����˳�" << endl;
		return 0;
	}

	/*���������߳�*/
	//TODO
	InitializeCriticalSection(&Section);
	receiveMessageThread = ::CreateThread(NULL, 0, StartReceivingThread, NULL, 0, NULL);
	checkRouterThread = ::CreateThread(NULL, 0, StartCheckRouterInfoThread, NULL, 0, NULL);
	sendRouterMessageThread = ::CreateThread(NULL, 0, StartSendRouterInfoThread, NULL, 0, NULL);

	cout << "��·����ͼ����" << endl;
	DrawRouterTable();

	while (1) {
		cout << "�������������ѡ���Ӧ�Ĺ��ܣ�" << endl;
		cout << "1. ������Ϣ��ĳһ·������" << endl;
		cout << "2. ��ʾ��·����ͼ " << endl;
		cout << "3. �˳�ϵͳ " << endl;
		string choose;
		cin >> choose;

		if (choose == "1") {
			int routerID;
			cout << "�����㷢�͵�Ŀ��·����" << endl;
			cin >> routerID;
			string msg;
			cout << "��������Ҫ���͵�����" << endl;
			cin >> msg;
			if (SendMessageToOtherRouter(msg, routerID)) {
				cout << "���ͳɹ�" << endl;
			}
			else {
				cout << "����ʧ��" << endl;
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
			cout << "��������ȷ�����" << endl;
		}

	}
	/*�ر��ӽ���*/
	::CloseHandle(receiveMessageThread);
	::CloseHandle(checkRouterThread);
	::CloseHandle(sendRouterMessageThread);

	DeleteCriticalSection(&Section);  /*ɾ���ٽ�������*/
	cout << "�˳�����" << endl;
	return 0;
}

