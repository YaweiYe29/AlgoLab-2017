#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
    int Z, J;
    cin >> Z >> J;
    vector<int> jobs(J);
    vector<pair<int,int>> tickets(Z, pair<int,int>(0,0));
    for(int i = 0; i < Z; i++){
        cin >> tickets[i].first;
    }
    for(int i = 0; i < J; i++){
        cin >> jobs[i];
    }
    int sum = 0;
    for(int i = 0; i < J; i++){
        int n;
        cin >> n;
        if(n == 0)
            sum += jobs[i];
        for(int j = 0; j < n; j++){
            int ticket;
            cin >> ticket;
            tickets[ticket].second += jobs[i];
        }
    }
    for(int i = 0; i < Z; i++){
        sum += max(0, tickets[i].second - tickets[i].first);
    }
    cout << sum << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}