#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

int** memo;

int poker(int i, int j, vector<int>& ii, vector<int>& jj){
    if(i < 0 || j < 0)
        return 0;

    if(memo[i][j] != -1)
        return memo[i][j];

    //cout << "i: " << i << "j: " << j << endl;
    int a, b, c = 0;
    if(ii[i] == jj[j])
        c = 1;

    a = poker(i - 1, j, ii, jj);
    b = poker(i, j -1, ii, jj);
    c += poker(i - 1, j -1, ii, jj);
    
    memo[i][j] = max(max(a, b), c);
    return memo[i][j];
}

void testcases(){
    int n_of_stacks;
    cin >> n_of_stacks;

    int stacks_sizes[n_of_stacks];
    vector<int> stacks[n_of_stacks];

    for(int i = 0; i < n_of_stacks; i++){
        cin >> stacks_sizes[i];
    }

    for(int i = 0; i < n_of_stacks; i++){
        for(int j = 0; j < stacks_sizes[i]; j++){
            //cout << "(" << i << "," << j << ")\n";
            int x;
            cin >> x;
            stacks[i].push_back(x);
        }
    }

    //cout << n_of_stacks << "stacks\n";
    assert(n_of_stacks <= 2);
    if(n_of_stacks == 1){
        cout << 0 << endl;
        return;
    }

    memo = new int*[stacks_sizes[0]];
    
        for(int i = 0; i < stacks_sizes[0]; i++){
            memo[i] = new int[stacks_sizes[1]];
            for(int j = 0; j < stacks_sizes[1]; j++){
                memo[i][j] = -1;
            }
        }

    cout << poker(stacks_sizes[0] - 1, stacks_sizes[1] - 1, stacks[0], stacks[1]) << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        testcases();
    }
}