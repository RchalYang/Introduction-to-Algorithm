#include <iostream>
#include <queue> 
#include <set>

using namespace std;

struct edge {
	int start;
	int end;
	int cost;
	edge(int s, int e, int c){
		start = s;
		end = e;
		cost = c;
	}

	friend bool operator < (const edge &a, const edge &b) { return a.cost > b.cost; }

};

int n, m;
int ans = 0;

int main()
{
	priority_queue<edge> p_queue;

	//input
	cin >> n >> m;

	//sets are made up with the nodes having the same root
	vector<set<int>*> union_find(n+1,NULL);

	for (int i = 0; i < m; i++)
	{
		int s, e, c;
		cin >> s >> e >> c;
		edge temp(s, e, c);
		//put into the priority queue
		p_queue.push(temp);
	}
	
	for (int i = 0; i < n - 1; i++)
	{
		//find the first useful edge 
		while ((union_find[(p_queue.top()).start] == union_find[(p_queue.top()).end]) && union_find[(p_queue.top()).start] != NULL) 
		{
			p_queue.pop();
		}

		edge now_edge = p_queue.top();
		p_queue.pop();
		ans += now_edge.cost;
		if (union_find[now_edge.start] == NULL && union_find[now_edge.end] == NULL) 
		{
			set<int>* temp = new set<int>;
			(*temp).clear();
			(*temp).insert(now_edge.start);
			(*temp).insert(now_edge.end);
			union_find[now_edge.start] = temp;
			union_find[now_edge.end] = temp;
			continue;
		}

		if (union_find[now_edge.start] != NULL && union_find[now_edge.end] == NULL)
		{
			set<int>* temp = union_find[now_edge.start];
			(*temp).insert(now_edge.end);
			union_find[now_edge.end] = temp;
			continue;
		}

		if (union_find[now_edge.start] == NULL && union_find[now_edge.end] != NULL)
		{
			set<int>* temp = union_find[now_edge.end];
			(*temp).insert(now_edge.start);
			union_find[now_edge.start] = temp;
			continue;
		}

		set<int> *large = union_find[now_edge.start],
					*small = union_find[now_edge.end];
		if ((*large).size() < (*small).size()) {
			swap(large, small);
		}

		//put the element in the small set into the larger set
		set<int>::iterator iter = (*small).begin();
		while (iter != (*small).end()) 
		{
			int temp = (int)(*iter);
			(*large).insert(temp);
			union_find[temp] = large;
			iter++;
		}
		delete(small);
	}
	cout << ans<<endl;
	system("pause");
}