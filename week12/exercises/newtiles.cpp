#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool canPlace(vector<vector<bool>> &floor, int i, int j){
    return floor[i][j] && floor[i+1][j] && floor[i][j+1] && floor[i+1][j+1];
}

int prune(vector<vector<bool>> &floor, vector<vector<int>> & memo, int i, int j, int h, int w){
    if(memo[i][j] != -1)
        return memo[i][j];
    int j2 = j;
    int placed = 0;
    for(int i2 = i; i2 < h - 3; i2+=2){
        while(j2 < w - 3){
            if(canPlace(floor, i2, j2)){
                placed++;
                j2+=2;
            } else 
                j2++;
        }
        j2 = 0;
    }
    return placed;
}

void testcase(){
    int h, w;
    cin >> h >> w;
    if(h < 4 || w < 4){
        for(int i = 0; i < h; i++){
            for(int j = 0; j < w; j++){
                int in;
                cin >> in;
            }
        }
        cout << "0.00" << endl;
        return;
    }

    vector<vector<bool>> floor(h-2, vector<bool>(w-2));
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            int in;
            cin >> in;
            if(i != 0 && j != 0 && i != h -1 && j != w -1){
                floor[i-1][j-1] = in == 1 ? true : false;
            }
        }
    }
    int maxTiles = 0;
    vector<vector<int>> memo(h-3, vector<int>(w-3, -1));
    for(int i = 0; i < h-3; i++){
        for(int j = 0; j < w-3; j++){
            int res = prune(floor, memo, i, j, h, w);
            memo[i][j] = res;
            maxTiles = max(maxTiles, res);
        }
    }

    for(auto it = memo.begin(); it != memo.end(); ++it){
        for(auto it2 = it->begin(); it2 != it->end(); ++it2){
            cout << *it2 << " ";
        }
        cout << endl;
    }
    cout << maxTiles << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}