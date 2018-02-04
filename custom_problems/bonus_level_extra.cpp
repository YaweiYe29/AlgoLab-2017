#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/*Task similar to bonus level from week10 but we go forth, back, forth i.e. three times*/
vector<vector<vector<vector<int>>>> memo;
int rec(int diag, int x1, int x2, int x3, int n, vector<vector<int>> & grid){
    if(x1 >= n || x2 >= n || x3 >= n || diag >= 2*n - 1 || diag - x1 >= n || diag - x2 >= n || diag - x3 >= n)
        return 0;
    if(memo[diag][x1][x2][x3] != -1)
        return memo[diag][x1][x2][x3];
    int val = grid[diag - x1][x1] + grid[diag - x2][x2] + grid[diag - x3][x3];
    if(x1 == x2 && x2 == x3)
        val = grid[diag - x1][x1];
    else if (x1 == x2)
        val = grid[diag - x1][x1] + grid[diag - x3][x3];
    else if (x2 == x3)
        val = grid[diag - x1][x1] + grid[diag - x2][x2];
    else if (x1 == x3)
        val = grid[diag - x1][x1] + grid[diag - x2][x2];
    int val2 = max(max(max(rec(diag+1, x1, x2, x3, n, grid), rec(diag+1, x1, x2, x3+1, n, grid)), 
                       max(rec(diag+1, x1, x2+1, x3, n, grid), rec(diag+1, x1+1, x2+1, x3+1, n, grid))),
                   max(max(rec(diag+1, x1+1, x2, x3, n, grid), rec(diag+1, x1+1, x2, x3+1, n, grid)),
                       max(rec(diag+1, x1+1, x2+1, x3, n, grid), rec(diag+1, x1+1, x2+1, x3+1, n, grid))));
    memo[diag][x1][x2][x3] = val + val2;
    return val + val2;
}

void testcase(){
    int n;
    cin >> n;
    vector<vector<int>> grid(n, vector<int>(n));
    memo = vector<vector<vector<vector<int>>>>(2*n-1, vector<vector<vector<int>>>(n, vector<vector<int>>(n, vector<int>(n, -1))));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> grid[i][j];
        }
    }
    cout << rec(0, 0, 0, 0, n, grid) << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}