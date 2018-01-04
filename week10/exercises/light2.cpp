#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef struct Player{
    int r;
    K::Point_2 p;
    int killedBy;
    int i;
} Player;

int cmp(Player p1, Player p2){
    return p1.killedBy > p2.killedBy;
}
/*
Idea worth 100 points is a bit complicated. 
Key is to create delaunay triangulation from lamps and then iterate through players and find closest lamp as in 60p solution.
If distance between is greater than height of lamps + radius of player than player wins for sure. If not than we have to iterate 
through lamps and find first one that "spotted" him. We save it it player's property. At the end if no one wins, we need to find player with
highest spotted lamp and than find all with this number. Sort them and print. If there are winners than just print them.
*/
void testcase(){
    int m, n; 
    int height;
    std::cin >> m >> n;
    std::vector<Player> players(m);
    std::vector<int> winners;
    std::vector<K::Point_2> lamps;
    
    lamps.reserve(n);
    for(int i = 0; i < m; i++){
        Player p;
        std::cin >> p.p >> p.r;
        p.killedBy = -1;
        p.i = i;
        players[i] = p;
    }
    std::cin >> height;
    for(int i = 0; i < n; i++){
        K::Point_2 pt;
        std::cin >> pt;
        lamps.push_back(pt);
    }
    Triangulation t;
    t.insert(lamps.begin(), lamps.end());
    bool won = false;
    for(int i = 0; i < m; i++){
        K::Point_2 pt = t.nearest_vertex(players[i].p)->point();
        double d = CGAL::squared_distance(pt, players[i].p);
        double d2 = players[i].r + height;
        d2 = d2 * d2;
        if(d >= d2){
            winners.push_back(i);
            won = true;
        } else {
            if(!won){
                for(int j = 0; j < n; j++){
                    d = CGAL::squared_distance(players[i].p, lamps[j]);
                    if(d < d2){
                        players[i].killedBy = j;
                        break;
                    }
                }
            }
        }
    }

    if(winners.empty()){
        std::sort(players.begin(), players.end(), cmp);
        int highest = players.begin()->killedBy;
        int i = 0;
        while(players[i].killedBy == highest)
            winners.push_back(players[i++].i);
        std::sort(winners.begin(), winners.end());
    }

    for (auto it = winners.begin(); it != winners.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}