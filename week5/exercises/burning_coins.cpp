#include <iostream>
#include <algorithm>
#include <map>
#include <utility>

using namespace std;

int * coins;

map<pair<int, int>, int> memo;

int ChooseCoin(int left, int right){
    if(left >= right)
        return 0;
    if(coins[left] > coins[right])
        left++;
    else
        right--;

    if(memo.find(pair<int,int>(left,right)) == memo.end())
        memo[pair<int,int>(left,right)] = max(ChooseCoin(left + 1, right) + coins[left], ChooseCoin(left, right - 1) + coins[right]);

    return memo[pair<int,int>(left,right)];
}

void BurnCoins(){
    int n;
    cin >> n;
    coins = new int[n];
    memo.clear();
    for(int i = 0; i < n; i++){
        cin >> coins[i];
    }
    cout << max(ChooseCoin(1, n - 1) + coins[0], ChooseCoin(0, n - 2) + coins[n - 1]) << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        BurnCoins();
    }
    return 0;
}