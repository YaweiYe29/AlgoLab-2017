#include<iostream>
#include<climits>
#include<unordered_map>


using namespace std;

bool check_bulbs(bool* bulbs, bool* pattern, int k, int n){
    for(int i = 0; i < k; i++){
        for(int j = 0; j < n/k; j++){
            if(pattern[i] != bulbs[j*k + i])
                return false;
        }
    }

    for(int i = 0; i < n; i++){
        //cout << bulbs[i] << " ";
    }
    //cout << endl;
    return true;
}

void sw(bool * bulbs, int to, bool whole){
    if(whole){
        for(int i = 0; i <= to; i++)
            bulbs[i] = !bulbs[i];
    } else 
        bulbs[to] = !bulbs[to];
}
int global_min;
unordered_map<string, int> memo;
void light(bool* bulbs, bool* pattern, int pos, int whole, int k, int n, int n_of_switched, vector<pair<int,bool> > prevSteps){
    if(pos >= n)
        return;

    sw(bulbs, pos, whole);
    if(check_bulbs(bulbs, pattern, k, n))
        if(n_of_switched < global_min){
            global_min = n_of_switched;
            cout << "New min:!!!!! " << global_min << " " << pos << " " << whole << endl;
            for(int i = 0; i < n; i++){
                cout << bulbs[i] << " ";
            }
            cout << endl;
            sw(bulbs, pos, whole);
            for(int i = 0; i < n; i++){
                cout << bulbs[i] << " ";
            }
            sw(bulbs, pos, whole);
            cout << endl;
        }

    }
    //cout << "L\n";

    //cout << "light" << endl;
    for(int i = pos + 1; i < n; i++){
        light(bulbs, pattern, pos+1, false, k, n, n_of_switched + 1);
        light(bulbs, pattern, pos+1, true, k, n, n_of_switched + 1);
    }

    sw(bulbs, pos, whole);
}

void solve(){
    int n, k, x;
    cin >> n >> k >> x;
    bool* pattern = new bool[n/k];
    bool* bulbs = new bool[n];
    global_min = INT_MAX;

    for(int i = 1; i <= k; i++){
        pattern[k - i] = (1 << (i - 1)) & x;
    }
    //cout << "Pattern created\n";
    int tmp;
    for(int i = 0; i < n; i++)
        cin >> bulbs[i];

     for(int i = 0; i < n; i++){
        light(bulbs, pattern, i, false, k, n, 1);
        light(bulbs, pattern, i, true, k, n, 1);
    }
    cout << global_min << endl;
}

int main(){
    int n;
    cin >> n;
    while(n--){
        solve();
    }
    return 0;
}