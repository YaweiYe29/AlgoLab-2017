#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

using namespace std;

typedef struct Motorcycle{
    int128_t up;                 //distance in Y axis
    int128_t length;             //distance in X axis
    int128_t startPosition;      //position on Y axis
    bool outOfTheGame;              //stops riding
    int index;                      //index
} Motorcycle;

// compares "angles" of two riders.
// I am trying to avoid the division
int cmp(Motorcycle m1, Motorcycle m2){
    return m1.up * m2.length > m1.length * m2.up;
}

// compares abs "angles" of two riders.
int absCmp(Motorcycle m1, Motorcycle m2){
    return abs(m1.up * m2.length) > abs(m1.length * m2.up);
}

// checks if two riders have the same "angle"
int absEq(Motorcycle m1, Motorcycle m2){
    return abs(m1.up * m2.length) == abs(m1.length * m2.up);
}

void Ride(){
    int n;
    int128_t startY, destX, destY;
    cin >> n;
    Motorcycle *cycles = new Motorcycle[n];

    for(int i = 0; i < n; i++){
        cin >> startY >> destX >> destY;
        cycles[i].startPosition = startY;
        cycles[i].up = destY - startY;
        cycles[i].length = destX;
        cycles[i].outOfTheGame = false;
        cycles[i].index = i;
    }

    sort(&cycles[0], cycles + n, cmp);
    vector<int> result;

    for(int i = 0; i < n; i++){
        if(cycles[i].outOfTheGame)
            continue;

        //if rides straight then he will ride forever
        if(cycles[i].up == 0)
            result.push_back(cycles[i].index);
        else {
            bool add = true;
            for(int j = n - 1; j >= 0; j--){
                // if he is out or is me than continue
                if(cycles[j].outOfTheGame || i == j)
                    continue;

                // if he is not out of the game, his is above me and I ride in bigger angle
                if(cycles[i].startPosition < cycles[j].startPosition && cmp(cycles[i], cycles[j])){
                    //if we have the same angles with different signes then we will meet in the same place and so I continue because I am on the right.
                    if(absEq(cycles[i], cycles[j]))
                        cycles[j].outOfTheGame = true;
                    //if I have bigger absolute angle than I am done.
                    else if(absCmp(cycles[i], cycles[j]))
                    {
                        add = false;
                        break;
                    } else // otherwise is the other
                        cycles[j].outOfTheGame = true;
                // if I am below him and he rides steeper down then I up then he is done.
                } else if(cycles[i].startPosition < cycles[j].startPosition && absCmp(cycles[j], cycles[i])) 
                        cycles[j].outOfTheGame = true;
            }
            if(add)
                result.push_back(cycles[i].index);
            else
                cycles[i].outOfTheGame = true;
        }
    }

    //sort and print
    sort(result.begin(), result.end());
    for(int i = 0; i < result.size(); i++)
        cout << result[i] << " ";
    cout << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n--){
        Ride();
    }
    return 0;
}