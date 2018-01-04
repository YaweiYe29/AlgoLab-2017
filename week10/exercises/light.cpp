/* This is solution for 60 points only. Checl light2.cpp for full points.*/

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
} Player;

void testcase(){
    int m, n; 
    int height;
    std::cin >> m >> n;
    std::vector<Player> players(m);
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for(int i = 0; i < m; i++){
        Player p;
        std::cin >> p.p >> p.r;
        players[i] = p;
    }
    std::cin >> height;
    for(int i = 0; i < n; i++){
        K::Point_2 pt;
        std::cin >> pt;
        pts.push_back(pt);
    }
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    for(int i = 0; i < m; i++){
        K::Point_2 pt = t.nearest_vertex(players[i].p)->point();
        double d = CGAL::squared_distance(pt, players[i].p);
        double d2 = players[i].r + height;
        d2 = d2 * d2;
        if(d >= d2)
            std::cout << i << " ";
    }
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