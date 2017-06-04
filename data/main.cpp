#include <iostream>
#include <fstream>
#include <utility>
#include <time.h>

using namespace std;

int n, m;


int main()
{
	srand(time(NULL));
	ofstream out("D:\\in_topo_sort.txt");
	out << 50 << " " << 2500<<endl;
	//for (int i = 0; i < 2500; i++) 
	//{
	//	out << rand() % 50 + 1 << " " << rand() % 50 + 1 << endl;
	//}
	for (int i = 0; i < 49; i++)
		for (int j=0;j<51;j++)
		{
			out << i + 1 << " " << i + 2 << endl;
		}
	out << 1 << " " << 2;

	out.close();
}