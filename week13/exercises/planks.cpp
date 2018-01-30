#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
/* naive approach */
int dp(vector<int> &arr, int index, long sum1, long sum2, long sum3, long sum4){
    if(index == arr.size()){
        return sum1 == sum2 && sum2 == sum3 && sum3 == sum4;
    } 
    return max(max(dp(arr, index+1, sum1+arr[index], sum2, sum3, sum4), dp(arr, index+1, sum1, sum2+arr[index], sum3, sum4)),
               max(dp(arr, index+1, sum1, sum2, sum3+arr[index], sum4), dp(arr, index+1, sum1, sum2, sum3, sum4+arr[index])));
}

void testcase(){
    int n;
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++) cin >> arr[i];
    cout << dp(arr, 0, 0, 0, 0, 0) << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}