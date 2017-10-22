#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace std;

int* coins;
int** memo;
int hits, misses;

int get_memo(int i, int j){
    return memo[i][j];
}
int nn;
void print(){
    for(int i = 0; i < nn; i++){
        for(int j = 0; j < nn; j++){
            cout << memo[i][j] << " ";
        }
        cout << endl;
    }
}

int ChooseCoin(int left, int right){
    if(left >= right)
        return 0;

    int ll = get_memo(left + 1, right) + coins[left];

    if(ll == -1){
        ll = ChooseCoin(left + 1, right) + coins[left];
        //memo[left + 1][right] = ll;
        misses++;
    }

    int rr = get_memo(left, right - 1) + coins[right];

    if(rr == -1){
        rr = ChooseCoin(left, right - 1) + coins[right];
        //memo[left][right - 1] = rr;
        misses++;
    }

    memo[left][right] = max(ll,rr);

    if(ll > rr)
        left++;
    else
        right--;


    if(get_memo(left,right) == -1){
        memo[left][right] = max(ChooseCoin(left + 1, right) + coins[left], ChooseCoin(left, right - 1) + coins[right]);
        misses++;
    }
    else
        hits++;
    return memo[left][right];
}

void BurnCoins(){
    int n;
    cin >> n;
    nn = n;
    coins = new int[n];
    memo = new int*[n];
    hits = 0;
    misses = 0;
    for(int i = 0; i < n; i++){
        cin >> coins[i];
        memo[i] = new int[n];
        for(int j = 0; j < n; j++){
            memo[i][j] = -1;
        }
    }
    cout << max(ChooseCoin(1, n - 1) + coins[0], ChooseCoin(0, n - 2) + coins[n - 1]) << endl;
    //cout << "hits: " << hits << " misses: " << misses << endl;
    print();
}

int main(){
    int n;
    cin >> n;
    while(n--){
        BurnCoins();
    }
    return 0;
}