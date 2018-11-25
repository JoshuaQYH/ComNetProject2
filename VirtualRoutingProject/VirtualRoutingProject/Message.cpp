#include "Message.h"

/*******************************************************
�������ܣ� Դ����������Ϣ����
�������룺 Msg ��Ϣ����  DstRouterID Ŀ�ĵ�����ID  
		   nextRouterID ��һ������ID
��������� Э���ַ������������紫��
ȫ�ֱ�����NumToAddrSendRouterMappingTable  
		  NumToAddrReceiveRouterMappingTable
Author: qyh
Date created: 18/11/22
Date finished: 
*******************************************************/
char * Message::CreateMessage(string Msg, int srcRouterID, int dstRouterID, int nextRouterID)
{
	Json::Value JsonMsg;
	JsonMsg["Version"] = PROTOCOL_VERSION;
	JsonMsg["Message_Type"] = FORWARDING_MESSAGE; 
	JsonMsg["Router_Protocol"] = ROUTER_PROTOCOL;
	JsonMsg["Stamptime"] = (int)time(0);
	JsonMsg["Msg"] = Msg;

	JsonMsg["Src_Num"] = srcRouterID;
	JsonMsg["Src_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[srcRouterID].sin_addr));
	JsonMsg["Src_Port"] = NumToAddrSendRouterMappingTable[srcRouterID].sin_port;

	JsonMsg["Dst_Num"] = dstRouterID;
	JsonMsg["Dst_Num"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[dstRouterID].sin_addr));
	JsonMsg["Dst_Port"] = NumToAddrReceiveRouterMappingTable[dstRouterID].sin_port;

	JsonMsg["From_Num"] = srcRouterID;
	JsonMsg["From_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[srcRouterID].sin_addr));
	JsonMsg["From_Port"] = NumToAddrSendRouterMappingTable[srcRouterID].sin_port;

	JsonMsg["Next_Num"] = nextRouterID;
	JsonMsg["Next_Addr"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[nextRouterID].sin_addr));
	JsonMsg["Next_Port"] = NumToAddrReceiveRouterMappingTable[nextRouterID].sin_port;

	JsonMsg["Cost"] = routerLinkTable[srcRouterID][nextRouterID];

	Json::FastWriter styled_writer;

	string s = styled_writer.write(JsonMsg);

	char res[BUFF_SIZE] = { 0 };
	
	strcpy_s(res, s.c_str());

	return res;
}


/*******************************************************
�������ܣ� �յ�json���ģ������޸ģ�������Ϣת������
�������룺 jsonMsg ��Ϣ����json  fromRouterID ת������ID
		   nextRouterID  ������һ������·��ID
��������� Э���ַ������������紫��
ȫ�ֱ�����NumToAddrSendRouterMappingTable
		  NumToAddrReceiveRouterMappingTable
Author: qyh
Date created: 18/11/22
Date finished:
*******************************************************/
char * Message::CreateMessage(Json::Value * jsonMsg, int fromRouterID, int nextRouterID, int addCost)
{
	(*jsonMsg)["From_Num"] = fromRouterID;
	(*jsonMsg)["From_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[fromRouterID].sin_addr));
	(*jsonMsg)["From_Port"] = NumToAddrSendRouterMappingTable[fromRouterID].sin_port;

	(*jsonMsg)["Next_Num"] = nextRouterID;
	(*jsonMsg)["Next_Addr"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[fromRouterID].sin_addr));
	(*jsonMsg)["Next_Port"] = NumToAddrReceiveRouterMappingTable[fromRouterID].sin_port;

	(*jsonMsg)["Cost"] = (*jsonMsg)["Cost"].asInt() + addCost;

	Json::FastWriter styled_writer;

	string s = styled_writer.write(*jsonMsg);

	char res[BUFF_SIZE] = { 0 };

	strcpy_s(res, s.c_str());

	return res;
}



