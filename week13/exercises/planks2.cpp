#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<long>> memo;
int possible;
/* split and list approach */
void dp(vector<int> &arr, int index, long sum1, long sum2, long sum3, long sum4, long side){
    if(sum1 > side || sum2 > side || sum3 > side || sum4 > side)
        return;
    
    if(index >= arr.size()/2){
        memo.push_back({sum1, sum2, sum3, sum4});
        return;
    } 
    dp(arr, index+1, sum1+arr[index], sum2, sum3, sum4, side);
    dp(arr, index+1, sum1, sum2+arr[index], sum3, sum4, side);
    dp(arr, index+1, sum1, sum2, sum3+arr[index], sum4, side);
    dp(arr, index+1, sum1, sum2, sum3, sum4+arr[index], side);
}

void dp2(vector<int> &arr, int index, long sum1, long sum2, long sum3, long sum4, long side){
    if(sum1 > side || sum2 > side || sum3 > side || sum4 > side)
        return;
    if(index >= arr.size()){
        vector<long> needed = {side-sum1, side-sum2, side-sum3, side-sum4};
        auto it = lower_bound(memo.begin(), memo.end(), needed);
        if(*it == needed){
            possible++;
        }
        return;
    } 
    dp2(arr, index+1, sum1+arr[index], sum2, sum3, sum4, side);
    dp2(arr, index+1, sum1, sum2+arr[index], sum3, sum4, side);
    dp2(arr, index+1, sum1, sum2, sum3+arr[index], sum4, side);
    dp2(arr, index+1, sum1, sum2, sum3, sum4+arr[index], side);
}

void testcase(){
    int n;
    long side = 0;
    memo.clear();
    possible = 0;
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        cin >> arr[i];
        side += arr[i];
    }
    side /= 4;
    dp(arr, 0, 0, 0, 0, 0, side);
    sort(memo.begin(), memo.end());

    dp2(arr, arr.size() / 2, 0, 0, 0, 0, side);
    cout << (possible > 0 ? "1" : "0") << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}