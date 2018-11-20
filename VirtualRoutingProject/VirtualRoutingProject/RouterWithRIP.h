#pragma once
#ifndef  _ROUTER_WITH_RIP_H_
#define _ROUTER_WITH_RIP_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: 使用类RIP协议&DV来实现路由选择

****************************************************/

class RouterWithRIP {
private:
	RouterWithRIP();                                // ctor hidden
	RouterWithRIP(RouterWithRIP const&);            // copy ctor hidden
	RouterWithRIP& operator=(RouterWithRIP const&); // assign op. hidden
	~RouterWithRIP();                               // dtor hidden

	vector<RouterLink> RouterTable;                 /*局部路由表，一维向量，存储当前节点到邻居的路由信息*/
	vector<ForwardingRouterInfo> ForwardingTable;   /*转发表，通过路由表来计算收到某一结点消息后，决定下一跳*/


public:
	static RouterWithRIP& Instance() {
		static RouterWithRIP theRouterWithRIP;
		return theRouterWithRIP;
	}


	/*更新转发表*/
	bool UpdateForwadingTable();

	/*接收其他结点的路由更新信息*/
	bool ReceiveUpdatingRouterInfo(vector<RouterLink>* fromOtherRouterTable);

	/*发送给其他结点路由的更新信息*/
	vector<RouterLink>* SendUpdatingRouterInfo(vector<RouterLink>* toOtherRouterTable);

	/*修改与当前邻接结点的链路费用信息*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*接收消息，确定下一跳地址ID*/
	int DecideForwardingAddress(int routerID);



};


#endif // ! _ROUTER_WITH_RIP_H_

