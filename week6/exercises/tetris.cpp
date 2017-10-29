#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
/*

Solution to tetris problem without max-flow.
Get just 10/100 but still dont understand why...


*/
using namespace std;

int cmp(pair<int,int> a1, pair<int,int> a2){
    return (a1.second - a1.first) < (a2.second - a2.first);
}

void testcases(){
    int w, n, x, y, start, end, n_of_rows = 0;
    cin >> w >> n;
    set<int> cracks;
    map<int, vector<pair<int ,int> > > bricks;
    vector<pair<int, int> > row;

    int crack = 0;

    for(int i = 0; i < n; i++){
        cin >> x >> y;
        start = min(x, y);
        end = max(x, y);

        bricks[start].push_back(pair<int, int>(start, end));
    }

    for(int i = 0; i < n; i++){
        if(bricks[i].size() > 0){
            sort(bricks[i].begin(), bricks[i].end(), cmp);
            //cout << i << ": [";
            //for(int j = 0; j < bricks[i].size(); j++){
                //cout << "(" << bricks[i][j].first << "," << bricks[i][j].second << ") ";
            //}
            //cout << "]\n";
        }
    }
    
    while(true){
        /*cout << "[";
        for(int i = 0; i < row.size(); i++){
            cout << "(" << row[i].first << "," << row[i].second << ") ";
        }
        cout << "]\n";*/

        //cout << "crack: " << crack << endl;
        if(crack != 0 && crack != n && cracks.find(crack) != cracks.end()){
            if(row.size() > 0)
                row.pop_back();

            if(row.size() > 0)
                crack = row.back().second;
            else
                crack = 0;
        }

        if(crack == w){
            n_of_rows++;
            //cout << "[";
            //for(int i = 0; i < row.size(); i++){
                //cout << "(" << row[i].first << "," << row[i].second << ") ";
            //}
            //cout << "]\n";
            for(int i = 0; i < row.size() - 1; i++){
                cracks.insert(row[i].second);
            }
            row.clear();
            crack = 0;
        }

        if(bricks[crack].size() > 0){
            row.push_back(bricks[crack].back());
            bricks[crack].pop_back();
            crack = row.back().second;
        } else if(crack == 0)
            break;
        else {
            if(row.size() > 0){
                row.pop_back();
                crack = row.back().second;
            } else
                crack = 0;
        }
    }

    std::cout << n_of_rows << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        testcases();
    }
    return 0;
}