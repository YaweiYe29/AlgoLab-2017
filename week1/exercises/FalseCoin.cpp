#include<iostream>
#include <algorithm>
#include<cstring>

using namespace std;

void FindFalseCoin(){
    int n, k;
    cin >> n;
    cin >> k;
    char coins[n];
    char pCoins[n];
    memset(coins, '-', n);

    for(int i = 0; i < k; i++){
        int pi;
        cin >> pi;

        int leftPan[pi], rightPan[pi];
        char weighing;
        memset(pCoins, '0', n);

        for(int j = 0; j < pi; j++){
            cin >> leftPan[j];
        }
        for(int j = 0; j < pi; j++){
            cin >> rightPan[j];
        }
        cin >> weighing;
        if(weighing == '='){
            for(int j = 0; j < pi; j++){
                coins[leftPan[j] - 1] = '=';
                coins[rightPan[j] - 1] = '=';
            }
        }
        else{
            for(int j = 0; j < pi; j++){
                int index = leftPan[j] - 1;
                pCoins[index] = '1';
                char val = coins[index];
                if(val == '=')
                    continue;
                if(val == '-')
                    coins[index] = weighing;
                else if(val != weighing)
                    coins[index] = '=';
            }
            if(weighing == '<')
                weighing = '>';
            else
                weighing = '<';

            for(int j = 0; j < pi; j++){
                int index = rightPan[j] - 1;
                pCoins[index] = '1';
                char val = coins[index];
                if(val == '=')
                    continue;
                if(val == '-')
                    coins[index] = weighing;
                else if(val != weighing)
                    coins[index] = '=';
            }

            for(int j = 0; j < n; j++){
                if(pCoins[j] != '1'){
                    coins[j] = '=';
                }
            }
        }
        /*for(int ii = 0; ii < n; ii++){
            cout << coins[ii] << " ";
        }
        cout << endl;

        for(int ii = 0; ii < n; ii++){
            cout << pCoins[ii] << " ";
        }
        cout << endl;*/
    }
    int fina = -1;
    for(int i = 0; i < n; i++){
        if(fina == -1 && coins[i] != '='){
            fina = i + 1;
        }else if(coins[i] != '='){
            fina = 0;
        }
    }
    cout << fina << endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int numOfTests;
    cin >> numOfTests;

    for(int i = 0; i < numOfTests; i++){
        FindFalseCoin();
    }
    return 0;
}