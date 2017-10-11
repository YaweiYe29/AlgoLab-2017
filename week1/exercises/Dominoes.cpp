#include<iostream>
using namespace std;

void RunDominoes(){
    int n;
    cin >> n;
    int arr[n];
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }

    int max = 0;
    for(int i = 0; i < n; i++){
        int val = arr[i] + i + 1;
        if(max != 0 && i + 1 >= max){
            cout << i << endl;
            break;
        }

        if(val > max){
            max = val;
        }
        if(i == n -1){
            cout << n << endl;
        }
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int numOfTests;
    cin >> numOfTests;
    for(int i = 0; i < numOfTests; i++){
        RunDominoes();
    }
    return 0;
}