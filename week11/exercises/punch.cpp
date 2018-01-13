/*Idea from https://github.com/timethy/algolab/blob/master/week11/punch/punch.cpp*/
#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

void testcase() {
    int n, k;
    cin >> n >> k;
    vector<int> volumes(n), costs(n);
    for(int i = 0; i < n; i++){
        cin >> costs[i] >> volumes[i];
    }
    int maxVolume = k + *max_element(volumes.begin(), volumes.end());           // max possible volume needed
    vector<vector<pair<int,int>>> memo(n, vector<pair<int,int>>(maxVolume));    // DP table
    memo[0][0] = make_pair(0,0);                                                // (cost, number of distinct beverages)
    for(int i = 1; i < maxVolume; i++){
        int ntimes = (i + volumes[0] - 1) / volumes[0];                         // how many packs of beverage 0 we need
        memo[0][i] = make_pair(ntimes * costs[0], 1);                           // cost of volume i only with beverage 0
    }
    for(int i = 1; i < n; i++){
        memo[i][0] = memo[i-1][0];
        for(int j = 1; j < maxVolume; j++){ // for every possible volume we will try if the cost can be minimized by replace one beverage with ith one
            auto not_added = memo[i -1][j];
            int min_cost, max_bev;
            if(j < volumes[i]){             // if targeted volume is smaller then the one with the beverage than take it
                min_cost = costs[i];
                max_bev = 1;
            } else {                        // else find state where volume is targeted volume - volume of curr beverage. And try this cost and n. of beverages
                auto added = memo[i][j - volumes[i]];  
                min_cost = added.first + costs[i];
                max_bev = added.second + (added == memo[i-1][j - volumes[i]] ? 1 : 0);
            }
            if(min_cost > not_added.first){ // if its cheaper to keep the same beverage do it
                min_cost = not_added.first;
                max_bev = not_added.second;
            } else if(min_cost == not_added.first){ // if costs are the same than take the one with more distinct beverages
                max_bev = max(max_bev, not_added.second);
            }
            memo[i][j] = make_pair(min_cost, max_bev);
        }
    }

    std::cout << memo[n-1][k].first << " " << memo[n-1][k].second << std::endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}