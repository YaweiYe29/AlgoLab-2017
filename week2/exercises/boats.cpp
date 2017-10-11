#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

struct Boat{
    int ring;
    int length;
};

static int currLeft;

int Cmp(Boat boat1, Boat boat2){
    return std::max(currLeft + boat1.length, boat1.ring) < std::max(currLeft + boat2.length, boat2.ring);
}

void CalcBoats(){
    currLeft = -100000;
    int wizardsCount, finalCount = 0;
    cin >> wizardsCount;
    Boat boats[wizardsCount];

    for(int i = 0; i < wizardsCount; i++){
        cin >> boats[i].length;
        cin >> boats[i].ring;
    }

    for(int i = 0; i < wizardsCount; i++){
        std::sort(&boats[i], boats + wizardsCount, Cmp);

        int left = std::max(boats[i].ring - boats[i].length, currLeft);
        int right = left + boats[i].length;

        if(right > boats[i].ring + boats[i].length)
            continue;

        currLeft = right;
        //cout << "(" << left << ";" << right << ") -> " << currLeft << endl;
        finalCount++;
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