#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
/*Program analysis:
 * In comparison to planks2 solution for 60 point we needed to actually count possibilities.
 * To accomplish that we need to remember groups i.e. sides of the squred and indexes which were taken for each side
 * We do it in first part of split and list
 * In the second part we list again but find number of solutions that fit to current one: (equal range)
 * We sum number of these solutions up
 * But we need to divide them by 24 because of rule in paper. If these numbers just switch side of the square its the same possibility.
 * There are 4*3*2*1 combinations over square.
 * */
using namespace std;
typedef long long ll;
typedef pair<vector<ll>, vector<ll>> IPair;

vector<IPair> memo;
int cmp(const IPair &p1, const IPair &p2){
    return p1.first < p2.first;
}

void dp(vector<ll> &arr, int index, ll sum1, ll sum2, ll sum3, ll sum4, ll side,
            ll s1, ll s2, ll s3, ll s4){
    if(sum1 > side || sum2 > side || sum3 > side || sum4 > side)
        return;

    if(index >= arr.size()/2){
        IPair pp = IPair({sum1, sum2, sum3, sum4}, {s1, s2, s3, s4});
        memo.push_back(pp);
        return;
    }
    s1 |= 1LL << index;   dp(arr, index+1, sum1+arr[index], sum2, sum3, sum4, side, s1, s2, s3, s4);  s1 &= ~(1LL << index);
    s2 |= 1LL << index;   dp(arr, index+1, sum1, sum2+arr[index], sum3, sum4, side, s1, s2, s3, s4);  s2 &= ~(1LL << index);
    s3 |= 1LL << index;   dp(arr, index+1, sum1, sum2, sum3+arr[index], sum4, side, s1, s2, s3, s4);  s3 &= ~(1LL << index);
    s4 |= 1LL << index;   dp(arr, index+1, sum1, sum2, sum3, sum4+arr[index], side, s1, s2, s3, s4);  s4 &= ~(1LL << index);
}

ll counter;
void dp2(vector<ll> &arr, int index, ll sum1, ll sum2, ll sum3, ll sum4, ll side,
            ll s1, ll s2, ll s3, ll s4){
    if(sum1 > side || sum2 > side || sum3 > side || sum4 > side)
        return;
    if(index >= arr.size()){
        IPair needed = IPair({side-sum1, side-sum2, side-sum3, side-sum4}, {s1, s2, s3, s4});
        vector<IPair>::iterator ebeg, eend;
        tie(ebeg, eend) = std::equal_range(memo.begin(), memo.end(), needed, cmp);
        counter += eend-ebeg;
        return;
    } 
    s1 |= 1LL << index;   dp2(arr, index+1, sum1+arr[index], sum2, sum3, sum4, side, s1, s2, s3, s4);  s1 &= ~(1LL << index);
    s2 |= 1LL << index;   dp2(arr, index+1, sum1, sum2+arr[index], sum3, sum4, side, s1, s2, s3, s4);  s2 &= ~(1LL << index);
    s3 |= 1LL << index;   dp2(arr, index+1, sum1, sum2, sum3+arr[index], sum4, side, s1, s2, s3, s4);  s3 &= ~(1LL << index);
    s4 |= 1LL << index;   dp2(arr, index+1, sum1, sum2, sum3, sum4+arr[index], side, s1, s2, s3, s4);  s4 &= ~(1LL << index);
}

void testcase(){
    int n;
    ll side = 0; 
    counter = 0;
    memo.clear();
    cin >> n;
    vector<ll> arr(n);
    for(int i = 0; i < n; i++){
        cin >> arr[i];
        side += arr[i];
    }
    side /= 4;
    ll s1, s2, s3, s4;
    s1 = s2 = s3 = s4 = 0;
    dp(arr, 0, 0, 0, 0, 0, side, s1, s2, s3, s4);
    sort(memo.begin(), memo.end(), cmp);
    s1 = s2 = s3 = s4 = 0;
    dp2(arr, arr.size() / 2, 0, 0, 0, 0, side, s1, s2, s3, s4);
    cout << counter/24 << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}