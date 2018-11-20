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
#include "RouterWithOSPF.h"
#include "RouterWithRIP.h"
#include "SocketService.h"
#include "Message.h"
#include <windows.data.json.h>
#include "json\json.h"
#include <queue>

using namespace cimg_library;
using namespace std;

/*******************全局常量************************************/

#define SOCK_VER 2                  /*程序使用的winsock主版本*/

#define CILENT_IP "127.0.0.1"       /*本客户端的IP地址*/

#define CLIENT_RECEIVE_PORT "3333"  /*客户端的接收端口*/

#define CLIENT_SEND_PORT "3332"     /*客户端发送端口*/
 
#define LOCAL_HOST_NUM  1           /*本地主机序号，假设不可变且唯一*/

#define BUFF_SIZE 2048              /*收发消息的缓冲区大小*/

#define INFINITY_COST 55555         /*若两个节点不相连，那么设置为该值*/
 
#define MAX_ROUTER_NODES  100       /*最大路由节点*/



/****************全局通用数据结构********************************/

/************
路由连接结构
*************/
struct RouterLink{
	int localHostNum;           /* 本地主机序号          */
	int dstHostNum;             /* 目的主机序号          */
	int linkCost;               /* 链路代价              */
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


const uint8_t MY_PROTO_MAGIC = 88;                   /*协议魔方数*/
const uint32_t MY_PROTO_MAX_SIZE = 10 * 1024 * 1024; /*协议包最大为10M*/
const uint32_t MY_PROTO_HEAD_SIZE = 8;               /*协议头大小为8字节*/



/* 协议状态 */
typedef enum MyProtoParserStatus
{
	ON_PARSER_INIT = 0,
	ON_PARSER_HAED = 1,
	ON_PARSER_BODY = 2,
}MyProtoParserStatus;

/*
	协议头结构体设计
 */
struct MyProtoHead
{
	uint8_t version;    //协议版本号
	uint8_t magic;      //协议魔数
	uint16_t server;    //协议复用的服务号，标识协议之上的不同服务
	uint32_t len;       //协议长度（协议头长度+变长json协议体长度）
};

/*
	协议消息体
 */
struct MyProtoMsg
{
	MyProtoHead head;   //协议头
	Json::Value body;   //协议体,使用json存储
};





/**************************全局变量********************************/

/*功能: 映射表，根据网络序号，获得对应的IP+端口。
  备注：当网络拓扑图设计好之后，基本保持不变，
        当某一节点出现故障之后，可以删除表项*/
map<int, sockaddr_in> NumToAddrRouterMappingTable;


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
}


/*********************************************
功能：将主机序号和IP地址信息录入到全局映射表中
Author: 邱奕浩
Date created: 18/11/19
Date finished: 
**********************************************/
void InitRouterMappingTable() {
	// TODO
}

// TODO
/**************
功能：画出网路拓扑图
输入：
输出：
依赖：
Author: 王德超
Date created:
***************/
void DrawRouterTable() {
	// TODO
}


#endif
