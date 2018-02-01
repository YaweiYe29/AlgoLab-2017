/*Solution worth 100 points*/
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <cassert>

using namespace std;

const int MAX_INPUT_SIZE = 1000000;

void testcase(){
    int n;
    cin >> n;
    vector<int> parasols(n);
    for(int i = 0; i < n; i++){
        cin >> parasols[i];
    }
    sort(parasols.begin(), parasols.end());
    int idxLast = 0, max_s = 0, max_d = 300;
    vector<int> finalD;
    for(int i = 0; i < n; i++){
        while(parasols[i] - parasols[idxLast] > 200)
            idxLast++;
        int count = i - idxLast + 1;
        int sum = parasols[i] + parasols[idxLast];
        int middle = sum / 2;
        if(sum <= 0 && sum % 2 != 0){
            middle--;
        }
        int dist = parasols[i] - middle;


        if(count > max_s || (count == max_s && dist < max_d)){
            finalD.clear();
            max_d = dist;
            max_s = count;
            finalD.push_back(middle);
            if(sum % 2 != 0)
                finalD.push_back(middle + 1);
        } else if (count == max_s && dist == max_d){
            finalD.push_back(middle);
            if(sum % 2 != 0)
                finalD.push_back(middle + 1);
        }
    }
    cout << max_s << " " << max_d << endl;
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