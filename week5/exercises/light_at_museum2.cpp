#include <vector>
#include <climits>
#include <iostream>
#include <algorithm>
#include <bitset>

using namespace std;

// get how much light we need for each room for configuration S
vector<int> get_needed(vector<vector<pair<int,int>>> &museum, vector<int> curr_needed, int S, int n, int m){
    vector<int> needed = curr_needed;
    for(int i = 0; i < n; i++){
        if(S & 1 << i){
            for(int j = 0; j < m; j++){
                needed[j] = needed[j] + museum[i][j].first - museum[i][j].second;
            }
        }
    }
    return needed;
}

void testcase(){
    int n, m;       // m -> number of rooms
    cin >> n >> m;  // n -> number of switches
    vector<int> target(m);
    for(int i = 0; i < m; i++){ // load target lights
        cin >> target[i];
    }
    vector<vector<pair<int,int>>> museum(n, vector<pair<int, int>>(m));
    vector<int> rooms(m, 0);
    vector<int> curr_needed(target);
    for(int i = 0; i < n; i++){     // load museum light scheme
        for(int j = 0; j < m; j++){
            int on, off;
            cin >> on >> off;
            museum[i][j] = make_pair(on, off);
            rooms[j] += on;
        }
    }

    for(int i = 0; i < m; i++){     // get how much we need when no action
        curr_needed[i] -= rooms[i];
    }

    int minTurns = INT32_MAX;
    int H1 = n / 2, H2 = n - H1;
    int size = 1 << H1;
    vector<pair<vector<int>,int>> needed_1(size);
    vector<vector<int>> needed_2;
    for(int i = 0; i < 1 << H1; i++){   // list and split. Compute first half
        bitset<30> bs(i);
        auto res = get_needed(museum, curr_needed, i, n, m);
        needed_1[i] = make_pair(res, bs.count());
    }

    sort(needed_1.begin(), needed_1.end());
    for(int i = 0; i < 1 << H1; i++){
        needed_2.push_back(needed_1[i].first);
    }
    
    for(int i = 0; i < 1 << H2; i++){   // compute second half and test if there exist subsolution in first half that will together form solution
        int moves = 0;
        vector<int> d(m, 0);
        for(int j = H1; j < n; j++){
            if(i & (1 << (j - H1))){
                moves++;
                for(int k = 0; k < m; k++){
                    d[k] = d[k] + museum[j][k].second - museum[j][k].first;
                }
            }
        }

        auto it = lower_bound(needed_2.begin(), needed_2.end(), d);
        if(*it == d){
            int idx = it - needed_2.begin();
            minTurns = min(minTurns, moves + needed_1[idx].second);
        }
    }

    if(minTurns == INT32_MAX)
        cout << "impossible";
    else 
        cout << minTurns;
    cout << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--)
        testcase();
    return 0;
}