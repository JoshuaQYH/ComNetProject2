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
�������: ����Э��ͷ + �䳤jsonЭ����

Э��ͷHeader������server�ֶΣ����������޸�
	server�ֶ���2������:
	0  ���� RIP
	1  ���� OSPF

	ע����Դ�ڵ㷢�ͱ��ľ������·��ʱ��ÿ����һ��·�ɣ���·�ɾ�Ҫ���ͻ��Ա��ģ���֪Դ�ڵ㣬��Ϣ���������
	    ����·���Ծ���Ҫ�������·�ɣ�������ֱ�ӷ��͸�Դ�ڵ㡣
	
Э����body�� 
	json�м������ݶΣ�
	�������� type  (int)��

	1  �����Դ�ڵ㿪ʼ���͵���Ϣ����  ����ϢҪ�������·�ɣ�
	2  ����ת��Դ�ڵ����Ϣ����
	3  ����ڵ�֮�佻��·����Ϣ�ı���  ����Ϣ��Ե㣩

	����������ֱ����������²�ͬ���ֶ�
	1 2 Src_Addr: ���ķ���Դ��ַ  Dst_Addr: ���ķ���Ŀ�ĵ�ַ Next_Addr: ��һ����ַ From_Addr: �����ͽڵ��ַ  Msg: ��Ϣ Extra_Data:�������Ϣ
	3   Src_Addr: ���ķ���Դ��ַ  Dst_Addr: ���ķ���Ŀ�ĵ�ַ  Msg: ������·����Ϣ Extra_Data: �������Ϣ����Ϊ��

	Msg:
	1 2  ��Ϣ�����������ֱ���� string ���� ������Ϣ
	3 ���������ڵ������� ���顣�����RIP����һά���飬�����OSPF���Ƕ�ά���顣 
	
***************************************************************************************/

/*
	MyProto�����
 */
class MyProtoEnCode
{
public:
	//Э����Ϣ��������
	uint8_t * encode(MyProtoMsg * pMsg, uint32_t & len);
private:
	//Э��ͷ�������
	void headEncode(uint8_t * pData, MyProtoMsg * pMsg);
};


/*
	MyProto�����
 */
class MyProtoDeCode
{
public:
	void init();    
	void clear();
	bool parser(void * data, size_t len);
	bool empty();
	MyProtoMsg * front();
	void pop();
private:
	bool parserHead(uint8_t ** curData, uint32_t & curLen,
		uint32_t & parserLen, bool & parserBreak);
	bool parserBody(uint8_t ** curData, uint32_t & curLen,
		uint32_t & parserLen, bool & parserBreak);

private:
	MyProtoMsg mCurMsg;                     //��ǰ�����е�Э����Ϣ��
	queue<MyProtoMsg *> mMsgQ;              //�����õ�Э����Ϣ����
	vector<uint8_t> mCurReserved;           //δ�����������ֽ���
	MyProtoParserStatus mCurParserStatus;   //��ǰ����״̬
};

/*������ͽ�����Լ����ɱ��ļ��ɵ�һ������*/
class Message
{
private:
	Message();                             // ctor hidden
	Message(Message const&);               // copy ctor hidden
	Message& operator=(Message const&);    // assign op. hidden
	~Message();                            // dtor hidden
	MyProtoDeCode decoder;             /*������*/
	MyProtoEnCode encoder;             /*������*/

public:
	// ����ģʽ
	static Message& Instance() {
		static Message theMessage;  /*ʹ�÷��� Message::Instance()�õ�����*/
		return theMessage;
	}

	//TODO  ��Ҫ�޸��������������ֵ
	
	/*����һ�ݱ���*/
	void CreateMessage();

	/*�ı��ַ���תΪ�����ֽ��������ڴ���*/
	void EncodeMessage();

	/*�������ֽ���תΪ�ı��ַ�*/
	void DecodeMessage();

};

#endif // ! _MESSAGE_H_
