#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string str_origin, str_target;

int dp[2003][2003] = { 0 };

int main()
{
	cin >> str_origin;
	cin >> str_target;

	if (str_origin.size() < str_target.size()) {
		swap(str_origin, str_target);
	}

	int sizeO = str_origin.size();
	int sizeT = str_target.size();
	for (int i = 1; i <= sizeO; i++)
	{
		dp[i][0] = i;
		dp[0][i] = i;
	}

	for (int i = 1; i <= sizeO; i++) {
		for (int j = 1; j <= sizeT; j++) {
			int temp = min(dp[i-1][j] + 1, dp[i][j - 1] + 1);
			dp[i][j] = min(temp,dp[i-1][j-1] + ((str_origin[i - 1] == str_target[j - 1]) ? 0 : 1));
		}
	}
	cout << dp[sizeO][sizeT];
	system("pause");
}