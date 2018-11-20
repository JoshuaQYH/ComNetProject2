#pragma once
/*****************************************************

Author: qiuyihao

Date created: 18/11/18

Date finished: 

Description�����һЩȫ�ֳ�����������ͨ�õĺ���

*******************************************************/
#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <winsock.h>  // �׽�������ͷ�ļ�
#pragma comment(lib, "WSOCK32.LIB")
#include <iostream>
#include<stdlib.h>
#include<vector>
#include<map>
#include<time.h>
#include<algorithm>
#include<fstream>
#include<time.h>
#include "CImg.h"
#include "RouterWithOSPF.h"
#include "RouterWithRIP.h"
#include "SocketService.h"
#include "Message.h"
#include <windows.data.json.h>
#include "json\json.h"
#include <queue>

using namespace cimg_library;
using namespace std;

/*******************ȫ�ֳ���************************************/

#define SOCK_VER 2                  /*����ʹ�õ�winsock���汾*/

#define CILENT_IP "127.0.0.1"       /*���ͻ��˵�IP��ַ*/

#define CLIENT_RECEIVE_PORT "3333"  /*�ͻ��˵Ľ��ն˿�*/

#define CLIENT_SEND_PORT "3332"     /*�ͻ��˷��Ͷ˿�*/
 
#define LOCAL_HOST_NUM  1           /*����������ţ����費�ɱ���Ψһ*/

#define BUFF_SIZE 2048              /*�շ���Ϣ�Ļ�������С*/

#define INFINITY_COST 55555         /*�������ڵ㲻��������ô����Ϊ��ֵ*/
 
#define MAX_ROUTER_NODES  100       /*���·�ɽڵ�*/



/****************ȫ��ͨ�����ݽṹ********************************/

/************
·�����ӽṹ
*************/
struct RouterLink{
	int localHostNum;           /* �����������          */
	int dstHostNum;             /* Ŀ���������          */
	int linkCost;               /* ��·����              */
	time_t lastModified;        /* ��·״̬�ϴθ���ʱ���*/    
};


/********
ת������
*********/
struct ForwardingRouterInfo {
	int targetedHostNum;           /* Ŀ���������     */
	int nextHostNum;               /* ��һ���������   */
	int jumpNum;                   /* ��Ŀ������������ */
}; 


const uint8_t MY_PROTO_MAGIC = 88;                   /*Э��ħ����*/
const uint32_t MY_PROTO_MAX_SIZE = 10 * 1024 * 1024; /*Э������Ϊ10M*/
const uint32_t MY_PROTO_HEAD_SIZE = 8;               /*Э��ͷ��СΪ8�ֽ�*/



/* Э��״̬ */
typedef enum MyProtoParserStatus
{
	ON_PARSER_INIT = 0,
	ON_PARSER_HAED = 1,
	ON_PARSER_BODY = 2,
}MyProtoParserStatus;

/*
	Э��ͷ�ṹ�����
 */
struct MyProtoHead
{
	uint8_t version;    //Э��汾��
	uint8_t magic;      //Э��ħ��
	uint16_t server;    //Э�鸴�õķ���ţ���ʶЭ��֮�ϵĲ�ͬ����
	uint32_t len;       //Э�鳤�ȣ�Э��ͷ����+�䳤jsonЭ���峤�ȣ�
};

/*
	Э����Ϣ��
 */
struct MyProtoMsg
{
	MyProtoHead head;   //Э��ͷ
	Json::Value body;   //Э����,ʹ��json�洢
};





/**************************ȫ�ֱ���********************************/

/*����: ӳ�������������ţ���ö�Ӧ��IP+�˿ڡ�
  ��ע������������ͼ��ƺ�֮�󣬻������ֲ��䣬
        ��ĳһ�ڵ���ֹ���֮�󣬿���ɾ������*/
map<int, sockaddr_in> NumToAddrRouterMappingTable;


/*���ܣ���ʼ��ȫ��·�ɱ�, �����±����������ţ�ֵ��������֮��ķ��á�
  ��ע��OSPFӵ�иñ�ȫ����Ϣ����RIP��ӵ�иñ���������ڵ����ڵ���Ϣ��
        ������ʼ��·����Ϣ¼���������·����Ϣ������������и��¡�
*/
int routerLinkTable[MAX_ROUTER_NODES][MAX_ROUTER_NODES];



/*******************ȫ��ͨ�ú���**************************************/ 
/********************************************
���ܣ�����ʼ·����Ϣ¼��ȫ��·����Ϣ�����
Author: ���Ⱥ� 
Date created: 18/11/19
Date finished: 
**********************************************/
void InitRouterLinkTable() {
	// TODO
}


/*********************************************
���ܣ���������ź�IP��ַ��Ϣ¼�뵽ȫ��ӳ�����
Author: ���Ⱥ�
Date created: 18/11/19
Date finished: 
**********************************************/
void InitRouterMappingTable() {
	// TODO
}

// TODO
/**************
���ܣ�������·����ͼ
���룺
�����
������
Author: ���³�
Date created:
***************/
void DrawRouterTable() {
	// TODO
}


#endif
