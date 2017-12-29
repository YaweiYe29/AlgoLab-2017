#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;

std::map<K::Point_2, std::vector<K::Point_2> > reachables;

int recur(K::Point_2 vertex, std::set<K::Point_2> visited){
    int max = 1;
    visited.insert(vertex);
    for (int i = 0; i < reachables[vertex].size(); i++){
        if(visited.find(reachables[vertex][i]) == visited.end()){
            //std::cout << vertex << " " << reachables[vertex][i] << std::endl;
            int res = recur(reachables[vertex][i], visited) + 1;
            if(res > max)
                max = res;
        }
    }
    visited.erase(vertex);
    return max;
}

void testcase(){
    int n, r;
    std::cin >> n >> r;
    r = r * r;
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    reachables.clear();

    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        if(i >= n / 2)
            pts.push_back(p);
    }

    for (int i = 0; i < pts.size(); i++){
        std::cout << pts[i] << ": ";
        for (int j = i + 1; j < pts.size(); j++){
            if (CGAL::squared_distance(pts[i], pts[j]) <= r){
                std::cout << "(" << pts[j] << ") ";
                reachables[pts[i]].push_back(pts[j]);
                reachables[pts[j]].push_back(pts[i]);
            }
        }
        std::cout << std::endl;
    }

    int max = 0;
    //std::cout << pts.size() << std::endl;
    for(int i = 0; i < pts.size(); i++) {
        int res = recur(pts[i], std::set<K::Point_2>());
        if(res > max)
            max = res;
    }
    std::cout << max << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n--) {
        testcase();
    };

    return 0;
}