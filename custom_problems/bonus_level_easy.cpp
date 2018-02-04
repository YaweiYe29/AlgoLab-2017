#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
vector<vector<int>> memo;
/*On style of bonus level from week10 but we go just one way.*/
int rec(int x, int y, vector<vector<int>> & table, int n, int m){
    if(x >= n || y >= m)
        return 0;
    if(memo[x][y] != -1)
        return memo[x][y];
    memo[x][y] = table[x][y] + max(rec(x, y + 1, table, n, m), rec(x + 1, y, table, n, m));
    return memo[x][y];
}

void testcase(){
    int n, m;
    cin >> n >> m;
    vector<vector<int>> table(n, vector<int>(m));
    memo = vector<vector<int>>(n, vector<int>(m, -1));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> table[i][j];
        }
    }
    cout << rec(0, 0, table, n, m) << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << memo[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}