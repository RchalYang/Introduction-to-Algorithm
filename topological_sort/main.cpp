#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int MAXN = 55;

int n,m;

vector<vector<int> > side_in(MAXN,vector<int>(0));//side to points
vector<vector<int> > side_out(MAXN, vector<int>(0));//side from points

queue<int> topological_queue;
queue<int> points_without_in;

bool DagOrNot();
void FindTheLoop();

int main()
{
	cin >> n >> m;

	for (int i = 0; i < m; i++)
	{
		int tempf,
			tempt;
		cin >> tempf >> tempt;

		side_in[tempt].push_back(tempf);//record the fan in
		//record the side start from tempf
		side_out[tempf].push_back(tempt);
	}
	
	if (DagOrNot())//judge whether the map is DAG
	{
		cout << "YES" << endl;
		int size = topological_queue.size();
		for (int i=0;i<size-1;i++) 
		{
			cout << topological_queue.front()<<",";
			topological_queue.pop();
		}
		cout << topological_queue.front();
		topological_queue.pop();
	}
	else 
	{
		cout << "NO" << endl;
		FindTheLoop();
	}
	system("pause");
}


bool DagOrNot() 
{
	//find the first spot
	for (int i = 1; i <= n; i++)
	{
		if (side_in[i].size() == 0) 
		{
			points_without_in.push(i);
		}		
	}

	while (!points_without_in.empty()) 
	{
		int current = points_without_in.front();
		points_without_in.pop();
		for (int i = 0; i < side_out[current].size(); i++)
		{
			int point_to_check = side_out[current][i];
			vector<int>::iterator to_find = side_in[point_to_check].begin();
			while (*to_find != current) to_find++;
			to_find = side_in[point_to_check].erase(to_find);
			if (side_in[point_to_check].size() == 0)
			{
				points_without_in.push(point_to_check);
			}
			continue;
		}
		topological_queue.push(current);
		side_out[current].clear();// clean up
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

void FindTheLoop() 
{
	int fast_spot;
	int slow_spot;
	//find the start point
	int flag = 1;
	while (flag)
	{
		flag = 0;
		for (int i = 1; i <= n; i++)
		{
			if (side_in[i].size() !=0 && side_out[i].size() == 0)
			{
				for (int j = 0; j < side_in[i].size(); j++)
				{
					int point_to_check = side_in[i][j];
					vector<int>::iterator to_find = side_out[point_to_check].begin();
					while (*to_find != i) to_find++;
					to_find = side_out[point_to_check].erase(to_find);
				}
				side_in[i].clear();
				flag = 1;
			}
		}
	}

	int j = 1;
	while (j <= n && side_in[j].size() == 0) j++;
	fast_spot = j;

	int step_len = 0;
	do
	{
		slow_spot = fast_spot;
		step_len++;
		for (int i = 0; i < step_len; i++) 
		{
			fast_spot = side_out[fast_spot][0];
			fast_spot = side_out[fast_spot][0];
			slow_spot = side_out[slow_spot][0];
		}
	} while (fast_spot != slow_spot);

	//output the loop
	for (int i = 0; i < step_len; i++)
	{
		cout << fast_spot<<",";
		fast_spot = side_out[fast_spot][0];
	}
	cout << fast_spot;
}