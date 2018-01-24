/*primitive solution worth 30 points*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
    // n -> defence players, m -> attackers, k -> power of attackers
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> defence(n);
    vector<int> sums(n);
    vector<int> blocks(n, 0);
    for(int i = 0; i < n; i++){
        cin >> defence[i];
    }
    sums[0] = defence[0];
    for(int i = 1; i < n; i++){
        sums[i] = sums[i-1] + defence[i];
    }
    int j = 0, i = 0;
    // sliding window O(n)
    while(i < n && j < n){
        int power = sums[i] - sums[j] + defence[j];
        if(power == k){
            blocks[i] = i - j + 1;
            i++; j++;
        } else if (power > k){
            j++;
        } else {
            i++;
        }
    }
    vector<vector<int>> DP(n+1, vector<int>(m+1, 0));
    DP[1][1]=blocks[0];
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            DP[i][j] = max(DP[i - blocks[i - 1]][j - 1] + blocks[i - 1], DP[i - 1][j]);
        }
    }
    if(DP[n][m] != DP[n][m-1])
        cout << DP[n][m] << endl;
    else
        cout << "fail\n";
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}