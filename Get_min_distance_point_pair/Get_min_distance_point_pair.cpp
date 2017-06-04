#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <stdio.h>

using namespace std;


typedef pair<double, double> point;
typedef vector<point> vecP;

inline double sqr(double x) { return x*x; }
inline double dis(point p1, point p2) { return sqr(p1.first - p2.first) + sqr(p1.second - p2.second); }

double get_min_dis(const vecP&, const int &, const int&);
double get_min_between(const vecP&, const int &, const int&, const int&, const double&);
double abs_m(const double& x) {
	if (x>0) return x;
	else return 0 - x;
}

bool cmp_x(const point & m1, const point & m2) {
	return m1.first < m2.first;
}

bool cmp_y(const point & m1, const point & m2) {
	return m1.second < m2.second;
}

int main()
{
	vecP points;

	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		point temp;
		cin >> temp.first >> temp.second;
		points.push_back(temp);
	}

	sort(points.begin(), points.end(), cmp_x);

	double ans = get_min_dis(points, 0, n - 1);
//	cout << ans << endl;
	printf("%.2f", ans);

	system("pause");

}


double get_min_dis(const vecP& pnts, const int& L, const int& R)
{
	if (L == R) return 250000000;

	int mid = (L + R) / 2;
	double minL = get_min_dis(pnts, L, mid);
	double minR = get_min_dis(pnts, mid + 1, R);

	double minLR = min(minL, minR);

	double minB = get_min_between(pnts, L, R, mid, minLR);

	return min(minB, minLR);

}

double get_min_between(const vecP& pnts, const int&L, const int&R, const int& mid, const double& minLR)
{
	double diff_max = 0;
	if (-1 == minLR) diff_max = pnts[R].first - pnts[L].first;
	else diff_max = minLR;

	int L_boundry = mid;
	int R_boundry = mid + 1;
	while (abs_m(pnts[L_boundry].first - pnts[mid].first) < diff_max && L_boundry > L) L_boundry--;
	while (abs_m(pnts[R_boundry].first - pnts[mid].first) < diff_max && R_boundry < R) R_boundry++;

	vecP pnts_between;
	pnts_between.clear();
	for (int i = L_boundry; i <= R_boundry; i++)
		pnts_between.push_back(pnts[i]);

	//sort the points by Y value
	sort(pnts_between.begin(), pnts_between.end(), cmp_y);

	double min_dis = 250000000;

	for (int i = 0; i < (int)pnts_between.size(); i++) {
		int toGo = i + 15;
		if (toGo >= (int)pnts_between.size()) toGo = pnts_between.size() - 1;

		for (int j = i + 1; j <= toGo; j++) {
			double temp = dis(pnts_between[i], pnts_between[j]);
			if (temp < min_dis) min_dis = temp;
		}
	}

	return min_dis;

}
