#include<iostream>

using namespace std;

void printMatrice(int** matrice, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << matrice[i][j] << " ";
        }
        cout << endl;
    }
}

void EvenMatrice(){
    int n, even = 0;
    cin >> n;
    int** matrice = new int*[n];
    int** partialSums = new int*[n];

    for(int i = 0; i < n; i++){
        matrice[i] = new int[n];
        partialSums[i] = new int[n];
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> matrice[i][j];
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int val = matrice[i][j];
            if(i == 0){
                partialSums[i][j] = partialSums[i][j - 1] + val;
            }
            else if(j == 0){
                partialSums[i][j] = partialSums[i - 1][j] + val;
            }
            else{
                partialSums[i][j] = partialSums[i - 1][j] + partialSums[i][j - 1] - partialSums[i - 1][j - 1] + val;
            }
        }
    }

    //printMatrice(partialSums, n);

    for(int i1 = 0; i1 < n; i1++){
        for(int i2 = i1; i2 < n; i2++){
            for(int j1 = 0; j1 < n; j1++){
                for(int j2 = j1; j2 < n; j2++){
                    int sum = 0;
                    if(i1 == 0 && j1 == 0){
                        sum = partialSums[i2][j2];
                    } else if(i1 == 0){
                        sum = partialSums[i2][j2] - partialSums[i2][j1 - 1];
                    } else if(j1 == 0){
                        sum = partialSums[i2][j2] - partialSums[i1 - 1][j2];
                    } else {
                        sum = partialSums[i2][j2] + partialSums[i1 - 1][j1 - 1] - partialSums[i1 - 1][j2] - partialSums[i2][j1 - 1];
                    }

                    if(sum % 2 == 0){
                        even++;
                    }
                }
            }
        }
    }

    /*for(int i1 = 0; i1 < n; i1++){
        for(int i2 = i1; i2 < n; i2++){
            for(int j1 = 0; j1 < n; j1++){
                for(int j2 = j1; j2 < n; j2++){
                    int sum = 0;
                    for(int i = i1; i <= i2; i++){
                        for(int j = j1; j <= j2; j++){
                            sum += matrice[i][j];
                        }
                    }
                    if(sum % 2 == 0)
                        even++;
                }
            }
        }
    }*/
    cout << even << endl;
    //printMatrice(matrice, n);
}

int main(){
    int numOfTests;
    cin >> numOfTests;

    for(int i = 0; i < numOfTests; i++){
        EvenMatrice();
    }
    return 0;
}