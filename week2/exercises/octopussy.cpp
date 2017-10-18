#include<iostream>
#include<algorithm>

using namespace std;

int times;

struct Ball{
    int index;
    int timer;
};

bool possible(int index, int* balls, int n, char* solved){
    if(solved[index] == 'S')
        return true;
    if(solved[index] == 'X')
        return false;
    if(index >= ((n - 1) / 2)){
        if(balls[index] > times){
            solved[index] = 'S';
            times++;
            return true;
        } else {
            return false;
            solved[index] = 'X';
        }
    }

    bool first = false, second = false;
    if(balls[2*index + 1] < balls[2*index +2]){
        first = possible(2*index + 1, balls, n, solved);
        second = possible(2*index + 2, balls, n, solved);
    }
    else{
        second = possible(2*index + 2, balls, n, solved);
        first = possible(2*index + 1, balls, n, solved);
    }
    if(first && second){
        if(balls[index] > times){
            solved[index] = 'S';
            times++;
            return true;
        } else{
            solved[index] = 'X';
            return false;
        }
    } else {
        solved[index] = 'X';
        return false;
    }
}

int Cmp(Ball b1, Ball b2){
    return b1.timer < b2.timer;
}

void Solve(){
    times = 0;
    int n;
    cin >> n;

    int balls[n];
    char solved[n];
    Ball sortedBalls[n];

    for(int i = 0; i < n; i++){
        cin >> balls[i];
        sortedBalls[i].index = i;
        sortedBalls[i].timer = balls[i];
        solved[i] = 'N';
    }

    std::sort(&sortedBalls[0], sortedBalls + n, Cmp);

    for(int i = 0; i < n; i++){
        if(!possible(sortedBalls[i].index, balls, n, solved)){
            cout << "no" << endl;
            return;
        }
    }
    cout << "yes" << endl;
}

int main(){
    int m;
    cin >> m;

    for(int i = 0; i < m; i++){
        Solve();
    }
    return 0;
}