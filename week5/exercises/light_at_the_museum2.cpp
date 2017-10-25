#include <iostream>
#include <vector>
#include <climits>
#include <queue>

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
    Room* rooms;
    int m;
public: Circuit(){}
public: Circuit(int m){
    rooms = new Room[m];
    this->m = m;
}
    void swap(){
        for(int i = 0; i < m; i++){
            rooms[i].swap();
        }
    }
};

struct State{
    vector<bool> states;
    int pos;
};

int* target;
int* tmpTarget;
Circuit* circuit;
int globalMin;
queue<State> toProcess;

bool match(int m, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(i == 0)
                tmpTarget[j] = 0;

            tmpTarget[j] += circuit[i].rooms[j].on;
        }
    }

    for(int i = 0; i < m; i++){
        if(tmpTarget[i] != target[i])
            return false;
    }
    return true;
}

void switchC(State state, int m, int n){
    int switched = 0;
    for(int i = 0; i < n; i++){
        if(state.states[i]){
            circuit[i].swap();
            switched++;
        }
    }

    if(match(m, n)){
        globalMin = switched;
        return;
    }

    for(int i = state.pos + 1; i < n; i++){
        State new_state;
        new_state.pos = i;
        state.states[i] = true;
        new_state.states = state.states;
        state.states[i] = false;
        toProcess.push(new_state);
    }

    for(int i = 0; i < n; i++){
        if(state.states[i])
            circuit[i].swap();
    }
}

void clear(queue<State> &q)
{
   std::queue<State> empty;
   std::swap( q, empty );
}

void light(){
    int n, m;
    cin >> n >> m;
    globalMin = INT_MAX;
    target = new int[m];
    tmpTarget = new int[m];
    circuit = new Circuit[n];
    clear(toProcess);
    for(int i = 0; i < m; i++){
        cin >> target[i];
    }

    for(int i = 0; i < n; i++){
        circuit[i] = Circuit(m);
        for(int j = 0; j < m; j++){
            cin >> circuit[i].rooms[j].on >> circuit[i].rooms[j].off;
        }
    }

    State firstState;
    firstState.pos = -1;
    firstState.states = vector<bool>(n);

    switchC(firstState, m, n);
    while(!toProcess.empty()){
        if(globalMin != INT_MAX){
            cout << "\nResult: " << globalMin << endl;
            return;
        }
        switchC(toProcess.front(), m, n);
        toProcess.pop();
    }

    cout << "\nimpossible\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        light();
    }
    return 0;
}