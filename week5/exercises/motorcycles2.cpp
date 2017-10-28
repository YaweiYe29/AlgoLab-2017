#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

typedef struct Motorcycle{
    int128_t up;                  //distance in Y axis
    int128_t length;              //distance in X axis
    int128_t startPosition;       //position on Y axis
    int index;                    //index
} Motorcycle;

// compares "angles" of two riders.
// I am trying to avoid the division
int cmp(Motorcycle m1, Motorcycle m2){
    return m1.up * m2.length > m1.length * m2.up;
}

// compares two start positions of motorcycles
int cmp2(Motorcycle m1, Motorcycle m2){
    return m1.startPosition > m2.startPosition;
}

// compares two motorcycles by indexes
int cmp3(Motorcycle m1, Motorcycle m2){
    return m1.index < m2.index;
}

// compares abs "angles" of two riders.
int absCmp(Motorcycle m1, Motorcycle m2){
    return abs(m1.up * m2.length) > abs(m1.length * m2.up);
}

// checks if two riders have the same "angle"
int eq(Motorcycle m1, Motorcycle m2){
    return m1.up * m2.length == m1.length * m2.up;
}

// checks if two riders have the same "angle" in absolute value
int absEq(Motorcycle m1, Motorcycle m2){
    return abs(m1.up * m2.length) == abs(m1.length * m2.up);
}

void Ride(){
    int n;
    int128_t startY, destX, destY;
    Motorcycle smallest;

    // reads input
    cin >> n;
    Motorcycle *cycles = new Motorcycle[n];

    for(int i = 0; i < n; i++){
        cin >> startY >> destX >> destY;
        cycles[i].startPosition = startY;
        cycles[i].up = destY - startY;
        cycles[i].length = destX;
        cycles[i].index = i;
    }

    sort(&cycles[0], cycles + n, cmp2);
    vector<Motorcycle> result;

    smallest = cycles[0];

    for(int i = 0; i < n; i++){
        if(cmp(smallest, cycles[i]) || eq(smallest, cycles[i])){                // if my angle is smaller than the smallest than add and update smallest
            smallest = cycles[i];
            result.push_back(smallest);
        } else if(absEq(smallest, cycles[i]) || absCmp(smallest, cycles[i])){   // if I am not smaller but in absolute value I am smaller than remove previous ones 
            int j = result.size() - 1;
            for(; j >= 0; j--){
                if((absEq(result.back(), cycles[i]) && !eq(result.back(), cycles[i]) && cycles[i].up > 0) || ((absCmp(result.back(), cycles[i]) && cmp(cycles[i], result.back())))){
                    result.pop_back();
                } else 
                    break;
            }

            if(result.size() == 0 || cmp(result.back(), cycles[i]) || eq(result.back(), cycles[i])){    // if there are no result or I am smaller than smallest than add and update
                smallest = cycles[i];
                result.push_back(smallest);
            }
        } 
    }

    //sort and print
    sort(result.begin(), result.end(), cmp3);
    for(int i = 0; i < result.size(); i++)
        cout << result[i].index << " ";
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