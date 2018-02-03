#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
/*Program analysis:
 * Table DP where state[i][j] represents max attacked defenders from first i players attacking first j defenders
 * To find "attackable" blocks we do sliding window technique with precomputed sums which takes O(n) time
 * Base cases are state[0][x] which are all 0 (0 players can attack 0 blocks)
 * And state[x][0] which are equal to blocks[0] i.e. whether we can attack first player
 * Running time O(n*m)
 * */
void testcase(){
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> defence_sum(n), defence(n);
    cin >> defence[0];
    defence_sum[0] = defence[0];
    for(int i = 1; i < n; i++){
        cin >> defence[i];
        defence_sum[i] = defence[i] + defence_sum[i-1];
    }
    int head = 0, tail = 0;
    vector<int> blocks(n, 0);
    while(head < n){
        int diff = defence_sum[head] - defence_sum[tail] + defence[tail];
        if(diff == k){
            blocks[head] = head - tail + 1;
            head++; tail++;
        } else if (diff < k){
            head++;
        } else {
            tail++;
        }
    }
    vector<vector<int>> DP(m+1, vector<int>(n));
    for(int i = 0; i < n; i++){
        DP[0][i] = 0;
    }
    for(int i = 1; i < m+1; i++){
        DP[i][0] = blocks[0];
    }

    for(int i = 1; i < m + 1; i++){
        for(int j = 1; j < n; j++){
            DP[i][j] = max(blocks[j] + DP[i-1][j - blocks[j]], DP[i][j-1]);
        }
    }
    if(DP[m - 1][n - 1] != DP[m][n - 1])
        cout << DP[m][n - 1];
    else
        cout << "fail";
    cout << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}