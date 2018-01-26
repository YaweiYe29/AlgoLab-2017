#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
/*
 * for 100p we need to use bottom up DP table with n rows(one row per new disk) and k columns(one column per one possible sum)
 * State DP[i][j] represents how many sums of j can be created from first i disks
 * Result is than stored in DP[n-1][ii]
 * Base cases: DP[0][0] = true, DP[0:n][0] = true since we can create sum of 0
*/
void testcase(){
    int n, ii, k;
    cin >> n >> ii >> k;
    vector<int> disks(n);
    for(int i = 0; i < n; i++){
        cin >> disks[i];
        disks[i] %= k;
    }
    vector<vector<bool>> DP(n+1, vector<bool>(k,false));
    for(int i = 1; i <= n; i++){
        for(int j = 0; j < k; j++){
            DP[i][j] = disks[i-1] == j || DP[i-1][(j - disks[i-1] + k) % k] || DP[i - 1][j];
        }   // I can create sum j of first i disks if disk i is equal to sum or if there exists sum without ith disk equal to (sum - ith disks)
    }       // or I can create sum j of first i - 1 disks
    if(DP[n][ii]) cout << "yes\n";
    else cout << "no\n";
}
int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}