#include "Message.h"

/*******************************************************
函数功能： 源主机生成消息报文
函数输入： Msg 消息内容  DstRouterID 目的地主机ID  
		   nextRouterID 下一跳主机ID
函数输出： 协议字符串，用于网络传输
全局变量：NumToAddrSendRouterMappingTable  
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
函数功能： 收到json报文，进行修改，生成消息转发报文
函数输入： jsonMsg 消息报文json  fromRouterID 转发主机ID
		   nextRouterID  生成下一跳主机路由ID
函数输出： 协议字符串，用于网络传输
全局变量：NumToAddrSendRouterMappingTable
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
函数功能： 接收OSPF协议下的本机路由信息，准备报文发送给其他结点
函数输入： routerTable 路由表
		   dstRouterID 目的主机路由
函数输出： 协议字符串，用于网络传输
全局变量：NumToAddrSendRouterMappingTable
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
函数功能： 接收RIP协议下的本机路由信息，准备报文发送给其他结点
函数输入： routerTable 路由表
		   dstRouterID 目的主机路由
函数输出： 协议字符串，用于网络传输
全局变量：NumToAddrSendRouterMappingTable
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

/*将json对象转为字符串*/
char * Message::EncodeMessage(Json::Value * msg)
{
	Json::FastWriter styled_writer;

	string s = styled_writer.write(msg);

	char res[BUFF_SIZE] = { 0 };

	strcpy_s(res, s.c_str());

	return res;
}

/*将字符串转为json对象*/
Json::Value * Message::DecodeMessage(char * msg)
{
	Json::Reader reader;
	Json::Value root;

	reader.parse(msg, root);
	return &root;
}

/*将报文以格式化的json字符串输出*/
void Message::Print(Json::Value msg)
{
	Json::StyledWriter styled_writer;
	std::cout << styled_writer.write(msg) << std::endl;
}
