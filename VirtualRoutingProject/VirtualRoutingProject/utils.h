#pragma once
/******************************************

Author: qiuyihao

Date created: 18/11/18

Date finished: 

Description�����һЩȫ�ֳ�����ͨ�õĺ���

*******************************************/
#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <winsock.h>  // �׽�������ͷ�ļ�
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

/*******************ȫ�ֳ���*************************/

#define SOCK_VER 2                 /*����ʹ�õ�winsock���汾*/

#define CILENT_IP "127.0.0.1"       /*���ͻ��˵�IP��ַ*/

#define CLIENT_RECEIVE_PORT "3333"  /*�ͻ��˵Ľ��ն˿�*/

#define CLIENT_SEND_PORT "3332"

#define BUFF_SIZE 2048             /*�շ���Ϣ�Ļ�������С*/

#define INFINITY_COST 55555        /*�������ڵ㲻��������ô����Ϊ��ֵ*/
 

/*******************ȫ��ͨ�ú���*********************/

// TODO
/**************
���ܣ�
���룺
�����
������
Author: ���³�
Date: 
***************/
void DrawRouterTable() {

}


#endif



/****************ȫ��ͨ�����ݽṹ*********************/

/******
���������ṹ
******/
struct DistanceVector {
	sockaddr_in localAddress;   /* ���ڵ��IP��ַ */
	sockaddr_in dstAddress;     /* Ŀ�Ľڵ�IP��ַ */
	int linkCost;               /* ��·���� */
	time_t lastModified;        /* ��·״̬�ϴθ���ʱ���*/
};


/***
ת������
***/
struct ForwardingRouterInfo {
	sockaddr_in targetedAddress;   /* Ŀ��������ַ */
	sockaddr_in nextRouterAddress; /* ��һ��·������ַ */
	int jumpNum;                   /*��Ŀ������������ */
};