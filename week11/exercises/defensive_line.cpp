/*primitive solution worth 30 points*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct Block{
    int from, to, power;
    int getPlayers(){ return to - from + 1; }
    void print (){ cout << "from: " << from << " to: " << to << " power: " << power << " players: " << getPlayers() << endl; }
} Block;

int cmp(Block b1, Block b2){
    return b1.getPlayers() > b2.getPlayers();
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
            b.power = power;
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
        int sum = 0;
        for (auto it = blocks.begin(); it != blocks.end(); it++){
            sum += it->getPlayers();
        }
        cout << sum << endl;
        return;
    }

    if(m == 1){
        std::cout << blocks.begin()->getPlayers() << endl;
        return;
    }

    int max = 0;
    // try every with every block and find nonconflicting maximum | just for m == 2
    for (int i = 0; i < blocks.size(); i++){
        for (int j = i + 1; j < blocks.size(); j++){
            if(conflict(blocks[i], blocks[j])){
                max = std::max(max, blocks[i].getPlayers() + blocks[j].getPlayers());
            }
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