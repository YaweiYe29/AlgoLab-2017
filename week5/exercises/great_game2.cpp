/* solution worth 75 points*/
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

std::vector<std::vector<int>> memoR; // memo first dimension is 0 (for even moves) and 1 for odd moves for red meeple
std::vector<std::vector<int>> memoB; // memo first dimension is 0 (for even moves) and 1 for odd moves for black meeple

int playB(int move, int b, std::vector<std::vector<int>>& graph, int n){
    if(b == n - 1)                              // if we reached end return n of moves
        return move;

    if(memoB[move%2][b] != -1)                   // if memo contains value return it
        return memoB[move%2][b] + move;

    int result;
    if(move % 2 != 0){                          // sherlock -> black, moriarty -> red
        int maxB = 0;
        for(auto it = graph[b].begin(); it != graph[b].end(); ++it){    // try all paths from current node and pick the maximal one
            int moves = playB(move + 1, *it, graph, n);
            maxB = std::max(moves, maxB);
        }
        result = maxB;
    } else { // sherlock -> red, moriarty -> black
        int minB = INT32_MAX;                                           // try all paths from current node and pick the minimal one
        for(auto it = graph[b].begin(); it != graph[b].end(); it++){
            int moves = playB(move + 1, *it, graph, n);
            minB = std::min(moves, minB);
        }
        result = minB;
    }
    memoB[move%2][b] = result - move;            // save to memo
    return result;
}

int playR(int move, int r, std::vector<std::vector<int>>& graph, int n){
    if(r == n - 1)                              // if we reached end return n of moves
        return move;

    if(memoR[move%2][r] != -1)                   // if memo contains value return it
        return memoR[move%2][r] + move;

    int result;
    if(move % 2 != 0){                          // sherlock -> red, moriarty -> black
        int maxB = 0;
        for(auto it = graph[r].begin(); it != graph[r].end(); ++it){    // try all paths from current node and pick the maximal one
            int moves = playR(move + 1, *it, graph, n);
            maxB = std::max(moves, maxB);
        }
        result = maxB;
    } else {                                   // sherlock -> black, moriarty -> red
        int minB = INT32_MAX;                                           // try all paths from current node and pick the minimal one
        for(auto it = graph[r].begin(); it != graph[r].end(); it++){
            int moves = playR(move + 1, *it, graph, n);
            minB = std::min(moves, minB);
        }
        result = minB;
    }
    memoR[move%2][r] = result - move;            // save to memo
    return result;
}

void testcase(){
    int n, m, r, b;
    std::cin >> n >> m >> r >> b;
    memoR = std::vector<std::vector<int>>(2, std::vector<int>(n, -1));
    memoB = std::vector<std::vector<int>>(2, std::vector<int>(n, -1));

    std::vector<std::vector<int>> graph(n);
    for(int i = 0; i < m; i++){                 // load graph
        int from, to;
        std::cin >> from >> to;
        graph[from-1].push_back(to-1);
    }
    int movesB = playB(0, b-1, graph, n);       // find best strategy for moriarty
    int movesR = playR(0, r-1, graph, n);       // find best strategy for holmes

    if(movesB < movesR)                         // if moriarty reaches n-1 first he is the winner
        std::cout << "1";
    else if(movesB > movesR)                    // otherwise sherlock
        std::cout << "0";
    else {                                      // if both at the same time than depend on whether move is even or odd
        if(movesR % 2 != 0)
            std::cout << "0";
        else 
            std::cout << "1";
    }
    std::cout << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--)
        testcase();
    return 0;
}