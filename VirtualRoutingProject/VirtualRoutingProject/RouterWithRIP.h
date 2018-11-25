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
	~RouterWithRIP() = default;                               // dtor hidden

	int LOCAL;
	vector<RouterLink> RouterTable;                 /*局部路由表，一维向量，存储当前节点到邻居的路由信息*/
	vector<ForwardingRouterInfo> ForwardingTable;   /*转发表，通过路由表来计算收到某一结点消息后，决定下一跳*/


public:
	static RouterWithRIP& Instance() {
		static RouterWithRIP theRouterWithRIP;
		return theRouterWithRIP;
	}
	vector<RouterLink> GetRIPRouterTable() {
		return RouterTable;
	}
	vector<ForwardingRouterInfo> GetRIPForwardingTable() {
		return ForwardingTable;
	}
	/*检查路由表中超过180秒没有通信的路由，将routeLink中的linkState置为false，无法连接*/
	void CheckRouterTable();

	/*根据ID检测链路状态，若可以表示能够连接*/
	bool CheckLinkState(int RouterID);

	/*路由选择表发生变化则更新转发表*/
	bool UpdateForwardingTable(vector<ForwardingRouterInfo>& fromOtherRouterTable, const int fromNum);
	bool UpdateForwardingTable(const int nextNum, const int st, const int cost);

	/*接收其他结点的路由更新信息*/
	bool ReceiveUpdatingRouterInfo(vector<ForwardingRouterInfo>& fromOtherRouterTable, const int fromNum);

	/*发送给其他结点路由的更新信息*/
	vector<ForwardingRouterInfo>* SendUpdatingRouterInfo(vector<ForwardingRouterInfo>* toOtherRouterTable);

	/*修改与当前邻接结点的链路费用信息*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*接收消息，确定下一跳地址ID*/
	int DecideForwardingAddress(int routerID);

	/*画出路由表*/
	void DrawRouterTable();

	/*打印转发表*/
	void PrintForwardingTable();


};


#endif // ! _ROUTER_WITH_RIP_H_

