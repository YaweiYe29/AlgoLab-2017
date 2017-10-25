#include<iostream>
#include<climits>

using namespace std;

int check_bulbs(bool* bulbs, bool* pattern, int k, int from){
    int wrong = 0;
    for(int i = from; i < from + k; i++){
        if(bulbs[i] != pattern[i - from])
            wrong++;
    }
    return wrong;
}

void solve(){
    int n, k, x;
    cin >> n >> k >> x;
    bool* pattern = new bool[k];
    bool* bulbs = new bool[n];

    for(int i = 1; i <= k; i++){
        pattern[k - i] = (1 << (i - 1)) & x;
    }

    for(int i = 0; i < n; i++)
        cin >> bulbs[i];

    int global_single_switches = 0, global_all_switches = 0, local_single_switches, local_all_switches;
    int groups = n/k;
    int switches_needed[groups];
    int switch_sum = 0;
    int n_of_switches = 0;

    for(int i = 0; i < groups; i++){
        switches_needed[i] = check_bulbs(bulbs, pattern, k, i * k);
        switch_sum += switches_needed[i];
    }
    switch_sum -= switches_needed[groups - 1];

    for(int i = groups - 1; i >= 0; i--){
        if(switch_sum == 0)
            break;

        if(switch_sum > ((i + 1) * k / 2)){
            switch_sum = 0;
            for(int j = 0; j < groups; j++){
                switches_needed[j] = k - switches_needed[j];
                switch_sum += switches_needed[j];
            }
            n_of_switches += switches_needed[i] + 1;
        }
        else {
            n_of_switches += switches_needed[i];
            switch_sum -= switches_needed[i];
        }
    }

    cout << n_of_switches << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        solve();
    }
    return 0;
}



/*1 1 1 0 0 1 1 1
1 1 1 0 0 1 1 1

sum = 6 i = 7
switches = 0
if(6 > 4){
    global_switch;
    count again;
    update sum;
    switches += 1 + 0
}

0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0

sum = 2 i = 6
switches = 1
if(2 > 3){
    nothing
} else {
    switches += local_single_switches // 0
}

0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0

sum = 2 i = 5
switches = 1
if(2 > 3){
    nothing
} else {
    switches += local_single_switches // 0
}

0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0

sum = 2 i = 4
switches = 1
if(2 > 2){
    nothing
} else {
    switches += local_single_switches // 1
}

0 0 0 1 1 0 0 0
0 0 0 1 1 0 0 0

sum = 1 i = 3
switches = 2

if(1 > 2){
    nothing
} else {
    switches += local_single_switches // 1
}

sum = 0 i = 2
switches = 3

sum == 0 we are done
*/