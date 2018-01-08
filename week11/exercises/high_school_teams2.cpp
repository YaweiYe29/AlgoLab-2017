#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <unordered_map>

using namespace std;

//70 points
void testcase(){
    long subset_skills[22];                             // max n is 22 -> fixed size faster but makes no difference
    int n, k;
    cin >> n >> k;
    vector<long> skills(n);                             // load skills
    for(int i = 0; i < n; i++){
        cin >> skills[i];
    }
    int limit = 1 << n;                                 // we are going to check all combinations -> 2^n 
    int counter = 0;
    for(int i = 0; i < limit; i++){
        bitset<22> bs(i);
        if(bs.count() < n - k)                          // check how many bits are set i.e. size of players that will be part either of team A or B
            continue;
        long s = 0; // sum
        int c = 0; // counter
        for(int j = 0; j < n; j++){
            if(i & 1 << j){                             // check is j-th player is selected
                s += skills[j];                         // if yes add his skill to sum of skills
                subset_skills[c++] = skills[j];         // and to subset of skills as well
            }
        }

        assert(c == bs.count());                        // independent check of the c
        if(s % 2 != 0)                                  // if sum is odd, there is no way of spliting set to two
            continue;

        vector<long> first_set, second_set;
        for(int j = 0; j < c; j++){                     // divide set to two
            if(j < c/2)
                first_set.push_back(subset_skills[j]);
            else
                second_set.push_back(subset_skills[j]);
        }

        int first_size = first_set.size(), second_size = second_set.size();
        int first_limit = 1 << first_size;
        int second_limit = 1 << second_size;
        long target_sum = s/2;
        unordered_map<long, long> sums;
        for(int ii = 0; ii < first_limit; ii++){        // generate all subsums of first subset. Attention there can be more than one same sum so use map with counts
            long sum1 = 0;
            for(int jj = 0; jj < first_size; jj++){
                if(ii & 1 << jj){
                    sum1 += first_set[jj];
                }
            }
            sums[sum1]++;
        }
        for(int ii = 0; ii < second_limit; ii++){       // generate all subsums of second subset and check if there exist sum from other subset so they will
            long sum2 = 0;                              // together give targeted sum(half of the sum)
            for(int jj = 0; jj < second_size; jj++){
                if(ii & 1 << jj){
                    sum2 += second_set[jj];
                }
            }
            counter += sums[target_sum - sum2];
        }
    }
    cout << counter << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    while(t--) {
        testcase();
    }
    return 0;
}