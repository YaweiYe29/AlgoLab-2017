#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

struct Jedi{
    int a;
    int b;
    int length;
};

int cmp(Jedi& a, Jedi& b){
    return a.a < b.a;
}

static int currLeft;

int GetLength(Jedi j, int nSeg){
    if(j.a <= j.b)
        return j.b - j.a;
    return nSeg - j.a + j.b; 
}

void Test(){
    //n - jedi count, m - n. of segments
    int n, m, finalCount = 0, currRight = INT_MIN, prevLength = INT_MAX, currLength = INT_MAX, start = -1;
    currLeft = 0;

    cin >> n >> m;

    Jedi jedis[n];
    for(int i = 0; i < n; i++){
        cin >> jedis[i].a >> jedis[i].b;
        jedis[i].length = GetLength(jedis[i], m);
    }
    

    sort(&jedis[0], jedis + n, cmp);

    int maxIndex, max = INT_MIN, startA = 0;
    for(int i = 0; i < n; i++){
        if(jedis[i].a + jedis[i].length > max){
            max = jedis[i].a + jedis[i].length;
            maxIndex = i;
        }
    }

    while(true){
        if(jedis[startA].a >= jedis[maxIndex].b)
            break;
        else
            startA++;
    }

    if(jedis[maxIndex].a + jedis[maxIndex].length <= m)
        startA = 0;

    int i = startA, startAA = jedis[startA].a;
    int j = 0, k = 0;
    cout << "MAX: " << jedis[maxIndex].b << " index " << startA << endl; 
    while(true){
        if(j == n)
            break;
        int tmpI = i % n;
        cout << tmpI << endl;
        if(jedis[tmpI].a > currRight && (currRight < m || currRight % m < startAA))
        {
            prevLength = currLength;
            currRight = jedis[tmpI].b;
            currLength = jedis[tmpI].length;
            finalCount++;
            k++;
            cout << " added currRight: " << endl;
        } else {
            int right = jedis[tmpI].b;
            if(right < currRight && prevLength > currLength){
                currRight = right;
                if(k == 1 && jedis[tmpI].a > startAA)
                    startAA = jedis[tmpI].a;
                cout << " currRight lowered " << endl;
            } else
                cout << "no operation " << endl;
        }

        cout << "JEDI: " << jedis[tmpI].a << " " << jedis[tmpI].b << " Curr length: " << currLength << " Curr right: " << currRight << " Prev length: " << prevLength << endl;

        if(tmpI == startA - 1)
            break;

        i++;
        j++;
    }

    cout << "RESULT: " << finalCount << endl;
}

int main(){
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        Test();
    }
    return 0;
}