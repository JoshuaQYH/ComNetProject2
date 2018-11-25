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
#include "RouterWithLS.h"
#include "RouterWithRIP.h"
#include "SocketService.h"
#include "Message.h"
#include <windows.data.json.h>
#include "json\json.h"
#include <queue>

using namespace cimg_library;
using namespace std;

/*******************ȫ�ֳ���************************************/
#define OSPF 1                      /* OSPF ·��ѡ��Э��*/

#define RIP  2                      /*RIP ·��ѡ��Э��*/

#define ROUTER_PROTOCOL OSPF         /*������õ�·��ѡ��Э��*/  

#define SOCK_VER 2                  /*����ʹ�õ�winsock���汾*/

#define IP "127.0.0.1"              /*���ͻ��˵�IP��ַ*/

#define RECEIVE_PORT 3333           /*�ͻ��˵Ľ��ն˿�*/

#define SEND_PORT 3332              /*�ͻ��˷��Ͷ˿�*/
 
#define LOCAL_HOST_NUM  1           /*����������ţ����費�ɱ���Ψһ*/

#define BUFF_SIZE 2048              /*�շ���Ϣ�Ļ�������С*/

#define INFINITY_COST 55555         /*�������ڵ㲻��������ô����Ϊ��ֵ*/
 
#define MAX_ROUTER_NODES  100       /*���·�ɽڵ�*/

#define DEBUG 1                     /* Ϊ1ʱ��ӡ��������Ϣ*/

#define PROTOCOL_VERSION  "1.0"     /*���İ汾��*/

#define FORWARDING_MESSAGE 0       /*������Ϣ����*/

#define UPDATE_MESSAGE 1           /*������±���*/
/****************ȫ��ͨ�����ݽṹ********************************/

/************
·�����ӽṹ
*************/
struct RouterLink{
	int localHostNum;           /* �����������          */
	int dstHostNum;             /* Ŀ���������          */
	int linkCost;               /* ��·����              */
	bool linkState;             /* Ϊ�����������������  */
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


/**************************ȫ�ֱ���********************************/

/*����: ӳ�������������ţ���ö�Ӧ��IP+�����ա��˿ڡ�
  ��ע������������ͼ��ƺ�֮�󣬻������ֲ��䣬
        ��ĳһ�ڵ���ֹ���֮�󣬿���ɾ������*/
map<int, sockaddr_in> NumToAddrReceiveRouterMappingTable;


/*����: ӳ�������������ţ���ö�Ӧ��IP+�����͡��˿ڡ�
  ��ע������������ͼ��ƺ�֮�󣬻������ֲ��䣬
		��ĳһ�ڵ���ֹ���֮�󣬿���ɾ������*/
map<int, sockaddr_in> NumToAddrSendRouterMappingTable;



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
	int Link[5][5] = {{INFINITY_COST,INFINITY_COST, 10, 4, 3},
					  {INFINITY_COST, INFINITY_COST, INFINITY_COST, 7, 9},
				      {1, INFINITY_COST, INFINITY_COST,6,INFINITY_COST},
					  {4,7,6,INFINITY_COST,INFINITY_COST},
	                  {3, 9,INFINITY_COST,INFINITY_COST, INFINITY_COST},
					};

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			routerLinkTable[i][j] = Link[i][j];
		}
	}
}


/*********************************************
���ܣ���������ź�IP��ַ��Ϣ¼�뵽ȫ��ӳ�����
Author: ���Ⱥ�
Date created: 18/11/19
Date finished: 
**********************************************/
void InitRouterMappingTable() {
	// TODO
	string IPTable[5] = { "127.0.0.1","127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1" };
	int sendPortTable[5] = { 3330,3331,3332,3333,3334 };
	int receivePortTable[5] = { 3220,3221,3222,3223,3224 };

	for (int i = 0; i < 5; i++) {
		sockaddr_in sendAddr = { 0 };
		sendAddr.sin_family = AF_INET;
		sendAddr.sin_port = htons(sendPortTable[i]);
		sendAddr.sin_addr.S_un.S_addr = inet_addr(IPTable[i].c_str());
		NumToAddrSendRouterMappingTable.insert(make_pair(i, sendAddr));

		sockaddr_in receiveAddr = { 0 };
		receiveAddr.sin_family = AF_INET;
		receiveAddr.sin_port = htons(receivePortTable[i]);
		receiveAddr.sin_addr.S_un.S_addr = inet_addr(IPTable[i].c_str());
		NumToAddrReceiveRouterMappingTable.insert(make_pair(i, receiveAddr));
	}
}

// TODO
/**************
���ܣ�������·����ͼ
���룺
�����
������
Author: qyh
Date created:
***************/
void DrawRouterTable() {
	// TODO

}


#endif
