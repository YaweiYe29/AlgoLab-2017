#include <iostream>
#include <vector>
#include <set>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;

// whether we can fit between these two points
bool greater(Vertex_handle pt1, Vertex_handle pt2, double d){
    int can = CGAL::squared_distance(pt1->point(), pt2->point()) >= (d * 4) ? 1 : 0;
    return CGAL::squared_distance(pt1->point(), pt2->point()) >= (d * 4);
}
// whether h is in visited
bool contains(std::set<Face_handle> &visited, Face_handle h){
    return visited.find(h) != visited.end();
}

// try to escape DFS like. If we reach infinite face we are done
bool dfs(Face_handle start, Triangulation &t, double d){
    std::set<Face_handle> visited;
    std::queue<Face_handle> queue;
    queue.push(start);
    visited.insert(start);
    while(!queue.empty()){
        Face_handle curr = queue.front();
        queue.pop();
        if(t.is_infinite(curr))
            return true;

        if(greater(curr->vertex(0), curr->vertex(1), d) && !contains(visited, curr->neighbor(2))){
            queue.push(curr->neighbor(2));
            visited.insert(curr->neighbor(2));
        }
        if(greater(curr->vertex(2), curr->vertex(1), d) && !contains(visited, curr->neighbor(0))){
            queue.push(curr->neighbor(0));
            visited.insert(curr->neighbor(0));
        }
        if(greater(curr->vertex(0), curr->vertex(2), d) && !contains(visited, curr->neighbor(1))){
            queue.push(curr->neighbor(1));
            visited.insert(curr->neighbor(1));
        }
    }
    return false;
}

void testcase(int n)
{
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        pts.push_back(p);
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    int m;
    std::cin >> m;

    for(int i = 0; i < m; i++){
        K::Point_2 pt;
        double d;
        std::cin >> pt >> d;
        Face_handle st = t.locate(pt);
        K::Point_2 pt2 = t.nearest_vertex(pt)->point();
        //if we get infected at the beginning output n. Otherwise try to escape...
        if(CGAL::squared_distance(pt, pt2) < d)
            std::cout << "n";
        else if(dfs(st, t, d))
            std::cout << "y";
        else
            std::cout << "n";
    }
    std::cout << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}