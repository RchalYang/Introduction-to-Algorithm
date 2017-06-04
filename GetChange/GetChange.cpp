#include<iostream>
#include<algorithm>
using namespace std;

int n, m;

int T[20] = { 0 };
int C[20] = { 0 };

int dp[22000] = { 0 };

int main() {

	cin >> n;

	for (int i = 0; i<n; i++) {
		cin >> T[i] >> C[i];
	}

	cin >> m;

	for (int i = 0; i <= m; i++) {
		dp[i] = 100000;
	}
	dp[0] = 0;

	for (int j = 0; j<n; j++) {
		int *flag = new int[m + 1];
		for (int i = 0; i <= m; i++) {
			flag[i] = 0;
		}
		for (int i = 0; i <= m; i++) {
			if (T[j] <= m && i + T[j] <= m && flag[i]<C[j]) {
				if (dp[i + T[j]] > dp[i] + 1) {
					dp[i + T[j]] = dp[i] + 1;
					flag[i+T[j]]=flag[i]+1;
				}
			}
		}
	}

	if (dp[m] != 100000) {
		cout << dp[m];
	}
	else {
		cout << "-1";
	}

	system("pause");
}