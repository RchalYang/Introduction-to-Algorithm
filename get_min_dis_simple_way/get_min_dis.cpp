#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

typedef pair<double, double> point;
typedef vector<point> vecP;

const double MAXD = 200000000;

double Dis(const point& a, const point& b) {
	return (a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second);
}

int n;
vecP points(0);

int main() {
	//input
	cin >> n;
	for (int i = 0; i < n; i++) {
		point temp;
		cin >> temp.first >> temp.second;
		points.push_back(temp);
	}

	double temp = MAXD;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			temp = min(temp, Dis(points[i], points[j]));
		}
	}
	printf("%.2f", temp);
	system("pause");
}