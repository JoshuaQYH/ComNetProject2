#pragma once
/******************************************

Author: qiuyihao

Date created: 18/11/18

Date finished: 

Description：存放一些全局常量和通用的函数

*******************************************/
#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <winsock.h>  // 套接字所需头文件
#pragma comment(lib, "WSOCK32.LIB")
#include <iostream>
#include<stdlib.h>
#include<vector>
#include<time.h>
#include<algorithm>
#include<fstream>
#include<time.h>
#include "CImg.h"
#include "RouterWithOSPF.h"
#include "RouterWithRIP.h"
#include "SocketService.h"
#include "Message.h"


using namespace std;

/*******************全局常量*************************/

#define SOCK_VER 2                 /*程序使用的winsock主版本*/

#define CILENT_IP "127.0.0.1"       /*本客户端的IP地址*/

#define CLIENT_RECEIVE_PORT "3333"  /*客户端的接收端口*/

#define CLIENT_SEND_PORT "3332"

#define BUFF_SIZE 2048             /*收发消息的缓冲区大小*/

#define INFINITY_COST 55555        /*若两个节点不相连，那么设置为该值*/
 

/*******************全局通用函数*********************/

// TODO
/**************
功能：
输入：
输出：
依赖：
Author: 王德超
Date: 
***************/
void DrawRouterTable() {

}


#endif



/****************全局通用数据结构*********************/

/******
距离向量结构
******/
struct DistanceVector {
	sockaddr_in localAddress;   /* 本节点的IP地址 */
	sockaddr_in dstAddress;     /* 目的节点IP地址 */
	int linkCost;               /* 链路代价 */
	time_t lastModified;        /* 链路状态上次更新时间戳*/
};


/***
转发表项
***/
struct ForwardingRouterInfo {
	sockaddr_in targetedAddress;   /* 目的子网地址 */
	sockaddr_in nextRouterAddress; /* 下一跳路由器地址 */
	int jumpNum;                   /*到目的子网的跳数 */
};