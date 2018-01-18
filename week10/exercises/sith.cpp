#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge Edge;
typedef Triangulation::Vertex_handle Vertex_handle;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property>	Graph;

// finds max component in nearest neighbor graph 
int max_component(std::map<K::Point_2,int> ptsi, std::vector<K::Point_2> pts, int f, long long rr, int n){
    Triangulation t;
    t.insert(pts.begin() + f, pts.end());

    Graph G(n - f);
    for (auto edgeit = t.finite_edges_begin(); edgeit != t.finite_edges_end(); ++edgeit) {
        Edge edge = *edgeit;
        K::Point_2 v1 = t.segment(edge).source();
        K::Point_2 v2 = t.segment(edge).target(); 
        if(CGAL::squared_distance(v1, v2) <= rr)
            boost::add_edge(ptsi[v1] - f, ptsi[v2] - f, G);
    }
    std::vector<int> componentmap(n - f);
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G))); 
	std::vector<int> componentsize(ncc);
	for (int i = 0; i < n-f; ++i)
		++componentsize[componentmap[i]];
	return *std::max_element(componentsize.begin(), componentsize.end());
}

void testcase(){
    int n, r;
    std::cin >> n >> r;
    long rr = r;
    rr *= rr;

    // read points
    std::vector<K::Point_2> pts;
    std::map<K::Point_2, int> ptsi;
    pts.reserve(n);
    for (int i = 0; i < n; i++) {
        K::Point_2 p;
        std::cin >> p;
        pts.push_back(p);
        ptsi[p] = i;
    }

    int low = 0;
    int high = n / 2 + 1;
    // binary search over (0, n/2 + 1) to find appropriate k
    while(high - low > 1){
        int k = low + (high - low) / 2;
        int res = max_component(ptsi, pts, k, rr, n);
        if(k > res)
            high = k;
        else
            low = k;
    }

    std::cout << low << std::endl;
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