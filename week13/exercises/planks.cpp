#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/* naive approach */
int dp(vector<int> &arr, int index, long sum1, long sum2, long sum3, long sum4, long side){
    if(sum1 > side || sum2 > side || sum3 > side || sum3 > side)
        return 0;
    if(index == arr.size()){
        return sum1 == sum2 && sum2 == sum3 && sum3 == sum4;
    } 
    return max(max(dp(arr, index+1, sum1+arr[index], sum2, sum3, sum4, side), dp(arr, index+1, sum1, sum2+arr[index], sum3, sum4, side)),
               max(dp(arr, index+1, sum1, sum2, sum3+arr[index], sum4, side), dp(arr, index+1, sum1, sum2, sum3, sum4+arr[index], side)));
}

void testcase(){
    int n;
    long side = 0;
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        cin >> arr[i];
        side += arr[i];
    }
    side /= 4;
    cout << dp(arr, 0, 0, 0, 0, 0, side) << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}