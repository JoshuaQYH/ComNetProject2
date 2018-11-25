#pragma once
/*****************************************************

Author: qiuyihao

Date created: 18/11/18

Date finished: 

Description：存放一些全局常量、变量、通用的函数

*******************************************************/
#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <winsock.h>  // 套接字所需头文件
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

/*******************全局常量************************************/
#define OSPF 1                      /* OSPF 路由选择协议*/

#define RIP  2                      /*RIP 路由选择协议*/

#define ROUTER_PROTOCOL OSPF         /*定义采用的路由选择协议*/  

#define SOCK_VER 2                  /*程序使用的winsock主版本*/

#define IP "127.0.0.1"              /*本客户端的IP地址*/

#define RECEIVE_PORT 3333           /*客户端的接收端口*/

#define SEND_PORT 3332              /*客户端发送端口*/
 
#define LOCAL_HOST_NUM  1           /*本地主机序号，假设不可变且唯一*/

#define BUFF_SIZE 2048              /*收发消息的缓冲区大小*/

#define INFINITY_COST 55555         /*若两个节点不相连，那么设置为该值*/
 
#define MAX_ROUTER_NODES  100       /*最大路由节点*/

#define DEBUG 1                     /* 为1时打印出进程消息*/

#define PROTOCOL_VERSION  "1.0"     /*报文版本号*/

#define FORWARDING_MESSAGE 0       /*代表消息报文*/

#define UPDATE_MESSAGE 1           /*代表更新报文*/
/****************全局通用数据结构********************************/

/************
路由连接结构
*************/
struct RouterLink{
	int localHostNum;           /* 本地主机序号          */
	int dstHostNum;             /* 目的主机序号          */
	int linkCost;               /* 链路代价              */
	bool linkState;             /* 为真代表网络连接正常  */
	time_t lastModified;        /* 链路状态上次更新时间戳*/    
};


/********
转发表项
*********/
struct ForwardingRouterInfo {
	int targetedHostNum;           /* 目的主机序号     */
	int nextHostNum;               /* 下一跳主机序号   */
	int jumpNum;                   /* 到目的子网的跳数 */
}; 


/**************************全局变量********************************/

/*功能: 映射表，根据网络序号，获得对应的IP+【接收】端口。
  备注：当网络拓扑图设计好之后，基本保持不变，
        当某一节点出现故障之后，可以删除表项*/
map<int, sockaddr_in> NumToAddrReceiveRouterMappingTable;


/*功能: 映射表，根据网络序号，获得对应的IP+【发送】端口。
  备注：当网络拓扑图设计好之后，基本保持不变，
		当某一节点出现故障之后，可以删除表项*/
map<int, sockaddr_in> NumToAddrSendRouterMappingTable;



/*功能：初始的全局路由表, 数组下标代表主机序号，值代表主机之间的费用。
  备注：OSPF拥有该表全部信息，而RIP则拥有该表中与自身节点相邻的信息。
        仅供初始的路由消息录入对象，随后的路由消息无需再这个表中更新。
*/
int routerLinkTable[MAX_ROUTER_NODES][MAX_ROUTER_NODES];



/*******************全局通用函数**************************************/ 
/********************************************
功能：将初始路由信息录入全局路由信息表格中
Author: 邱奕浩 
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
功能：将主机序号和IP地址信息录入到全局映射表中
Author: 邱奕浩
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
功能：画出网路拓扑图
输入：
输出：
依赖：
Author: qyh
Date created:
***************/
void DrawRouterTable() {
	// TODO

}


#endif
