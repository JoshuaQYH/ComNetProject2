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

class RouterWithLS {

private:
	RouterWithLS();                                 // ctor hidden
	RouterWithLS(RouterWithLS const&);            // copy ctor hidden
	RouterWithLS& operator=(RouterWithLS const&); // assign op. hidden
	~RouterWithLS() = default;                                // dtor hidden
	
													//中间变量
	int local;									   //本地路由编号
	vector<int> dist;                             //当前路径花费
	vector<int> path;                            //路径记录
	vector<bool> visited;						//标签是否访问

	vector< vector<RouterLink> > RouterTable;           /*全局路由表，二维向量，存储当前节点到邻居的路由信息*/
	vector<ForwardingRouterInfo> ForwardingTable;     /*转发表，通过路由表来计算收到某一结点消息后，决定下一跳*/

public:
	static RouterWithLS& Instance() {
		static RouterWithLS theRouterWithOSPF;
		return theRouterWithOSPF;
	}

	/*获取路由表*/
	vector<vector<RouterLink>> GetOSPFRouterTable() {
		return RouterTable;
	}


	/*检查路由表中超过180秒没有通信的路由，将routeLink中的linkState置为false，无法连接*/
	int CheckRouterTable();

	/*根据ID检测链路状态，若可以表示能够连接*/
	bool CheckLinkState(int RouterID);

	/*根据路由表的变化，更新转发表*/
	bool UpdateForwadingTable();

	/*接收其他结点的路由更新信息*/
	bool ReceiveUpdatingRouterInfo(vector<vector<RouterLink>> fromOtherRouterTable);

	/*发送给其他结点路由的更新信息*/
	vector<vector<RouterLink>> SendUpdatingRouterInfo();

	/*修改与当前邻接结点的链路费用信息*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*接收消息，确定下一跳地址ID*/
	int DecideForwardingAddress(int routerID);


	/*画出路由表*/
	void DrawRouterTable();

	/*打印转发表*/
	void PrintForwardingTable();

};

#endif // ! _ROUTER_WITH_OSPF_