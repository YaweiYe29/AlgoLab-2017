#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>

using namespace std;

/*void MakeCounts(int index, int* counts, int n){
    if(index == 0 || index == n)
        return;

    if(counts[index - 1] > counts[index]){
        int sum = counts[index - 1] + counts[index];
        int sum2 = (int)sum/2;

        counts[index] = counts[index - 1] = sum2;
        if(sum % 2 == 1)
            counts[index]++;

        MakeCounts(index - 1, counts, n);
    }
    MakeCounts(index + 1, counts, n);
}*/

void MakeCounts(int index, int* counts, int n){
    int max = 0, maxIndex, min = 1234567890;

    while(max == 0 || (max - min > 1 && maxIndex != n - 1)){
        max = 0;
        min = 1234567890;
        for(int i = 0; i < n; i++){
            if(counts[i] > counts[i + 1] && i < n - 1){
                int sum = counts[i] + counts[i + 1];
                int sum2 = (int)sum/2;
    
                counts[i] = counts[i + 1] = sum2;
                if(sum % 2 == 1)
                    counts[i + 1]++;
            }
            if(counts[i] >= max){
                max = counts[i];
                maxIndex = i;
            }
            if(counts[i] <= min)
                min = counts[i];
        }
    }
}

void books(){
    int n, m, finalT, j = 0;
    cin >> n >> m;
    int* friends = new int[n];
    int* counts = new int[n];
    int* boxes = new int[m];

    for(int i = 0; i < n; i++){
        cin >> friends[i];
        counts[i] = 0;
    }

    for(int i = 0; i < m; i++){
        cin >> boxes[i];
    }

    std::sort(&friends[0], friends + n);
    std::sort(&boxes[0], boxes + m);

    for(int i = 0; i < m; i++){
        while(boxes[i] > friends[j] && j < n)
            j++;

        if(j == n){
            cout << "impossible" << endl;
            return;
        } else 
            counts[j]++;
    }

    MakeCounts(1, counts, n);

    int max = -1;
    for(int i = 0; i < n; i++){
        if(counts[i] > max)
            max = counts[i];
    }

    cout<< max * 3 - 1 << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;

    for(int i = 0; i < n; i++){
        books();
    }
    return 0;
}