#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;
typedef long long ll;
/*primitive solution worth 60p done in 7 minutes*/
/*split and list*/
void testcase(){
    int n, ii, k;
    cin >> n >> ii >> k;
    vector<ll> disks(n);
    for(int i = 0; i < n; i++){
        cin >> disks[i];
    }
    int N1 = n/2, N2 = n - N1;
    std::set<int> half;
    for(int i = 1; i < 1 << N1; i++){
        ll sum = 0;
        for(int j = 0; j < N1; j++){
            if(i & 1 << j)
                sum += disks[j];
        }
        half.insert(sum % k);
    }
    for(int i = 0; i < 1 << N2; i++){
        ll sum = 0;
        for(int j = 0; j < N2; j++){
            if(i & 1 << j)
                sum += disks[j + N1];
        }
        sum = sum % k;

        if(half.find(ii - sum) != half.end()){
            cout << "yes" << endl;
            return;
        }
    }
    cout << "no" << endl;
}
int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}