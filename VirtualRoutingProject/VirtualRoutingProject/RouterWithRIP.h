#pragma once
#ifndef  _ROUTER_WITH_RIP_H_
#define _ROUTER_WITH_RIP_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: ʹ����RIPЭ��&DV��ʵ��·��ѡ��

****************************************************/

class RouterWithRIP {
private:
	RouterWithRIP();                                // ctor hidden
	RouterWithRIP(RouterWithRIP const&);            // copy ctor hidden
	RouterWithRIP& operator=(RouterWithRIP const&); // assign op. hidden
	~RouterWithRIP() = default;                               // dtor hidden

	int LOCAL;
	vector<RouterLink> RouterTable;                 /*�ֲ�·�ɱ�һά�������洢��ǰ�ڵ㵽�ھӵ�·����Ϣ*/
	vector<ForwardingRouterInfo> ForwardingTable;   /*ת����ͨ��·�ɱ��������յ�ĳһ�����Ϣ�󣬾�����һ��*/


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
	/*���·�ɱ��г���180��û��ͨ�ŵ�·�ɣ���routeLink�е�linkState��Ϊfalse���޷�����*/
	void CheckRouterTable();

	/*����ID�����·״̬�������Ա�ʾ�ܹ�����*/
	bool CheckLinkState(int RouterID);

	/*·��ѡ������仯�����ת����*/
	bool UpdateForwardingTable(vector<ForwardingRouterInfo>& fromOtherRouterTable, const int fromNum);
	bool UpdateForwardingTable(const int nextNum, const int st, const int cost);

	/*������������·�ɸ�����Ϣ*/
	bool ReceiveUpdatingRouterInfo(vector<ForwardingRouterInfo>& fromOtherRouterTable, const int fromNum);

	/*���͸��������·�ɵĸ�����Ϣ*/
	vector<ForwardingRouterInfo>* SendUpdatingRouterInfo(vector<ForwardingRouterInfo>* toOtherRouterTable);

	/*�޸��뵱ǰ�ڽӽ�����·������Ϣ*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*������Ϣ��ȷ����һ����ַID*/
	int DecideForwardingAddress(int routerID);

	/*����·�ɱ�*/
	void DrawRouterTable();

	/*��ӡת����*/
	void PrintForwardingTable();


};


#endif // ! _ROUTER_WITH_RIP_H_

