/*primitive solution worth 30 points*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct Block{
    int from, to;
    int getPlayers(){ return to - from + 1; }
    void print (){ cout << "from: " << from << " to: " << to << " players: " << getPlayers() << endl; }
} Block;

int cmp(Block b1, Block b2){
    return b1.from < b2.from;
}

int sum(vector<Block> blocks){
    int sum = 0;
    for(auto it = blocks.begin(); it != blocks.end(); ++it)
        sum += it->getPlayers();
    return sum;
}

bool conflict(Block b1, Block b2){
    return b1.from > b2.to || b1.to < b2.from;
}

void testcase(){
    // n -> defence players, m -> attackers, k -> power of attackers
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> defence(n);
    vector<int> sums(n);
    vector<Block> blocks;
    for(int i = 0; i < n; i++){
        cin >> defence[i];
    }
    sums[0] = defence[0];
    for(int i = 1; i < n; i++){
        sums[i] = sums[i-1] + defence[i];
    }
    int j = 0, i = 0;
    // sliding window O(n)
    while(i < n && j < n){
        int power = sums[i] - sums[j] + defence[j];
        if(power == k){
            Block b;
            b.from = j; b.to = i;
            blocks.push_back(b);
            i++; j++;
        } else if (power > k){
            j++;
        } else {
            i++;
        }
    }
    if(blocks.size() < m){
        cout << "fail\n";
        return;
    } else if (blocks.size() == m){
        cout << sum(blocks) << endl;
        return;
    }

    sort(blocks.begin(), blocks.end(), cmp);

    int max = 0, prevRight, currRight = -1, last_i;
    for (int i = 0; i < blocks.size(); i++){
        if(blocks[i].from > currRight){
            prevRight = currRight;
            currRight = blocks[i].to;
            max += blocks[i].getPlayers();
            last_i = i;
        } else {
            
        }
    }
    if(max == 0)
        cout << "fail\n";
    else
        std::cout << max << endl;
}

int main(){
    int t;
    cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}