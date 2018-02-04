#include <iostream>
#include <vector>
#include <algorithm>
/* Program analysis:
 * Traversing square to and back is equal to one traversal with two movements.
 * Interesting insight is that we have to move by one diagonal with every move.
 * Number of diagonals is 2*n - 1 and for every diagonal we have two possitions of our moves.
 * We need to remember just x or y coordiante because other one we can compute from diagonal.*/
using namespace std;
vector<vector<vector<int>>> DP;

int rec(int diag, int x1, int x2, vector<vector<int>> & grid, int n){
    if(x1 >= n || x2 >= n || diag >= 2*n - 1 || diag - x1 >= n || diag - x2 >= n)
        return 0;
    if(DP[diag][x1][x2] != -1)
        return DP[diag][x1][x2];
    int val1 = grid[diag - x1][x1] + grid[diag - x2][x2];
    if(x1 == x2)
        val1 = grid[diag - x1][x1];
    int val2 = max(max(rec(diag + 1, x1 + 1, x2 + 1, grid, n), rec(diag + 1, x1 + 1, x2, grid, n)),
                   max(rec(diag + 1, x1, x2 + 1, grid, n), rec(diag + 1, x1, x2, grid, n)));
    DP[diag][x1][x2] = val1 + val2;
    return DP[diag][x1][x2];
}

void testcase(){
    int n;
    cin >> n;
    vector<vector<int>> grid(n, vector<int>(n, 0));
    DP = vector<vector<vector<int>>>(n*2 - 1, vector<vector<int>>(n, vector<int>(n, -1)));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> grid[i][j];
        }
    }
    cout << rec(0, 0, 0, grid, n) << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}