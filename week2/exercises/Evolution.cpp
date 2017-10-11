#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<utility>
using namespace std;

struct Specie{
    string name;
    int age;
    vector<int> ancestors;
    vector<int> children;
};

Specie * species;
map<string, int> nameToIndex;

void DFS(int index, vector<Specie *>&prev){
    Specie cur = species[index];
    unsigned i = 1;
    unsigned n = prev.size();
    while(n - i < prev.size()){
        species[index].ancestors.push_back(nameToIndex[prev[n - i]->name]);
        i *= 2;
    }

    if (n > 0 && prev[0]->name != species[species[index].ancestors.back()].name) {
        species[index].ancestors.push_back(nameToIndex[prev[0]->name]);
    }

    prev.push_back(&species[index]);
    for(int j = 0; j < species[index].children.size(); j++){
        DFS(species[index].children[j], prev);
    }
    prev.pop_back();
}

string binSearch(int curr, int age) {
    vector<int> ancestors = species[curr].ancestors;
    if (ancestors.size() == 0 ||  species[ancestors[0]].age > age) {
      return species[curr].name;
    }
    int last = ancestors[0];
    for (unsigned i = 1 ; i < ancestors.size() ; ++i) {
      if (species[ancestors[i]].age > age) {
        break;
      }
      last = ancestors[i];
    }
    return binSearch(last, age);
  }

void FindAncestor(){
    int n, q, max = 0;
    cin >> n >> q;
    nameToIndex.clear();

    species = new Specie[n];
    int root;
    vector<Specie *> prev;

    for(int i = 0; i < n; i++){
        cin >> species[i].name >> species[i].age;
        if(species[i].age > max){
            max = species[i].age;
            root = i;
        }
        nameToIndex[species[i].name] = i;
    }

    string parent, child;
    for(int i = 0; i < n - 1; i++){
        cin >> child >> parent;
        species[nameToIndex[parent]].children.push_back(nameToIndex[child]);
    }

    //cout << "DFS: " << endl;
    DFS(root, prev);

    /*for(int i = 0; i < n; i++){
        cout << endl;
        cout << "[" << species[i].name << ";" << species[i].age << "]: (Ancestors): ";
        for(int j = 0; j < species[i].ancestors.size(); j++){
            cout << species[species[i].ancestors[j]].name << " ";
        }
        cout << "(Children): ";
        for(int j = 0; j < species[i].children.size(); j++){
            cout << species[species[i].children[j]].name << " ";
        }
    }*/

    for(int i = 0; i < q; i++){
        string query;
        int maxAge;
        cin >> query >> maxAge;

        cout << binSearch(nameToIndex[query], maxAge) << " ";
    }
    cout << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        FindAncestor();
    }
    return 0;
}