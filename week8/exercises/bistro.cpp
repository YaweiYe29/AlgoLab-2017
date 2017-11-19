#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

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

  int m;
  std::cin >> m;
  // read position of potential bistro and calculate distance to closest point
  for(int i = 0; i < m; i++){
    K::Point_2 p;
    std::cin >> p;
    K::Point_2 nearest = t.nearest_vertex(p)->point();
    std::cout << CGAL::squared_distance(p, nearest) << std::endl;
  }
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