/* Idea worth 20/100 points -> recursion is too slow*/
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Room{
    int on;
    int off;

    void swap(){
        int tmp = on;
        on = off;
        off = tmp;
    }
};

struct Circuit{
    vector<Room> rooms;
public: Circuit(){}
public: Circuit(int m){
    rooms = vector<Room>(m);
}
    void swap(){
        for(auto it = rooms.begin(); it != rooms.end(); ++it){
            it->swap();
        }
    }
};

int* target;
int* tmpTarget;
Circuit* circuit;
int globalMin;

bool match(int m, int pos, bool modified){
    if(modified){
        for(int i = 0; i < m; i++){        
            tmpTarget[i] = tmpTarget[i] - circuit[pos].rooms[i].off + circuit[pos].rooms[i].on;
        }
    }

    for (int i = 0; i < m; i++){
        if(tmpTarget[i] != target[i])
            return false;
    }
    return true;
}

void switchC(bool sw, int pos, int m, int n, int nOfSwitched){
    if(pos >= n)
        return;

    if(sw)
        circuit[pos].swap();

    if(match(m, pos, sw) && globalMin > nOfSwitched)
        globalMin = nOfSwitched;

    switchC(false, pos + 1, m, n, nOfSwitched);
    switchC(true, pos + 1, m, n, nOfSwitched + 1);

    if(sw){
        for(int i = 0; i < m; i++){
            tmpTarget[i] = tmpTarget[i] + circuit[pos].rooms[i].off - circuit[pos].rooms[i].on;
        }

        circuit[pos].swap();
    }
}

void light(){
    int n, m;
    cin >> n >> m;
    globalMin = INT32_MAX;
    target = new int[m];
    tmpTarget = new int[m];
    circuit = new Circuit[n];

    for(int i = 0; i < m; i++){
        cin >> target[i];
    }

    for(int i = 0; i < n; i++){
        circuit[i] = Circuit(m);
        for(int j = 0; j < m; j++){
            cin >> circuit[i].rooms[j].on >> circuit[i].rooms[j].off;
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(i == 0)
                tmpTarget[j] = 0;

            tmpTarget[j] += circuit[i].rooms[j].on;
        }
    }

    switchC(true, 0, m, n, 1);
    switchC(false, 0, m, n, 0);

    if(globalMin == INT32_MAX)
        cout << "impossible\n";
    else
        cout << globalMin << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        light();
    }
}