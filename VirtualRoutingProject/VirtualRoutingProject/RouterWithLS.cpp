#include "RouterWithLS.h"


//���캯������������
RouterWithLS::RouterWithLS() {
	local = LOCAL_HOST_NUM;
	RouterLink temp;
	for (int i = 0; i < MAX_ROUTER_NODES; i++) {
		for (int t = 0; t < MAX_ROUTER_NODES; t++) {
			if (routerLinkTable[i][t]) {
				temp.localHostNum = i;
				temp.dstHostNum = t;
				temp.lastModified = time(NULL);
				temp.linkCost = routerLinkTable[i][t];
				temp.linkState = true;
				RouterTable[i][t] = temp;
			}
		}
	}
	UpdateForwadingTable();

}
RouterWithLS::RouterWithLS(RouterWithLS const&) {

}
RouterWithLS& RouterWithLS::operator=(RouterWithLS const&) {

}




/*���·�ɱ��г���180��û��ͨ�ŵ�·�ɣ���routeLink�е�linkState��Ϊfalse���޷�����*/
int RouterWithLS::CheckRouterTable() {
	time_t nowTime = (int)time(NULL);
	for (auto i = RouterTable[local].begin(); i != RouterTable[local].end(); i++) {
		if (nowTime - (i->lastModified) <= 180) {

		}
		else {
			if (i->lastModified == true) {
				i->lastModified = false;
				//UpdateForwardingTable(i->dstHostNum, i->linkCost, INFINITY_COST);
				i->linkCost = INFINITY_COST;
			}
		}
	}
}

/*����ID�����·״̬�������Ա�ʾ�ܹ�����*/
bool RouterWithLS::CheckLinkState(int RouterID) {
	for (auto i = RouterTable[local].begin(); i != RouterTable[local].end(); i++) {
		if (i->dstHostNum == RouterID) {
			return i->linkState == true ? true : false;
		}
	}
	return false;
}

/*����·�ɱ�ı仯������ת����*/
bool RouterWithLS::UpdateForwadingTable() {
	int min; int index = 0;
	int nodes = RouterTable[0].size();
	for (int i = 0; i < nodes; i++) {
		visited[i] = false;
	}
	visited[local] = true;
	for (int i = 0; i < nodes; i++) {
		dist[i] = RouterTable[local][i].linkCost;
		path[i] = local;
	}
	for (int i = 1; i < nodes; i++) {
		min = 9999;
		for (int j = 0; j < nodes; j++)
		{
			if (visited[j] == false && dist[j] < min)  // �ҵ�Ȩֵ��С
			{
				min = dist[j];
				index = j;
			}
		}
		visited[index] = true;
		for (int j = 0; j < nodes; j++) {
			if (!visited[j] && RouterTable[index][j].linkCost != 9999 && RouterTable[index][j].linkCost + min < dist[j]) {
				dist[j] = RouterTable[index][j].linkCost + min;
				path[j] = index;
			}
		}
	}
	for (int i = 0; i < nodes; i++) {
		if (i != local) {
			int num = 0;
			vector<int> realpath;
			ForwardingRouterInfo temp;
			realpath.push_back(i);
			cout << local << "��" << i << "��̾����ǣ�" <<
				dist[i] << "��·���ǣ�" << i;
			int t = path[i];
			while (t != local) {
				realpath.push_back(t);
				cout << "<--" << t;
				t = path[t];
				num++;
			}
			temp.nextHostNum = realpath.back();
			realpath.push_back(t);
			temp.jumpNum = num + 1;
			temp.targetedHostNum = i;

			ForwardingTable[i] = temp;
		}
	}
	return true;
}

/*������������·�ɸ�����Ϣ*/
bool RouterWithLS::ReceiveUpdatingRouterInfo(vector<vector<RouterLink>> fromOtherRouterTable) {

	for (int i = 0; i < RouterTable[0].size(); i++) {
		for (int t = 0; t < RouterTable[0].size(); t++) {
			RouterTable[i][t].lastModified = time(NULL);
			RouterTable[i][t].linkCost = fromOtherRouterTable[i][t].linkCost;
			RouterTable[i][t].linkState = fromOtherRouterTable[i][t].linkState;
		}
	}
	UpdateForwadingTable();
	return true;
}

/*���͸��������·�ɵĸ�����Ϣ*/
vector<vector<RouterLink>> RouterWithLS::SendUpdatingRouterInfo() {
	return RouterTable;
}

/*�޸��뵱ǰ�ڽӽ�����·������Ϣ*/
bool RouterWithLS::ModifyNeighborRouterInfo(int neighborRouterID, int cost) {
	if (neighborRouterID >= RouterTable[0].size()) {
		return false;
	}
	RouterTable[local][neighborRouterID].linkCost = cost;
	return true;
}

/*������Ϣ��ȷ����һ����ַID*/
int RouterWithLS::DecideForwardingAddress(int routerID) {
	return ForwardingTable[routerID].nextHostNum;
}



/*����·�ɱ�*/
void RouterWithLS::DrawRouterTable() {
	cout << "The Router Table:" << endl;
	for (auto i = RouterTable[local].begin(); i != RouterTable[local].end(); i++) {
		cout << "Local Number: " << i->localHostNum << " Neighbor: " << i->dstHostNum << " Link Status: " << i->linkState << " ";
		if (i->linkCost == INFINITY_COST) cout << "INFINITY" << endl;
		else cout << i->linkCost << endl;
	}

}

/*��ӡת����*/
void RouterWithLS::PrintForwardingTable() {
	for (auto i = ForwardingTable.begin(); i != ForwardingTable.end(); i++) {
		cout << "Destination: " << i->targetedHostNum << " Next number: " << i->nextHostNum << " ";
		if (i->jumpNum == INFINITY_COST) cout << "INFINITY" << endl;
		else cout << i->jumpNum;
	}
}


