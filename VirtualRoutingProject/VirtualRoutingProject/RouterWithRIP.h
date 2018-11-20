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
	~RouterWithRIP();                               // dtor hidden

	vector<RouterLink> RouterTable;                 /*�ֲ�·�ɱ�һά�������洢��ǰ�ڵ㵽�ھӵ�·����Ϣ*/
	vector<ForwardingRouterInfo> ForwardingTable;   /*ת����ͨ��·�ɱ��������յ�ĳһ�����Ϣ�󣬾�����һ��*/


public:
	static RouterWithRIP& Instance() {
		static RouterWithRIP theRouterWithRIP;
		return theRouterWithRIP;
	}


	/*����ת����*/
	bool UpdateForwadingTable();

	/*������������·�ɸ�����Ϣ*/
	bool ReceiveUpdatingRouterInfo(vector<RouterLink>* fromOtherRouterTable);

	/*���͸��������·�ɵĸ�����Ϣ*/
	vector<RouterLink>* SendUpdatingRouterInfo(vector<RouterLink>* toOtherRouterTable);

	/*�޸��뵱ǰ�ڽӽ�����·������Ϣ*/
	bool ModifyNeighborRouterInfo(int neighborRouterID, int cost);

	/*������Ϣ��ȷ����һ����ַID*/
	int DecideForwardingAddress(int routerID);



};


#endif // ! _ROUTER_WITH_RIP_H_

