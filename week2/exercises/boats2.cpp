#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

struct Boat{
    int ring;
    int length;
};

int Cmp(Boat boat1, Boat boat2){
    return boat1.ring < boat2.ring;
}

void CalcBoats(){
    int wizardsCount, finalCount = 0, currRight = INT_MIN, prevRight = INT_MIN;
    cin >> wizardsCount;
    Boat boats[wizardsCount];

    for(int i = 0; i < wizardsCount; i++){
        cin >> boats[i].length;
        cin >> boats[i].ring;
    }

    std::sort(&boats[0], boats + wizardsCount, Cmp);

    for(int i = 0; i < wizardsCount; i++){
        if(boats[i].ring >= currRight)
        {
            prevRight = currRight;
            currRight = std::max(boats[i].ring, boats[i].length + currRight);
            finalCount++;
        } else {
            int right = std::max(boats[i].ring, boats[i].length + prevRight);
            if(right < currRight)
                currRight = right;
        }
    }

    cout << finalCount << endl;
}

int main(){
    int n;
    cin >> n;

    for(int i = 0; i < n; i++){
        CalcBoats();
    }
    return 0;
}