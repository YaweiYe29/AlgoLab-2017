#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
/*primitive solution worth 30p done in 4 minutes*/
void testcase(){
    int n, ii, k;
    cin >> n >> ii >> k;
    vector<ll> disks(n);
    for(int i = 0; i < n; i++){
        cin >> disks[i];
    }

    for(int i = 1; i < 1 << n; i++){
        ll sum = 0;
        for(int j = 0; j < n; j++){
            if(i & 1 << j)
                sum += disks[j];
        }
        if(sum % k == ii){
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