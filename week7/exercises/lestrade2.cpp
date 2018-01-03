#include <map>
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>

#include <CGAL/Gmpq.h>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef struct GangMember{
    int x, y, u, v, w;
} GangMember;

using namespace std::chrono;
void testcase(){
    // z -> money requested by holmes
    // u -> 'where' required
    // v -> 'who' required
    // w -> 'how' required
    int z, u, v, w;
    std::cin >> z >> u >> v >> w;

    // a -> n. of agents
    // g -> n. of gang members
    int a, g;
    std::cin >> a >> g;

    // gang members
    std::map<K::Point_2, GangMember> gang;
    std::map<K::Point_2, int> spiedGangs;
    std::vector<K::Point_2> pts;
    pts.reserve(g);
    Triangulation t;
    int tmpU = 0, tmpV = 0, tmpW = 0;
    // load gang members
    for(int i = 0; i < g; i++){
        GangMember gg;
        std::cin >> gg.x >> gg.y >> gg.u >> gg.v >> gg.w;
        K::Point_2 pt(gg.x, gg.y);
        gang[pt] = gg;
        pts.push_back(pt);
    }

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    t.insert(pts.begin(), pts.end());
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
    // our variables will be hours of agents so they have to be between 0 and 24
    Program * lp = new Program(CGAL::LARGER, true, 0, true, 24);

    lp->set_b(0, u);
    lp->set_b(1, v);
    lp->set_b(2, w);
t1 = high_resolution_clock::now();
    for(int i = 0; i < a; i++){
        int x, y, z; // z -> hour wage, x, y position
        std::cin >> x >> y >> z;
        // find closest gang member
        K::Point_2 p = t.nearest_vertex(K::Point_2(x, y))->point();
        if(spiedGangs[p] == 0)
            spiedGangs[p] = z;
        else 
            spiedGangs[p] = std::min(spiedGangs[p], z);
    }
    t2 = high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
    
t1 = high_resolution_clock::now();
    int agent = 0;
    for(auto it = spiedGangs.begin(); it != spiedGangs.end(); ++it){
        int z = it->second;
        GangMember gg = gang[it->first];
        lp->set_a(agent, 0, gg.u);
        lp->set_a(agent, 1, gg.v);
        lp->set_a(agent, 2, gg.w);
        tmpU += gg.u * 24;
        tmpV += gg.v * 24;
        tmpW += gg.w * 24;
        lp->set_c(agent, z);
        agent++;
    }
t2 = high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
    if(tmpU < u || tmpV < v || tmpW < w){
        std::cout << "H\n";
        return;
    }

t1 = high_resolution_clock::now();
    //std::cout << "agents: " << agent << "(real: " << a << ")" << std::endl;
    // solve it!
    Solution s = CGAL::solve_linear_program(*lp, ET());
t2 = high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
    if(s.is_infeasible()){
        std::cout << "H\n";
    } else {
        if(CGAL::to_double(s.objective_value()) <= z)
            std::cout << "L\n";
        else 
            std::cout << "H\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
    return 0;
}