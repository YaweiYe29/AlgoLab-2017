#include<iostream>
#include<climits>
#include<algorithm>
#include<cassert>

using namespace std;

// returns how many bulbs are wrong in interval <from, from + k)
int check_bulbs(bool* bulbs, bool* pattern, int k, int from){
    int wrong = 0;
    for(int i = from; i < from + k; i++){
        if(bulbs[i] != pattern[i - from])
            wrong++;
    }
    return wrong;
}

// corrects number of wrong according to no of global switches
int correct_wrong(int switches, int switched, int k){
    if(switched % 2 == 1)
        return k - switches;
    return switches;
}

void solve(){
    int n, k, x;
    cin >> n >> k >> x;
    bool* pattern = new bool[k];
    bool* bulbs = new bool[n];

    for(int i = 1; i <= k; i++){
        pattern[k - i] = (1 << (i - 1)) & x;        // read pattern
    }

    for(int i = 0; i < n; i++)
        cin >> bulbs[i];                            // read bulbs

    int global_single_switches = 0, global_switches_needed = 0, next_wrong, wrong;
    int groups = n/k;

    for(int i = groups - 1; i > 0; i--){
        wrong = correct_wrong(check_bulbs(bulbs, pattern, k, i * k), global_switches_needed, k);                // read how many are wrong in current group
        next_wrong = correct_wrong(check_bulbs(bulbs, pattern, k, (i - 1) * k), global_switches_needed, k);     // read how many are wrong in next group

        if(wrong == 0)                                                                                          // if no are wrong than continue
            continue;

        if((wrong > (k/2) && next_wrong > (k/2)) || (k > 1 && wrong > ((k + 1)/2))){                            // if I can by global switch switch more bulbs into correct position than do it
            global_switches_needed++;
            if(k > 1)
                global_single_switches += k - wrong;                                                            // switch the ones than were wrong after global switch in the group
        } else {
            global_single_switches += wrong;                                                                    // otherwise switch individually
        }
    }

    int lastGroup = correct_wrong(check_bulbs(bulbs, pattern, k, 0), global_switches_needed, k);                // deal with the last group(first in the order)
    if(k > 1)
        lastGroup = min(lastGroup, k - lastGroup + 1);

    cout << global_single_switches + global_switches_needed + lastGroup << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        solve();
    }
    return 0;
}