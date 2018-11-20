#pragma once
#ifndef  _ROUTER_WITH_OSPF_H_
#define _ROUTER_WITH_OSPF_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: ʹ����OSPF�ķ�ʽ������·��ѡ��

****************************************************/

class RouterWithOSPF {
public:
	static RouterWithOSPF& Instance() {
		static RouterWithOSPF theRouterWithOSPF;
		return theRouterWithOSPF;
	}

	/*����ת����*/
	bool UpdateForwadingTable();

	/*������������·�ɸ�����Ϣ*/
	bool ReceiveUpdatingRouterInfo(vector<vector<RouterLink>>* fromOtherRouterTable);

	/*���͸��������·�ɵĸ�����Ϣ*/
	vector<vector<RouterLink>>* SendUpdatingRouterInfo(vector<vector<RouterLink>>* toOtherRouterTable);

	/*�޸��뵱ǰ�ڽӽ�����·������Ϣ*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*������Ϣ��ȷ����һ����ַID*/
	int DecideForwardingAddress(int routerID);

	/*��ӡ����·��*/
	void PrintBestRouterPath();


private:
	RouterWithOSPF();                                 // ctor hidden
	RouterWithOSPF(RouterWithOSPF const&);            // copy ctor hidden
	RouterWithOSPF& operator=(RouterWithOSPF const&); // assign op. hidden
	~RouterWithOSPF();                                // dtor hidden

	vector<vector<RouterLink>> RouterTable;           /*ȫ��·�ɱ���ά�������洢��ǰ�ڵ㵽�ھӵ�·����Ϣ*/
	vector<ForwardingRouterInfo> ForwardingTable;     /*ת����ͨ��·�ɱ��������յ�ĳһ�����Ϣ�󣬾�����һ��*/
};

#endif // ! _ROUTER_WITH_OSPF_