/************************************************************
�������ܣ� ����OSPFЭ���µı���·����Ϣ��׼�����ķ��͸��������
�������룺 routerTable ·�ɱ�
		   dstRouterID Ŀ������·��
��������� Э���ַ������������紫��
ȫ�ֱ�����NumToAddrSendRouterMappingTable
		  NumToAddrReceiveRouterMappingTable
Author: qyh
Date created: 18/11/22
Date finished:
*******************************************************/
char * Message::CreateOSPFMessage(vector<vector<RouterLink>>* routerTable, int dstRouterID)
{
	Json::Value JsonMsg;
	JsonMsg["Version"] = PROTOCOL_VERSION;
	JsonMsg["MessageType"] = UPDATE_MESSAGE;
	JsonMsg["Stamptime"] = (int)time(0);
	JsonMsg["Router_Protocol"] = OSPF;

	JsonMsg["Src_Num"] = LOCAL_HOST_NUM;
	JsonMsg["Src_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_addr));
	JsonMsg["Src_Port"] = NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_port;

	JsonMsg["Dst_Num"] = dstRouterID;
	JsonMsg["Dst_Num"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[dstRouterID].sin_addr));
	JsonMsg["Dst_Port"] = NumToAddrReceiveRouterMappingTable[dstRouterID].sin_port;

	JsonMsg["Cost"] = routerLinkTable[LOCAL_HOST_NUM][dstRouterID];

	JsonMsg["Router_Num"] = (int)(*routerTable).size();
	for (int i = 0; i < (*routerTable).size(); i++) {
		for (int j = 0; j < (*routerTable)[i].size(); j++) {
			JsonMsg["Router_Info"][i][j]["Src_Num"] = (*routerTable)[i][j].localHostNum;
			JsonMsg["Router_Info"][i][j]["Dst_Num"] = (*routerTable)[i][j].linkCost;
			JsonMsg["Router_Info"][i][j]["Timestamp"] = (int)(*routerTable)[i][j].lastModified;
		}
	}

	Json::FastWriter styled_writer;

	string s = styled_writer.write(JsonMsg);

	char res[BUFF_SIZE] = { 0 };

	strcpy_s(res, s.c_str());

	return res;

}

/************************************************************
�������ܣ� ����RIPЭ���µı���·����Ϣ��׼�����ķ��͸��������
�������룺 routerTable ·�ɱ�
		   dstRouterID Ŀ������·��
��������� Э���ַ������������紫��
ȫ�ֱ�����NumToAddrSendRouterMappingTable
		  NumToAddrReceiveRouterMappingTable
Author: qyh
Date created: 18/11/22
Date finished:
*******************************************************/
char * Message::CreateRIPMessage(vector<RouterLink>* routerTable, int dstRouterID)
{
	Json::Value JsonMsg;
	JsonMsg["Version"] = PROTOCOL_VERSION;
	JsonMsg["MessageType"] = UPDATE_MESSAGE;
	JsonMsg["Router_Protocol"] = RIP;
	JsonMsg["Stamptime"] = (int)time(0);

	JsonMsg["Src_Num"] = LOCAL_HOST_NUM;
	JsonMsg["Src_Addr"] = string(inet_ntoa(NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_addr));
	JsonMsg["Src_Port"] = NumToAddrSendRouterMappingTable[LOCAL_HOST_NUM].sin_port;

	JsonMsg["Dst_Num"] = dstRouterID;
	JsonMsg["Dst_Num"] = string(inet_ntoa(NumToAddrReceiveRouterMappingTable[dstRouterID].sin_addr));
	JsonMsg["Dst_Port"] = NumToAddrReceiveRouterMappingTable[dstRouterID].sin_port;

	JsonMsg["Cost"] = routerLinkTable[LOCAL_HOST_NUM][dstRouterID];

	JsonMsg["Router_Num"] = (int)(*routerTable).size();

	for (int i = 0; i < (*routerTable).size(); i++) {
		JsonMsg["Router_Info"][i]["Src_Num"] = (*routerTable)[i].localHostNum;
		JsonMsg["Router_Info"][i]["Dst_Num"] = (*routerTable)[i].linkCost;
		JsonMsg["Router_Info"][i]["Timestamp"] = (int)(*routerTable)[i].lastModified;

	}

	Json::FastWriter styled_writer;

	string s = styled_writer.write(JsonMsg);

	char res[BUFF_SIZE] = { 0 };

	strcpy_s(res, s.c_str());

	return res;
}

/*��json����תΪ�ַ���*/
char * Message::EncodeMessage(Json::Value * msg)
{
	Json::FastWriter styled_writer;

	string s = styled_writer.write(msg);

	char res[BUFF_SIZE] = { 0 };

	strcpy_s(res, s.c_str());

	return res;
}

/*���ַ���תΪjson����*/
Json::Value * Message::DecodeMessage(char * msg)
{
	Json::Reader reader;
	Json::Value root;

	reader.parse(msg, root);
	return &root;
}

/*�������Ը�ʽ����json�ַ������*/
void Message::Print(Json::Value msg)
{
	Json::StyledWriter styled_writer;
	std::cout << styled_writer.write(msg) << std::endl;
}
