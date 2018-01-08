#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long sum(vector<long> vec){
    long sum = 0;
    for(auto it = vec.begin(); it != vec.end(); ++it){
        sum += *it;
    }
    return sum;
}

// 10 points :(
int dp(vector<long> & players, vector<long> &team1, vector<long> &team2, int pos, int k) {
    if(pos == players.size()){
        int non_assigned = players.size() - team1.size() - team2.size();
        if(non_assigned > k)
            return 0;

        return sum(team1) == sum(team2);
    }

    team1.push_back(players[pos]);
    int left = dp(players, team1, team2, pos + 1, k);
    team1.pop_back();
    int none = dp(players, team1, team2, pos + 1, k);
    team2.push_back(players[pos]);
    int right = dp(players, team1, team2, pos + 1, k);
    team2.pop_back();

    return left + none + right;
}
// 10 points
int dp2(vector<long> & players, long team1, long team2, int assigned, int pos, int k) {
    if(pos == players.size()){
        if(players.size() - assigned > k)
            return 0;

        return team1 == team2;
    }

    team1 += players[pos];
    int left = dp2(players, team1, team2, assigned + 1, pos + 1, k);
    team1 -= players[pos];
    int none = dp2(players, team1, team2, assigned, pos + 1, k);
    team2 += players[pos];
    int right = dp2(players, team1, team2, assigned + 1, pos + 1, k);
    team2 -= players[pos];

    return left + none + right;
}

void testcase(){
    int n, k;
    cin >> n >> k;
    vector<long> players(n, 0);
    vector<long> team1;
    vector<long> team2;

    for(int i = 0; i < n; i++){
        cin >> players[i];
    }
    //cout << dp(players, team1, team2, 0, k) << endl;
    cout << dp2(players, 0, 0, 0, 0, k) << endl;
}

int main() {
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
    return 0;
}