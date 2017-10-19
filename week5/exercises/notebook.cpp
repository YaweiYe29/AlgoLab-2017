//STL includes
#include <iostream>
#include <unordered_map>
#include <string>
 
using namespace std;

void notebook(){
    int k, n, q, tmp;
    cin >> k >> n >> q;
    // this does the trick! Complexity O(1) rather than O(log(n)) in standart map
    unordered_map<string, int> finalMap;

    //skip bounds, we dont need them!
    for(int i = 0; i < k; i++){
        cin >> tmp >> tmp;
    }
    // add numbers to string and then to the map
    for(int i = 0; i < n; i++){
        string s = "";
        for(int j = 0; j < k; j++){
            cin >> tmp;
            s += std::to_string(tmp);
        }
        cin >> tmp;
        finalMap[s] = tmp;
    }

    //add every query to string and get the value from the map
    for(int i = 0; i < q; i++){
        string s = "";
        for(int j = 0; j < k; j++){
            cin >> tmp;
            s += std::to_string(tmp);
        }
        if(finalMap.find(s) == finalMap.end())
            cout << "-1" << endl;
        else
            cout << finalMap[s] << endl;
    }
}

int main(){
    int n;
    cin >> n;
    while(n--){
        notebook();
    }
    return 0;
}