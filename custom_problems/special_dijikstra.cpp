#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>

using namespace std;
/*Task without story
 * Given n gates and m connections between gates you have to find shortest cheapest way between from and to gate.
 * On each gate you have to pay for the entrance. Output length and money spent in case you have enough money, 
 * 'not enough money' otherwise
 * First line contains 5 integers: n, m, from, to, money
 * next line contains n integers -> prices of passing gate i
 * next m lines contains 3 integers. From, to and length
 * */
void dijkstra(vector<vector<pair<int,int>>> graph, int start, vector<pair<int,int>> & dist, 
                vector<int> & pred, vector<int> & costs, int n){
    set<int> Q;
    for(int i = 0; i < n; i++){
        dist[i] = make_pair(INT32_MAX, INT32_MAX);    // infinity
        Q.insert(i);
    }
    dist[start] = make_pair(0, costs[start]);
    while(!Q.empty()){
        int u, min_dist = INT32_MAX;
        for(auto it = Q.begin(); it != Q.end(); ++it){
            if(dist[*it].first < min_dist){
                min_dist = dist[*it].first;
                u = *it;
            }
        }
        Q.erase(u);
        for(auto it = graph[u].begin(); it != graph[u].end(); ++it){
            int d = dist[u].first + it->second;
            int cost = dist[u].second + costs[it->first];
            if(d < dist[it->first].first){
                dist[it->first] = make_pair(d, cost);
                pred[it->first] = u; 
            } else if (d == dist[it->first].first){
                if(dist[it->first].second > cost){
                    dist[it->first] = make_pair(d, cost);
                    pred[it->first] = u; 
                }
            }
        }
    }
}

void testcase(){
    int n, m, from, to, money;
    cin >> n >> m >> from >> to >> money;
    vector<int> prices(n);
    vector<pair<int,int>> dist(n);
    vector<int> pred(n);
    vector<vector<pair<int,int>>> graph(n, vector<pair<int,int>>());
    for(int i = 0; i < n; i++){
        cin >> prices[i];
    }
    for(int i = 0; i < m; i++){
        int from, to, weight;
        cin >> from >> to >> weight;
        graph[from].push_back(make_pair(to,weight));
        graph[to].push_back(make_pair(from,weight));
    }
    dijkstra(graph, from, dist, pred, prices, n);
    /*for(int i = 0; i < n; i++){
        cout << i << ": (" << dist[i].first << "," << dist[i].second << ") | ";
    }*/
    if(dist[to].second <= money)
        cout << dist[to].first << " " << dist[to].second;
    else cout << "not enough money";
    cout << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}