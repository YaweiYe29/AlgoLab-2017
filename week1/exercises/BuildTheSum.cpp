#include<iostream>

using namespace std;

void computeSum(){
    int m;
    double finalSum = 0, tmp = 0;
    cin >> m;

    for(int i = 0; i < m; i++){
        cin >> tmp;
        finalSum += tmp;
    }

    cout << finalSum << endl;
}

int main(){
    int n;
    cin >> n;

    for(int i = 0; i < n; i++){
        computeSum();
    }

    return 0;
}