#include <stdio.h>
#include <iostream>
#include <cstring>

using namespace std;

long long int dp[15000];

signed main() {

    memset(dp, 0, sizeof(dp));
    dp[1] = 0;
    dp[2] = 1;
    dp[3] = 2;

    for (int i = 4; i < 15000; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    int n, a;

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", &a);
        cout << dp[a] << endl;
    }
    return 0;
}