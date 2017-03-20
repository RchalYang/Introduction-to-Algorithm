#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;


typedef pair<int, int> point;
typedef vector<point> vecP;

inline int sqr(int x) { return x*x; }
inline int dis(point p1, point p2) { return sqr(p1.first - p2.first) + sqr(p1.second - p2.second); }

int get_min_dis(const vecP&,const int &,const int&);
int get_min_between(const vecP&, const int &, const int&,const int&,const int&);

bool cmp_x(const point & m1,const point & m2) {
	return m1.first < m2.first;
}

bool cmp_y(const point & m1,const point & m2) {
	return m1.second < m2.second;
}

int main()
{
	vecP points;

	int n;
	cin >> n;
	for (int i = 0;i < n;i++){
		point temp;
		cin >> temp.first >> temp.second;
		points.push_back(temp);
	}

	sort(points.begin(), points.end(), cmp_x);

	int ans = get_min_dis(points,0,n-1);
	cout << ans;

//	system("pause");

}


int get_min_dis(const vecP& pnts, const int& L, const int& R)
{
	if (L == R) return 250000000;

	int mid = (L + R) / 2;
	int minL = get_min_dis(pnts, L, mid);
	int minR = get_min_dis(pnts, mid + 1, R);

	int minLR = min(minL, minR);

	int minB = get_min_between(pnts, L, R, mid,minLR);
	
	return min(minB, minLR);

}

int get_min_between(const vecP& pnts,const int&L,const int&R,const int& mid,const int& minLR)
{
	int diff_max = 0;
	if (-1 == minLR) diff_max = pnts[R].first - pnts[L].first;
	else diff_max = minLR;

	int L_boundry = mid;
	int R_boundry = mid + 1;
	while (abs(pnts[L_boundry].first - pnts[mid].first) < diff_max && L_boundry > L) L_boundry--;
	while (abs(pnts[R_boundry].first - pnts[mid].first) < diff_max && R_boundry < R) R_boundry++;

	vecP pnts_between;
	pnts_between.clear();
	for (int i = L_boundry;i <= R_boundry;i++)
		pnts_between.push_back(pnts[i]);

	//sort the points by Y value
	sort(pnts_between.begin(), pnts_between.end(), cmp_y);

	int min_dis= 250000000;

	for (int i = 0;i < (int)pnts_between.size();i++) {
		int toGo = i + 15;
		if (toGo >= (int)pnts_between.size()) toGo = pnts_between.size()-1;

		for (int j = i + 1;j <= toGo;j++) {
			int temp = dis(pnts_between[i], pnts_between[j]);
			if (temp < min_dis) min_dis = temp;
		}
	}

	return min_dis;

}
