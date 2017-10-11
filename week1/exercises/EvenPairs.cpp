#include<iostream>
/*
gcc -lstdc++ filename.cpp
./filename.out
*/
using namespace std;

void CountEvenPairs()
{
    int n, odd = 0, even = 0, tmp, partial = 0;
    cin >> n;

    for(int i = 0; i < n; i++){
        cin >> tmp;
        partial += tmp;
        if(partial % 2 == 0)
            even++;
        else
            odd++;
    }

    cout << ((even * (even - 1) / 2) + (odd * (odd - 1) / 2) + even) << endl;
}


int main()
{
    int numOfTestCases;
    cin >> numOfTestCases;

    for(int i = 0; i < numOfTestCases; i++){
        CountEvenPairs();
    }

    return 0;
}