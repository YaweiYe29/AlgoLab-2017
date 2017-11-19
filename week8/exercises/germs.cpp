/*

This solution gives only 30/100. See germs for full points.

*/

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;

int ceil_to_int(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

int get_germ_time(K::FT& x){
    if(x <= 0.5)
        return 0;
    return ceil_to_int(CGAL::sqrt(CGAL::sqrt(x) - 0.5));
}

void testcase(int n)
{
  assert(n > 2);
  //l ≤ x ≤ r and b ≤ y ≤ t.
  int l, b, r, t;
  std::cin >> l >> b >> r >> t;

  // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    K::Point_2 p;
    std::cin >> p;
    pts.push_back(p);
  }

  // construct triangulation
  Triangulation triangulation;
  triangulation.insert(pts.begin(), pts.end());

  std::map<K::Point_2, K::FT> germs;
  std::vector<K::FT> deaths;

  // iterate all edges in delaunay diagram
  // from theory we know that diagram contains nearest neighbour graph
  // so germ will incident with closest edge or with border
  // find minimum of those!
  for (Edge_iterator e = triangulation.finite_edges_begin(); e != triangulation.finite_edges_end(); ++e){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

    K::FT distance = triangulation.segment(e).squared_length() / 4;
    K::FT v1dist = std::min(std::min(abs(v1->point().x() - l), abs(v1->point().x() - r)),
                          std::min(abs(v1->point().y() - b), abs(v1->point().y() - t)));
    K::FT v2dist = std::min(std::min(abs(v2->point().x() - l), abs(v2->point().x() - r)),
                          std::min(abs(v2->point().y() - b), abs(v2->point().y() - t)));

    v1dist = std::min(v1dist * v1dist, distance);
    v2dist = std::min(v2dist * v2dist, distance);

    if(v1dist < germs[v1->point()] || germs[v1->point()] == 0){
        germs[v1->point()] = v1dist;
    }
    if(v2dist < germs[v2->point()] || germs[v2->point()] == 0){
        germs[v2->point()] = v2dist;
    }                   
  }

  assert(germs.size() == n);
  for(auto it = germs.begin(); it != germs.end(); ++it){
    deaths.push_back(it->second);
  }

  std::sort(deaths.begin(), deaths.end(), [](const K::FT& d1, const K::FT& d2) { return d1 < d2; });
  int middle = (int)(deaths.size() / 2);

  std::cout << get_germ_time(deaths.front()) << " " << get_germ_time(deaths[middle]) << " " <<  get_germ_time(deaths.back()) << std::endl;
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