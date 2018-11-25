#pragma once
#ifndef  _MESSAGE_H_
#define _MESSAGE_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: ʹ�ýṹ���װ��ͨ�ű����е��ֶΣ������˱���ͽ���
			 �Ĺ��ܣ��������紫��ͽ������ݡ�

****************************************************/


/*

�󲿷��հ���ͷ����Դ�룬�Զ���Ӧ�ò�Э�飬
����������ֽ������Լ��������ֽ�������ɱ����ַ���
https://segmentfault.com/a/1190000008740863#articleHeader6

*/

/************************************************************************************
Э�鱨�����: �䳤json�ṹ��

ת����Ϣ����json���� �����ת��������
{
	Version:  ....  Э��汾��
	Router_Protocl:.ospf �� RIP
	Message_Type:...Ϊ1 ��ʾת������Ϣ���� int
	Src_Addr: ....  ��Ϣ����Դ������ַ string
	Src_Port: ....  ��Ϣ����Դ�������Ͷ˿� int
	Src_Num: .....  ��Ϣ����Դ����ID int
	Dst_Addr: ....  ��Ϣ����Ŀ��������ַ
	Dst_Port: ....  ��Ϣ����Ŀ���������ն˿�
	Dst_Num:  ....  ��Ϣ����Ŀ������ID
	Next_Addr: .... ��Ϣ������һ��������ַ
	Next_Port: .... ��Ϣ������һ���������ն˿�
	Next_Num: ....  ��Ϣ������һ������ID
	From_Addr: .... ��ǰ��Ϣ���ĵķ�����Դ������ַ
	From_Port: .... ��ǰ��Ϣ���ĵķ�����Դ�������Ͷ˿�
	From_Num: ....  ��ǰ��Ϣ���ĵķ�����Դ����ID
	Cost:    .....  ÿ����һ����㣬��Ҫ������·�ܴ���  int
	Msg: .......... ��Ϣ����Я���ı�����Ϣ   string 
	Stamptime: .... ��Ϣ�����������ʱ���   int
	// src dst �ֶ�һ��ȷ�������ٱ��
	// next  from ����ת�����̽����޸�
	// ��������ķ�����������Ҫ�ر�ָ��������Ϣ������ʱ�������·����
	// ����ת����������Ҫ������·���ۡ�
}

OSPF·����Ϣ�������ģ�һ�δ�����
{
	Version:  ....  Э��汾��
	Src_Addr: ....  ��Ϣ����Դ������ַ
	Src_Port: ....  ��Ϣ����Դ�������Ͷ˿�
	Src_Num: .....  ��Ϣ����Դ����ID
	Dst_Addr: ....  ��Ϣ����Ŀ��������ַ
	Dst_Port: ....  ��Ϣ����Ŀ���������ն˿�
	Cost: ........  ��·����
	Message_Type:  ....... Ϊ0 ��ʾ���±��� 
	Router_Protocol: ...... Ϊ OSPF
	Router_Num: ·����Ϣ�漰�ĵĽ�㣬��ά�����ĳ��ȣ���ȣ�
	Router_Info:[
			[
				[Src: Src_Num:  Dst: Dst_Num:  Cost:  Timestamp: ]   int int int time_t // ��ά����
			],
			[
				[Src: Src_Num:  Dst: Dst_Num: Cost: Timestamp: ]
			]
		 ] ....·����Ϣ
}


RIP·����Ϣ�������ģ�һ�δ�����
{
	Version:  ....  Э��汾��
	Src_Addr: ....  ��Ϣ����Դ������ַ
	Src_Port: ....  ��Ϣ����Դ�������Ͷ˿�
	Src_Num: .....  ��Ϣ����Դ����ID
	Dst_Addr: ....  ��Ϣ����Ŀ��������ַ
	Dst_Port: ....  ��Ϣ����Ŀ���������ն˿�
	Cost: ........  ��·����
	Message_Type:  .......  Ϊ0��ʾ���±���
	Router_Protocol: ...... Ϊ rip
	Router_Num: ·����Ϣ�漰�ĵĽ�㣬һά�����ĳ��ȣ���ȣ�
	Router_Info:[
			[Src:  Src_Num: Dst: Dst_Num: Cost: Timestamp: ],             // һά����
			[Src: Src_Num: Dst: Dst_Num: Cost: Timestamp: ].
		 ] ....·����Ϣ
}

/*������ͽ�����Լ����ɱ��ļ��ɵ�һ������*/
class Message
{
private:
	Message();                             // ctor hidden
	Message(Message const&);               // copy ctor hidden
	Message& operator=(Message const&);    // assign op. hidden
	~Message();                            // dtor hidden
	

public:
	// ����ģʽ
	static Message& Instance() {
		static Message theMessage;  /*ʹ�÷��� Message::Instance()�õ�����*/
		return theMessage;
	}

	/*����һ����Ϣ����*/
	char* CreateMessage(string Msg,int srcRouterID, int dstRouterID, int nextRouterID);

	/*����һ��ת������Ϣ����*/
	char* CreateMessage(Json::Value* jsonMsg, int fromRouterID, int nextRouterID, int addCost);

	/*ΪospfЭ�鴴��·����Ϣ��������*/
	char* CreateOSPFMessage(vector<vector<RouterLink>>* routerTable, int dstRouterID);

	/*ΪRIPЭ�鴴��·����Ϣ��������*/
	char* CreateRIPMessage(vector<RouterLink>* routerTable, int dstRouterID);

	/*��json�ṹ����ת��Ϊchar�ַ���ָ�룬�������紫��*/
	char* EncodeMessage(Json::Value* msg);

	/*���ַ���ָ��תΪjson�������ڽ�������*/
	Json::Value* DecodeMessage(char* msg);

	/*��ӡ����*/
	void Print(Json::Value msg);


};

#endif // ! _MESSAGE_H_
