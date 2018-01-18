#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

// memo getters and setters
vector<vector<vector<vector<vector<int>>>>> memo;
int get_memo(vector<int> &p){
    return memo[p[0]][p[1]][p[2]][p[3]][p[4]];
}
void set_memo(vector<int> &p, int value){
    memo[p[0]][p[1]][p[2]][p[3]][p[4]] = value;
}

int dp(vector<int> &positions, vector<vector<int>> &stacks, int n){
    for(int i = 0; i < n; i++){
        if(positions[i] < 0)
            return 0;
    }
    int memo_val = get_memo(positions);
    if (memo_val != -1)
        return memo_val;

    int limit = 1 << n;
    int points = 0;
    for(int i = 1; i < limit; i++){
        map<int,int> selected_colors;
        bool cont = false;
        for(int j = 0; j < n; j++){
            if(i & 1 << j){
                if(positions[j] == 0){      // empty stack.. this combination is not allowed!
                    cont = true;            // if position == 0 than stack is empty
                    break;
                }
            }
        }
        if(cont) continue;                  // if we chose combination which takes chip from empty stack we omit this combination

        for(int j = 0; j < n; j++){
            if(i & 1 << j){                 // count color counts in the turn
                selected_colors[stacks[j][positions[j] - 1]]++;
                positions[j]--;
            }
        }
        int pts = 0;                        // and add points
        for(auto it = selected_colors.begin(); it != selected_colors.end(); ++it){
            if(it->second > 1){
                pts += pow(2, it->second - 2);
            }
        }
        pts += dp(positions, stacks, n);    // plus points in next turns
        for(int j = 0; j < n; j++){
            if(i & 1 << j){
                positions[j]++;
            }
        }
        points = max(points, pts);          // maximize turn
    }
    set_memo(positions, points);
    return points;
}

void testcase(){
    int n;
    cin >> n;
    vector<vector<int>> stacks(n);
    vector<int> counts(5, 0);
    vector<int> positions(5, 0);
    for(int i = 0; i < n; i++){                 // load stack sizes
        cin >> counts[i];
        stacks[i].reserve(counts[i]);
        positions[i] = counts[i];
    }
    vector<vector<vector<int>>> tmp = vector<vector<vector<int>>>(3, vector<vector<int>>(2, vector<int>(3)));       // huge memo suited for max 5 stacks
    memo = vector<vector<vector<vector<vector<int>>>>>(counts[0] + 1,                                               // each dimension for each stack
                  vector<vector<vector<vector<int>>>>(counts[1] + 1,                                                // if there are less stacks than 5
                         vector<vector<vector<int>>>(counts[2] + 1,                                                 // than last dimensions are of size 1
                                vector<vector<int>>(counts[3] + 1, 
                                       vector<int>(counts[4] + 1, -1)))));
    for(int i = 0; i < n; i++){                 // load stacks
        for(int j = 0; j < counts[i]; j++){
            int color;
            cin >> color;
            stacks[i].push_back(color);
        }
    }
    cout << dp(positions, stacks, n) << endl;
    /*for(int i1 = 0; i1 < counts[0] + 1; i1++){                                                                    // debug output
        for(int i2 = 0; i2 < counts[1] + 1; i2++){
            for(int i3 = 0; i3 < counts[2] + 1; i3++){
                for(int i4 = 0; i4 < counts[3] + 1; i4++){
                    for(int i5 = 0; i5 < counts[4] + 1; i5++){
                        cout << i1 << i2 << i3 << i4 << i5 << " -> " << memo[i1][i2][i3][i4][i5] << endl;
                    }
                }
            }
        }
    }*/
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        testcase();
    }
    return 0;
}