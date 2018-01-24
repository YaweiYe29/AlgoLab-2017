#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <map>
#include <iomanip>

typedef long long ll;
using namespace std;
/*  100 points idea
* we need to use split and list globally
* first we need to solve first half and store all pairs of <int,int> (sum first team and sum second team) to vector
* than we need to sort the vector
* than we will iterate through second part find all pairs and for each pair check if there exists such a pair in first half such that sum of both teams is equal
*/

void testcase(){
    int n, k;
    cin >> n >> k;
    vector<ll> skills(n);                             // load skills
    for(int i = 0; i < n; i++){
        cin >> skills[i];
    }
    int N1 = n/2, N2 = n - N1;
    std::map<std::pair<std::pair<ll,ll>,ll>, ll> pairs;         // map (team1_sum, team2_sum, selected) -> count
    for(int i = 0; i < 1 << N1; i++){
        std::vector<ll> selected;
        for(int j = 0; j < N1; j++){
            if(i & 1 << j){
                selected.push_back(skills[j]);
            }
        }
        int ssize = selected.size();
        for(int j = 0; j < 1 << ssize; j++){
            ll team1 = 0, team2 = 0;
            for(int k = 0; k < ssize; k++){
                if(j & 1 << k){
                    team1 += selected[k];
                } else {
                    team2 += selected[k];
                }
            }
            auto team = make_pair(make_pair(team1,team2), ssize);
            if(pairs.find(team) == pairs.end())
                pairs[team] = 1;
            else
                pairs[team]++;
        }
    }
    std::map<ll, vector<pair<ll,ll>>> finalMap;
    for(auto it = pairs.begin(); it != pairs.end(); ++it){
        finalMap[it->first.first.first - it->first.first.second].push_back(make_pair(it->first.second, it->second));
    }
    ll counter = 0;
    for(int i = 0; i < 1 << N2; i++){
        std::vector<ll> selected;
        for(int j = 0; j < N2; j++){
            if(i & 1 << j){
                selected.push_back(skills[j + N1]);
            }
        }
        int ssize = selected.size();
        for(int j = 0; j < 1 << ssize; j++){
            ll team1 = 0, team2 = 0;
            for(int k = 0; k < ssize; k++){
                if(j & 1 << k){
                    team1 += selected[k];
                } else {
                    team2 += selected[k];
                }
            }
            auto team = team2 - team1;
            if(finalMap.find(team) != finalMap.end()){
                auto vec = finalMap[team];
                for(auto it = vec.begin(); it != vec.end(); ++it){
                    if(ssize + it->first >= n - k)
                        counter += it->second;
                }
            }
        }
    }
    cout << setprecision(17) << counter << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}