#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace std;

typedef pair<double, double> point;
typedef vector<point> vecP;

bool CmpUsingX(const point& a, const point& b) {
	return a.first > b.first;
}

bool CmpUsingY(const point& a, const point& b) {
	return a.second > b.second;
}

double Dis(const point& a, const point& b) {
	return (a.first - b.first)*(a.first - b.first) + (a.second - b.second)*(a.second - b.second);
}

int n;
vecP points(0);

//获取两端间点对最小距离
double GetMinDis(const int&, const int&);
//获取窄带中点对最小距离
double GetMinBetween(const int&, const int&,const double&);

int main(){
	//input
	cin >> n;
	for (int i = 0; i < n; i++){
		point temp;
		cin >> temp.first >> temp.second;
		points.push_back(temp);
	}

	sort(points.begin(), points.end(), CmpUsingX);

	double ans = GetMinDis(0, n-1);
	printf("%.2f",ans);
	system("pause");
}

double GetMinDis(const int& L, const int& R){
	
	if (L >= R) return 200000000;
	if (L + 1 == R) return Dis(points[L], points[R]);

	int mid = (L + R) / 2;

	double min_l = GetMinDis(L, mid);
	double min_r = GetMinDis(mid + 1, R);
	double min_l_r = min(min_l, min_r);
	double min_between = GetMinBetween(L, R, min_l_r);

	return min_between;
}

double GetMinBetween(const int& L, const int& R, const double& min_diff){

	int mid = (L + R) / 2;

	int boundray_l = mid, boundray_r = mid+1;
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