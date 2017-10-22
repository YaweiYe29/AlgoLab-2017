#include <iostream>
#include <vector>
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

void set_memo(int i, int j, int value){
    memo[i][j] = value;
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

/*int ChooseCoin(int left, int right, int turn){
    if(left == right){
        set_memo(left, right, 0, turn);
        return coins[left];
    }

    //if(right - left == 1){
      //  int minn = min(coins[right], coins[left]);
        //set_memo(left, right, minn, turn);
       // return minn;
    //}

    int ll = get_memo(left + 1, right, turn + 1);

    if(ll == -1){
        cout << "left: " << left + 1 << " right: " << right << "turn: " << turn % 2 << endl;
        ll = ChooseCoin(left + 1, right, turn + 1);
        set_memo(left + 1, right, ll, turn + 1);
        ll += coins[left];
        misses++;
    } else {
        ll += coins[left];
        hits++;
    }

    int rr = get_memo(left, right - 1, turn + 1);

    if(rr == -1){
        cout << "left: " << left << " right: " << right - 1 << "turn: " << turn % 2 << endl;
        rr = ChooseCoin(left, right - 1, turn + 1);
        set_memo(left, right - 1, rr, turn + 1);
        rr += coins[right];
        misses++;
    } else {
        rr += coins[right];
        hits++;
    }

    //set_memo(left, right, max(ll, rr), turn);
    turn++;
    int tmp = 0;
    if(ll > rr){
        tmp = coins[left];
        left++;
    }
    else{
        tmp = coins[right];
        right--;
    }

    if(get_memo(left,right, turn) == -1){
        tmp = 0;
        cout << "left: " << left << " right: " << right << "turn: " << turn % 2 << endl;
        set_memo(left, right, max(ChooseCoin(left + 1, right, turn + 1) + coins[left], ChooseCoin(left, right - 1, turn + 1) + coins[right]), turn);
        misses++;
    } else {
        hits++;
    }

    return get_memo(left, right, turn) + tmp;
}*/

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
    if(n == 1)
        cout << coins[0] << endl;
    else
        cout << ChooseCoin(0, n - 1) << endl;
        //cout << max(ChooseCoin(1, n - 1, 1) + coins[0], ChooseCoin(0, n - 2, 1) + coins[n - 1]) << endl;
    //cout << "hits: " << hits << " misses: " << misses << endl;
    //print();
    //cout << "----------------\n";
    //print2();
}

int main(){
    int n;
    cin >> n;
    while(n--){
        BurnCoins();
    }
    return 0;
}