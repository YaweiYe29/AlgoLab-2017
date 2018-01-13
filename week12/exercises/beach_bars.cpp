/*Solution worth 40 points*/
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

const int MAX_INPUT_SIZE = 1000000;

void testcase(){
    int n, maxP = INT32_MIN, minP = INT32_MAX, range;
    vector<int> reachables(2*MAX_INPUT_SIZE, 0);
    vector<int> maxDist(2*MAX_INPUT_SIZE, 0);
    vector<int> finalD;
    cin >> n;
    vector<int> parasols(n);
    for(int i = 0; i < n; i++){ // O(200*n) -> bad!
        cin >> parasols[i];
        for(int j = parasols[i] - 100; j <= parasols[i] + 100; j++){
            if(j <= MAX_INPUT_SIZE && j >= -MAX_INPUT_SIZE){
                reachables[MAX_INPUT_SIZE+j]++;
                maxDist[MAX_INPUT_SIZE+j] = max(maxDist[MAX_INPUT_SIZE+j], abs(j - parasols[i]));
            }
        }
    }
    int maxIndex, maxParasols = 0, maxDistance = 0;
    for(int i = 0; i < 2*MAX_INPUT_SIZE; i++){
        if(reachables[i] > maxParasols){
            maxParasols = reachables[i];
            maxDistance = maxDist[i];
            maxIndex = i;
            finalD.clear();
            finalD.push_back(i - MAX_INPUT_SIZE);
        } else if(reachables[i] == maxParasols && maxDist[i] < maxDistance){
            maxIndex = i;
            maxDistance = maxDist[i];
            finalD.clear();
            finalD.push_back(i - MAX_INPUT_SIZE);
        } else if(reachables[i] == maxParasols && maxDist[i] == maxDistance){
            finalD.push_back(i - MAX_INPUT_SIZE);
        }
    }
    cout << maxParasols << " " << maxDistance << endl;
    sort(finalD.begin(), finalD.end());
    for(auto it = finalD.begin(); it != finalD.end(); ++it){
        cout << *it << " ";
    }
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