#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

class Edge {
public:
	int ori;
	int des;
	int value;
	Edge() {
		ori = 0;
		des = 0;
		value = 0;
	}
	Edge(int o, int d, int v) {
		ori = o;
		des = d;
		value = v;
	}
};

bool compare(Edge x, Edge y)
{
	return x.value < y.value;
}

int f_father(int now,vector<int>& arr_temp) {
	int temp = now;
	while (arr_temp[temp] != temp) temp = arr_temp[temp];
	return temp;
}

int n, m;
int ans = 0;
vector<Edge> arr_edge;
vector<int> arr_node;

int main()
{
//	ifstream in("input.txt");
	cin >> n >> m;

	for (int i = 0;i < m;i++) {
		int to, td, tv;
		cin >> to >> td >> tv;
		Edge temp(to,td,tv);
		arr_edge.push_back(temp);
	}

	for (int i = 0;i <=n;i++)
		arr_node.push_back(i);

	sort(arr_edge.begin(),arr_edge.end(),compare);

	for (int i = 0;i < m;i++){
		int fo = f_father(arr_edge[i].ori,arr_node);
		int fd = f_father(arr_edge[i].des,arr_node);
		if (fo != fd) {
			ans += arr_edge[i].value;
			arr_node[fo] = fd;
		}
	}

	cout << ans;

//	system("pause");
}