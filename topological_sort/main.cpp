#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 50;

int n,
	m;

vector<int> fan_in(MAXN, 0);
vector<int> fan_out(MAXN, 0);

vector<vector<int>> side_in(MAXN,vector<int>(0));//side to points
vector<vector<int>> side_out(MAXN, vector<int>(0));//side from points

bool DagOrNot();
bool FindTheLoop();

int main()
{
	cin >> n >> m;

	for (int i = 0; i < m; i++)
	{
		int tempf,
			tempt;
		cin >> tempf >> tempt;

		side_in[tempt].push_back(tempf);
		side_out[tempt].push_back(tempt);
	}
	
	queue<int> topological_queue;
	queue<int> points_without_in;
	if (DagOrNot())//judeg whether the map is DAG
	{
		while (!topological_queue.empty()) 
		{
			cout << topological_queue.front();
			topological_queue.pop;
		}
	}
	else 
	{
	}

}


bool DagOrNot() 
{
	queue<int> topological_queue;
	queue<int> points_without_in;

	for (int i = 0; i < n; i++)
	{
		if (side_in[i].size() == 0) 
		{
			points_without_in.push(i);
			topological_queue.push(i);
			side_out[i].swap(vector<int>(0));// clean up
		}		
	}

	while (!points_without_in.empty()) 
	{
		int current = points_without_in.front();
		points_without_in.pop();
		for (int i = 0; i < side_in[current].size(); i++)
		{
			vector<int>::iterator to_find = side_in[side_in[current][i]].begin(); \
			while (*to_find != current) to_find++;
			to_find = side_in[side_in[current][i]].erase(to_find);
			continue;
		}
		topological_queue.push(current);
		side_out[current].swap(vector<int>(0));// clean up
	}

	if (topological_queue.size() != n) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool FindTheLoop() 
{

}