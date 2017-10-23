#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace std;

int* coins;
int** memo;

int get_memo(int i, int j){
    return memo[i][j];
}

void set_memo(int i, int j, int value){
    memo[i][j] = value;
}

int ChooseCoin(int left, int right){
    if(left == right){
        return coins[left];
    }
    if(left > right)
        return 0;

    if(get_memo(left, right) == -1){
        set_memo(left, right, max(min(ChooseCoin(left + 2, right), ChooseCoin(left + 1, right - 1)) + coins[left],
               min(ChooseCoin(left, right - 2), ChooseCoin(left + 1, right - 1)) + coins[right]));
    }
    return get_memo(left, right);
}

void BurnCoins(){
    int n;
    cin >> n;
    coins = new int[n];
    memo = new int*[n];
    for(int i = 0; i < n; i++){
        cin >> coins[i];
        memo[i] = new int[n];
        for(int j = 0; j < n; j++){
            memo[i][j] = -1;
        }
    }
    cout << ChooseCoin(0, n - 1) << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        BurnCoins();
    }
    return 0;
}