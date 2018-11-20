#pragma once
#ifndef  _ROUTER_WITH_OSPF_H_
#define _ROUTER_WITH_OSPF_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: 使用类OSPF的方式来进行路由选择。

****************************************************/

class RouterWithOSPF {
public:
	static RouterWithOSPF& Instance() {
		static RouterWithOSPF theRouterWithOSPF;
		return theRouterWithOSPF;
	}

	/*更新转发表*/
	bool UpdateForwadingTable();

	/*接收其他结点的路由更新信息*/
	bool ReceiveUpdatingRouterInfo(vector<vector<RouterLink>>* fromOtherRouterTable);

	/*发送给其他结点路由的更新信息*/
	vector<vector<RouterLink>>* SendUpdatingRouterInfo(vector<vector<RouterLink>>* toOtherRouterTable);

	/*修改与当前邻接结点的链路费用信息*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*接收消息，确定下一跳地址ID*/
	int DecideForwardingAddress(int routerID);

	/*打印最优路径*/
	void PrintBestRouterPath();


private:
	RouterWithOSPF();                                 // ctor hidden
	RouterWithOSPF(RouterWithOSPF const&);            // copy ctor hidden
	RouterWithOSPF& operator=(RouterWithOSPF const&); // assign op. hidden
	~RouterWithOSPF();                                // dtor hidden

	vector<vector<RouterLink>> RouterTable;           /*全局路由表，二维向量，存储当前节点到邻居的路由信息*/
	vector<ForwardingRouterInfo> ForwardingTable;     /*转发表，通过路由表来计算收到某一结点消息后，决定下一跳*/
};

#endif // ! _ROUTER_WITH_OSPF_