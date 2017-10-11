#include<iostream>
#include<math.h>

using namespace std;

void Cards(){
    int n, k, finalI = -1, finalJ = -1, finalSum = INT_MAX;
    cin >> n;
    cin >> k;

    int numbers[n], sums[n];

    for(int i = 0; i < n; i++){
        cin >> numbers[i];
    }
    sums[0] = numbers[0];
    for(int i = 1; i < n; i++){
        sums[i] = sums[i - 1] + numbers[i];
    }

    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            int sum = sums[j] - sums[i - 1];
            if(i == j)
                sum = numbers[i];
            else if (i == 0)
                sum = sums[j];

            if(sum == k){
                cout << i << " " << j << endl;
                return;
            }
            else if (abs(k - sum) < abs(k - finalSum)){
                finalSum = sum;
                finalI = i;
                finalJ = j;
            }
        }
    }

    cout << finalI << " " << finalJ << endl;
}

int main(){
    int n;
    cin >> n;

    for(int i =0; i < n; i++){
        Cards();
    }
    return 0;
}