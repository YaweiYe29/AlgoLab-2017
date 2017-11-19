/*

Reworked solution from Nikola Dodik.
The only difference is that we loop through vertexes and their edges. It gives 100/100.

*/

#include <iostream>
#include <vector>
#include <fstream>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Triangulation = CGAL::Delaunay_triangulation_2<K>;

int ceil_to_double(const K::FT& x) {
    double a = ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

void run_test(int n) {
    double min_x, max_x, min_y, max_y;
    std::cin >> min_x >> min_y >> max_x >> max_y;
    std::vector<K::Point_2> bacteria;
    bacteria.reserve(n);
    std::copy_n(std::istream_iterator<K::Point_2>(std::cin), n, std::back_inserter(bacteria));

    Triangulation dt(bacteria.begin(), bacteria.end());
    std::vector<K::FT> min_dist_sq;
    min_dist_sq.reserve(n);
    for(auto vert_h = dt.finite_vertices_begin(); vert_h != dt.finite_vertices_end(); ++vert_h) {
        auto pt = vert_h->point();
        auto min_x_d = std::min(pt.x() - min_x, max_x - pt.x());
        auto min_y_d = std::min(pt.y() - min_y, max_y - pt.y());
        auto min_edge_d = std::min(min_x_d, min_y_d);
        min_edge_d *= min_edge_d;
        auto circulator = dt.incident_edges(vert_h);
        CGAL::Container_from_circulator<decltype(circulator)> container(circulator);
        auto min_to_neighbor_it = std::min_element(container.begin(), container.end(),
            [&dt](const Triangulation::Edge& edge1, const Triangulation::Edge& edge2) {
                if(!dt.is_infinite(edge1) && !dt.is_infinite(edge2))
                    return dt.segment(edge1).squared_length() < dt.segment(edge2).squared_length();
                if(dt.is_infinite(edge1) && dt.is_infinite(edge2))
                    return false;
                if(dt.is_infinite(edge1))
                    return false;
                return true;
            }
        );
        K::FT min_to_neighbor;
        if(min_to_neighbor_it != container.end()) {
            min_to_neighbor = dt.segment(*min_to_neighbor_it).squared_length() / 4;
            min_dist_sq.push_back(std::min(min_edge_d, min_to_neighbor));
        } else {
            min_dist_sq.push_back(min_edge_d);
        }
    }
    std::sort(min_dist_sq.begin(), min_dist_sq.end(), [](const K::FT& d1, const K::FT& d2) { return d1 < d2; });
    int middle = min_dist_sq.size() / 2;
    std::cout << ceil_to_double(CGAL::sqrt(CGAL::sqrt(min_dist_sq.front()) - 0.5)) << ' '
        << ceil_to_double(CGAL::sqrt(CGAL::sqrt(min_dist_sq[middle]) - 0.5)) << ' '
        << ceil_to_double(CGAL::sqrt(CGAL::sqrt(min_dist_sq.back()) - 0.5)) << std::endl;
}

int main() {
    int n = 0;
    std::cin >> n;
    while(n != 0) {
        run_test(n);
        std::cin >> n;
    }
    return 0;
}
