#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// ***************** 30 points ********************

int rec_back(int i, int j, int ** matrix, int n){
    int res;
    if(i == 0 && j == 0)
        res = matrix[i][j];
    else if(i == 0)
        res = matrix[i][j] + rec_back(i, j-1, matrix, n);
    else if(j == 0)
        res = matrix[i][j] + rec_back(i-1, j, matrix, n);
    else
        res = matrix[i][j] + max(rec_back(i-1, j, matrix, n), rec_back(i, j-1, matrix, n));
    return res;
}

int rec_to(int i, int j, int ** matrix, int n){
    int res;
    int value = matrix[i][j];
    matrix[i][j] = 0;
    if(i == n - 1 && j == n - 1)
        res = value + rec_back(i, j, matrix, n); 
    else if(i == n - 1)
        res = value + rec_to(i, j+1, matrix, n);
    else if(j == n - 1)
        res = value + rec_to(i+1, j, matrix, n);
    else
        res = value + max(rec_to(i+1, j, matrix, n), rec_to(i, j+1, matrix, n));
    matrix[i][j] = value;

    return res;
}

// ***************** 100 points *******************
vector<vector<vector<int> > > memo;

int rec(int diagonal, int i1, int i2, int ** matrix, int n){
    //std::cout << "diagonal: " << diagonal << " i1: " << i1 << " i2: " << i2 << endl;
    int j1 = diagonal - i1;
    int j2 = diagonal - i2;
    if(memo[diagonal][i1][i2] != -1)
        return memo[diagonal][i1][i2];

    int value = matrix[i1][j1];
    matrix[i1][j1] = 0;
    int res;
    if(i1 == n-1 && i1 == n-1 && j1 == n-1 && j2 == n-1)
        res = value;
    else{
        int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
        if(i1 != n-1 && j2 != n-1)
            t1 = rec(diagonal+1, i1+1, i2, matrix, n);
        if(i1 != n-1 && i2 != n-1)
            t2 = rec(diagonal+1, i1+1, i2+1, matrix, n);
        if (j1 != n-1 && j2 != n-1)
            t3 = rec(diagonal+1, i1, i2, matrix, n);
        if (i2 != n-1 && j1 != n-1)
            t4 = rec(diagonal+1, i1, i2+1, matrix, n);
        res = value + matrix[i2][j2] + max(max(t1, t2), max(t3, t4));
    }

    memo[diagonal][i1][i2] = res;
    matrix[i1][j1] = value;
    return res;
}

void testcase(){
    int n;
    cin >> n;
    memo = vector<vector<vector<int>>>(2*n, vector<vector<int>>(n, vector<int>(n, -1)));
    int ** matrix;
    matrix = new int*[n];
    for(int i = 0; i < n; i++){
        matrix[i] = new int[n];
        for(int j = 0; j < n; j++){
            cin >> matrix[i][j];
        }
    }
    //std::cout << rec_to(0, 0, matrix, n) << endl; // 30 points
    std::cout << rec(0, 0, 0, matrix, n) << endl;   // 100 points
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}