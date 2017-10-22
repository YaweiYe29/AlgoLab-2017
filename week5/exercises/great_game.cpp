#include <iostream>
#include <vector>

using namespace std;

vector<int>* positions;
int holmes_turns;
int moriarty_turns;
bool printed;

void turn(int red_position, int black_position, bool sherlock_move, int n){
    cout << "(" << red_position << "," << black_position << ")\n";
    if(red_position == n /*&& !printed*/){
        cout << "0\n";
        printed = true;
        return;
    }

    if(black_position == n /*&& !printed*/){
        cout << "1\n";
        printed = true;
        return;
    }


    if(sherlock_move){
        if(holmes_turns % 2 == 0){
            holmes_turns++;
            for(int i = 0; i < positions[red_position].size(); i++)
                turn(positions[red_position][i], black_position, !sherlock_move, n);
        }
        else {
            holmes_turns++;
            for(int i = 0; i < positions[black_position].size(); i++)
                turn(red_position, positions[black_position][i], !sherlock_move, n);
        }

    } else {
        if(moriarty_turns % 2 == 0){
            moriarty_turns++;
            for(int i = 0; i < positions[black_position].size(); i++)
                turn(red_position, positions[black_position][i], !sherlock_move, n);
        }
        else {
            moriarty_turns++;
            for(int i = 0; i < positions[red_position].size(); i++)
                turn(positions[red_position][i], black_position, !sherlock_move, n);
        }
    }
}

void play(){
    holmes_turns = 0;
    moriarty_turns = 0;
    printed = false;
    
    // red, black meeple | even - holmes(red) -moriarty(black) | holmes starts
    int n, m, r, b, from, to;
    cin >> n >> m >> r >> b;
    positions = new vector<int>[n];
    for(int i = 0; i < m; i++){
        cin >> from >> to;
        positions[from - 1].push_back(to - 1);
    }

    turn(r - 1, b - 1, true, n - 1);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        play();
    }
}