#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/*Program analysis
 * State in DP represents pair(min price, max dist beverages) for volume i and beverages j
 * State DP[0][0] is (0,0)
 * Next states are DP[x][0] for all 0 <= x < k + max_volume; which represents minimal prices just with first beverage
 * Now for each state we are going to minimize price and maximize beverages
 * We can either take 
 * */
void testcase(){
    int n, k;                               // n -> distinct beverages, k -> people coming to the party
    cin >> n >> k;
    vector<int> costs(n), volumes(n);
    int maxb = 0;
    for(int i = 0; i < n; i++){             // load costs and volumes
        cin >> costs[i] >> volumes[i];
        maxb = max(maxb, volumes[i]);
    }
    vector<vector<pair<int,int>>> DP(k + maxb, vector<pair<int,int>>(n, pair<int,int>(0,0)));
    for(int i = 1; i < k + maxb; i++){      // precompute min prices just with 1 beverage
        if((DP[i-1][0].first/costs[0]) * volumes[0] >= i)
            DP[i][0] = DP[i-1][0];
        else
            DP[i][0] = make_pair(DP[i-1][0].first + costs[0], 1);
    }

    for(int i = 1; i < k + maxb; i++){
        for(int j = 1; j < n; j++){
            int price = DP[i][j-1].first;               // take price as for one bev. less
            int bev_count = DP[i][j-1].second;          // take count as for one bev. less
            if(price > costs[j] && volumes[j] >= i){    // if its cheaper to take just one of this kind take it
                price = costs[j];
                bev_count = 1;
            }
            if(i - volumes[j] > 0){                     // if we can access memo
                int p = DP[i - volumes[j]][j].first;
                int b = DP[i - volumes[j]][j].second;
                if(p == DP[i - volumes[j]][j-1].first)  // if they have the same price
                    b = max(b, DP[i - volumes[j]][j-1].second + 1); // maximize beverage count
                p += costs[j];                          // plus cost of curr beverage
                if(price > p){                          // if its cheaper take it
                    price = p;
                    bev_count = b;
                } else if (price == p)                  // if price is the same than maximize beverages count
                    bev_count = max(bev_count, b);
            }
            DP[i][j] = make_pair(price, bev_count);
        }
    }
    cout << DP[k][n-1].first << " " << DP[k][n-1].second << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}