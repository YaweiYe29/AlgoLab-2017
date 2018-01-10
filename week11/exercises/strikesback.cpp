// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/MP_Float.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
// choose exact integral type
typedef CGAL::MP_Float ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    int a, s, b, e;
    std::cin >> a >> s >> b >> e;

    Program lp (CGAL::LARGER, true, 0, false, 0);

    std::vector<int> a_x(a), a_y(a);
    for(int i = 0; i < a; i++){
        int d;
        std::cin >> a_x[i] >> a_y[i] >> d;
        lp.set_b(i, d);
    }
    std::vector<int> s_x(s), s_y(s);
    for(int i = 0; i < s; i++){
        std::cin >> s_x[i] >> s_y[i];
    }
    std::vector<K::Point_2> br;
    br.reserve(b);
    for(int i = 0; i < b; i++){
        K::Point_2 pt;
        std::cin >> pt;
        br.push_back(pt);
    }
    Triangulation t;
    t.insert(br.begin(), br.end());

    for(int i = 0; i < s; i++){
        double closest_rider = INT32_MAX;
        if(b != 0){
            K::Point_2 pt(s_x[i], s_y[i]);
            K::Point_2 nearest_rider = t.nearest_vertex(pt)->point();
            closest_rider = CGAL::squared_distance(pt, nearest_rider);
        }
        for(int j = 0; j < a; j++){
            double dist = CGAL::squared_distance(K::Point_2(a_x[j], a_y[j]), K::Point_2(s_x[i], s_y[i]));
            if(dist < closest_rider){
                dist = std::max(1.0, dist);
                double d = 1.0/dist;
                lp.set_a(i, j, d);
            }
        }
        lp.set_c(i, 1);
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());
    //std::cout << "value: " << CGAL::to_double(sol.objective_value()) << " e: " << e << std::endl;
    if(!sol.is_infeasible() && CGAL::to_double(sol.objective_value()) <= e){
        std::cout << "y\n";
    } else {
        std::cout << "n\n";
    }
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}