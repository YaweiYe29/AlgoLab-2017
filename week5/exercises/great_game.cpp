#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
// even -> Mor(b), Holmes(r)
// odd -> Mor(r), Holmes(b)

vector<vector<int>> memo;                           // memo first dimension is 0 (for even moves) and 1 for odd moves
int move(int moves, int meeple, int n, vector<vector<int>> & graph){
    if(meeple == n-1)                               // if we reached end return n of moves
        return moves;
    
    if(memo[moves%2][meeple] != -1)                 // if memo contains value return it
        return memo[moves%2][meeple] + moves;
    int result;
    if(moves % 2 != 1){                             // both want to minimize on their even move
        int min_moves = INT32_MAX;                  // try all paths from current node and pick the maximal one
        for(auto it = graph[meeple].begin(); it != graph[meeple].end(); ++it){
            min_moves = min(min_moves, move(moves+1, *it, n, graph));
        }
        result = min_moves;
    } else {                                        // both want to maximize on their odd move
        int max_moves = INT32_MIN;                  // try all paths from current node and pick the minimal one
        for(auto it = graph[meeple].begin(); it != graph[meeple].end(); ++it){
            max_moves = max(max_moves, move(moves+1, *it, n, graph));
        }
        result = max_moves;
    }
    memo[moves%2][meeple] = result - moves;         // save to memo
    return result;
}

void testcase(){
    int n, m, r, b;
    cin >> n >> m >> r >> b;
    r--; b--;
    vector<vector<int>> graph(n);
    memo = vector<vector<int>>(2, vector<int>(n, -1));
    for(int i = 0; i < m; i++){
        int from, to;
        cin >> from >> to;
        from--; to--;
        graph[from].push_back(to);
    }
    int moves_r = move(0, r, n, graph);
    int moves_b = move(0, b, n, graph);
    if(moves_r < moves_b) cout << "0" << endl;      // when there are less moves to red than sherlock wins
    else if(moves_r > moves_b) cout << "1" << endl; // otherwise moriarty
    else if(moves_r % 2 != 0) cout << "0" << endl;  // if num. of moves is equal. Than we need to check with which figure 
    else cout << "1" << endl;                       // Holmes moved in last move. If with red than he won otherwise Moriarty.
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}