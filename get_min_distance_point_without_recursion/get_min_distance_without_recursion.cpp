#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

const double MAXD = 200000000;

//类型定义
typedef pair<double, double> point;
typedef vector<point> vecP;

//排序所用比较函数
bool CmpUsingX(const point& a, const point& b) {
	return a.first > b.first;
}
bool CmpUsingY(const point& a, const point& b) {
	return a.second > b.second;
}
//距离函数
double Dis(const point& a, const point& b) {
	return (a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second);
}


int n;
vecP points(0);

//获取窄带中点对最小距离
double GetMinBetween(const int&, const int&,const int&, const double&);

int main()
{
	//input
	cin >> n;
	for (int i = 0; i < n; i++) {
		point temp;
		cin >> temp.first >> temp.second;
		points.push_back(temp);
	}

	vector<double> min_dis(n, MAXD);
	sort(points.begin(), points.end(), CmpUsingX);
	//利用数组标记结果
	int len = 2;
	while (len < n) {
		int start = 0;
		while (start < n) {
			int right = start + len - 1;
			if (right >= n) right = n - 1;
			double minL = min_dis[start];
			double minR = min_dis[right];
			double minLRB = GetMinBetween(start, right,(start+right)/2, min(minL, minR));
			
			min_dis[start] = minLRB;
			min_dis[right] = minLRB;
			start += len;
		}
		len *= 2;
	}
	double ans = GetMinBetween(0,n-1,(len/2)-1,min(min_dis[0], min_dis[n - 1]));
	if (n == 2) ans = Dis(points[0], points[1]);
	printf("%.2f", ans);
	system("pause");
}

double GetMinBetween(const int& L, const int& R,const int& mid, const double& min_diff) {
	if (L == R) return MAXD;
	if (L + 1 == R) return Dis(points[L], points[R]);
	//获取左右边界
	int boundray_l = mid, boundray_r = mid + 1;
	while ((boundray_l > L) && (abs(boundray_l - mid) < min_diff)) boundray_l--;
	while ((boundray_r < R) && (abs(boundray_r - mid) < min_diff)) boundray_r++;
	//小数据量完全不需要排序
	if (boundray_r - boundray_l <= 20) {
		double temp = min_diff;
		for (int i = boundray_l; i <= boundray_r; i++) {
			for (int j = i + 1; j <= boundray_r; j++) {
				temp = min(temp, Dis(points[i], points[j]));
			}
		}
		return temp;
	}
	//大数据量排序
	vecP points_in_between(0);
	for (int i = boundray_l; i <= boundray_r; i++) {
		points_in_between.push_back(points[i]);
	}
	sort(points_in_between.begin(), points_in_between.end(), CmpUsingY);
	//仅考虑后侧13个点（前方点已被考虑过）
	double temp = min_diff;
	int len = (int)points_in_between.size();
	for (int i = 0; i < len; i++) {
		int l_most = i + 14;
		l_most = min(len, l_most);
		for (int j = i + 1; j < l_most; j++) {
			temp = min(temp, Dis(points_in_between[i], points_in_between[j]));
		}
	}

	return temp;
}