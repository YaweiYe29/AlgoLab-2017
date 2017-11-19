#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;

void testcase(int n)
{
  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    K::Point_2 p;
    std::cin >> p;
    pts.push_back(p);
  }

  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  double globalMin = INT_MAX;
  // iterate all edges in delaunay diagram
  // from theory we know that diagram contains nearest neighbour graph
  // so one of the edges connect ape with closest one
  // find minimum of those and print it!
  for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    double distance  = CGAL::squared_distance(v1->point(), v2->point());
    if(distance < globalMin){
        globalMin = distance;
    }
  }

  globalMin = ceil(sqrt(globalMin) * 50);

  std::cout << globalMin << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setprecision(30);
    int n;
    while(true) {
        std::cin >> n;
        if(n == 0) break;
        testcase(n);
    };

    return 0;
}