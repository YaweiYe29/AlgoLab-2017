#include<iostream>
#include<algorithm>
#include<set>
#include<map>

using namespace std;

struct Word{
    int group;
    int index;

public: Word(int group, int index){
    this->group = group;
    this->index = index;
    }

public: Word(){
    group = -1;
    index = -1;
    }
};

int Cmp(Word word1, Word word2){
    return word1.index < word2.index;
}

void Snippet(){
    int n, sum = 0, counter = 0, min = 1234567890;
    std::set<int> included;
    std::set<int> uniqueIn;
    std::map<int, int> groupsInCount;

    cin >> n;
    int counts[n];
    for(int i = 0; i < n; i++){
        cin >> counts[i];
        groupsInCount[i] = 0;
        sum += counts[i];
    }

    Word words[sum];
    for(int i = 0; i < n; i++){
        for (int j = 0; j < counts[i]; j++){
            int index;
            cin >> index;

            words[counter++] = Word(i, index);
        }
    }

    std::sort(&words[0], words+sum, Cmp);

    bool first = true;
    for(int i = 0; i < sum; i++){
        included.insert(words[i].index);
        uniqueIn.insert(words[i].group);
        groupsInCount[words[i].group]++;

        if(uniqueIn.size() == n && first){
            min = *included.rbegin() - *included.begin() + 1;
            first = false;
        } else {
            int size = included.size();
            int excludedIndex = i - size + 1;
            int excludedGroup = words[excludedIndex].group;
            if(groupsInCount[excludedGroup] > 1){
                while(words[excludedIndex].group == excludedGroup && groupsInCount[excludedGroup] > 1){
                    included.erase(words[excludedIndex].index);
                    groupsInCount[excludedGroup]--;
                    excludedIndex++;
                    excludedGroup = words[excludedIndex].group;
                }
            }

            if(uniqueIn.size() == n && !first){
                int localMin = *included.rbegin() - *included.begin() + 1;
                if(localMin < min)
                    min = localMin;
                }
        }
    }
    cout << min << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    cin >> n;

    for(int i = 0; i < n; i++){
        Snippet();
    }
    return 0;
}