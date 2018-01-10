#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <unordered_map>

int min_price, max_beverages;

int getUsed(std::unordered_map<int, int> &counts, std::unordered_map<int,int> &used){
    int sum = 0;
    for(auto it = counts.begin(); it != counts.end(); ++it){
        sum += std::min(it->second, used[it->first]);
    }
    return sum;
}

void dp(std::unordered_map<int, int> &counts, std::unordered_map<int,int> &used, std::unordered_map<int,int> &prices, 
        int needed, int price, int best_volume){
    if(needed <= 0){
        int beverages = getUsed(counts, used);
        if(price < min_price){
            min_price = price;
            max_beverages = beverages;
        } else if (price == min_price){
            max_beverages = std::max(max_beverages, beverages);
        }
        return;
    }

    for(auto it = counts.begin(); it != counts.end(); ++it){
        int needed2 = needed - it->first;
        int price2 = price + prices[it->first];
        used[it->first]++;
        dp(counts, used, prices, needed2, price2, best_volume);
        used[it->first]--;
    }
}

void testcase() {
    min_price = INT32_MAX;
    max_beverages = 0;
    int n, k;
    std::cin >> n >> k;
    std::unordered_map<int,int> counts, used, prices;
    std::vector<std::pair<int,int>> beverages(n);
    double minRatio = 10000.00001;
    int minVolume, minPrice;
    for(int i = 0; i < n; i++){
        std::cin >> beverages[i].first >> beverages[i].second;
        double ratio = (double)((double)beverages[i].first / (double)beverages[i].second);
        if(ratio < minRatio){
            minRatio = ratio;
            minVolume = beverages[i].second;
            minPrice = beverages[i].first;
        }
    }

    for(int i = 0; i < n; i++){
        int cost = beverages[i].first, volume = beverages[i].second;
        double ratio = (double)((double)cost/(double)volume);

        if((volume < minVolume && cost > minPrice) || (minVolume > volume && cost > minPrice) || (minVolume * 2 > volume && cost > 2 * minPrice))
            continue;
        if(prices[volume] == 0 || prices[volume] > cost){
            prices[volume] = cost;
            counts[volume] = 1;
        } else if(prices[volume] == cost){
            counts[volume]++;
        }
    }

    int needed = k % minVolume;

    std::cout << " k: " << k << std::endl;

    for(auto it = counts.begin(); it != counts.end(); ++it){
        std::cout << it->first << " -> " << it->second << " (" << prices[it->first] << ")"; 
        if(it->first == minVolume)
            std::cout << " BEST VOLUME ";
        std::cout << std::endl;
    }

    //dp(counts, used, prices, k, needed, minVolume);
    std::cout << min_price << " " << max_beverages << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}