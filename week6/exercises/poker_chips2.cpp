#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <string>

using namespace std;

unordered_map<string, int> memo;

string vector_to_string(vector<int> vec){
    string s = "";
    for(int i = 0; i < vec.size(); i++){
        s += std::to_string(vec[i]);
    }
    return s;
}

bool same(vector<int> pos){
    int ii = pos[0];
    for(int i = 0; i < pos.size(); i++)
        if(ii != pos[i])
            return false;

    return true;
}

int poker(vector<int> pos, vector<vector<int> >& stacks){
    cout << "[";
    for(int i = 0; i < pos.size(); i++){
        cout << pos[i] << ",";
    }
    cout << "]\n";

    for(int i = 0; i < pos.size(); i++){
        if(pos[i] < 0)
            return 0;
    }

    string hashed = vector_to_string(pos);
    if(memo.find(hashed) != memo.end())
        return memo[hashed];

    vector<int> res(pow(2, pos.size()));
    cout << "pos.size: " << pos.size() << " pow: " << res.size() << endl;
    for(int i = 0; i < res.size(); i++) {
        vector<int> newPos = pos;
        for(int j = 0; j < pos.size(); j++){
            cout << "i: " << i << " j: " << j << endl;
            if(i >> j & 1){
                cout << "new pos\n";
                newPos[j] = newPos[j] - 1;
            }
        }
        res[i] = poker(newPos, stacks);
        if(same(newPos))
            res[i] += pow(2, pos.size() - 2);
    }
    
    memo[hashed] = *std::max_element(res.begin(), res.end());
    return memo[hashed];
}

void testcases(){
    int n_of_stacks;
    cin >> n_of_stacks;
    memo.clear();

    std::vector<int> stacks_sizes(n_of_stacks);
    std::vector<vector<int > > stacks(n_of_stacks, vector<int>());

    for(int i = 0; i < n_of_stacks; i++){
        cin >> stacks_sizes[i];
    }

    for(int i = 0; i < n_of_stacks; i++){
        for(int j = 0; j < stacks_sizes[i]; j++){
            int x;
            cin >> x;
            stacks[i].push_back(x);
        }
    }

    for(int i = 0; i < n_of_stacks; i++){
        stacks_sizes[i] = stacks_sizes[i] - 1;
    }

    if(n_of_stacks == 1){
        cout << 0 << endl;
        return;
    }

    cout << poker(stacks_sizes, stacks) << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        testcases();
    }
}