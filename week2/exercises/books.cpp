#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>

using namespace std;
int global_times = 0;
void books(){
    int n, m;
    cin >> n >> m;
    int friends[n];
    vector<int> boxes(m);

    for(int i = 0; i < n; i++){
        cin >> friends[i];
    }
    for(int i = 0; i < m; i++){
        cin >> boxes[i];
    }

    std::sort(&friends[0], friends + n, std::greater<int>());
    std::sort(boxes.begin(), boxes.end(), std::greater<int>());

    int nTimes = 0, takenBoxes = 0, prevBoxes = 0, maxLoad = n;
    while(takenBoxes <= m - 1){
        int j = 0, i = 0;
        while (i < boxes.size() && j < maxLoad){
            global_times++;
            if(boxes[i] <= friends[j]){
                j++;
                boxes.erase(boxes.begin() + i);
                i--;
                takenBoxes++;
            }
            i++;
        }
        maxLoad = j + 1;
        if(takenBoxes == prevBoxes){
            cout << "impossible" << endl;
            return;
        }
        else
            prevBoxes = takenBoxes;
            
        nTimes++;
    }

    cout << nTimes * 3 - 1 << endl; 
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