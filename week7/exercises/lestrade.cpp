#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef struct GangMember{
    int x, y, u, v, w;
} GangMember;

int distance(GangMember g, int x, int y){
    return (g.x - x) * (g.x - x) + (g.y - y) * (g.y - y);
}

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
    std::map<K::Point_2, std::vector<int>> spiedGangs;
    std::vector<K::Point_2> pts;
    Triangulation t;

    // load gang members
    for(int i = 0; i < g; i++){
        GangMember gg;
        std::cin >> gg.x >> gg.y >> gg.u >> gg.v >> gg.w;
        K::Point_2 pt(gg.x, gg.y);
        gang[pt] = gg;
        pts.push_back(pt);
    }

    t.insert(pts.begin(), pts.end());

    // our variables will be hours of agents so they have to be between 0 and 24
    Program * lp = new Program(CGAL::LARGER, true, 0, true, 24);

    lp->set_b(0, u);
    lp->set_b(1, v);
    lp->set_b(2, w);

    for(int i = 0; i < a; i++){
        int x, y, z; // z -> hour wage, x, y position
        std::cin >> x >> y >> z;

        // set objective function -> agent * salary
        lp->set_c(i, z);
        // find closest gang member
        int min = INT32_MAX;
        GangMember closest = gang[t.nearest_vertex(K::Point_2(x, y))->point()];
    
        //std::cout << "closest: " << closest.x << " " << closest.y << std::endl;
        lp->set_a(i, 0, closest.u);
        lp->set_a(i, 1, closest.v);
        lp->set_a(i, 2, closest.w);
        spiedGangs[K::Point_2(closest.x, closest.y)].push_back(i);
    }
    
    int constrain = 3;
    for(auto it = spiedGangs.begin(); it != spiedGangs.end(); ++it){
        if(it->second.size() > 1){
            //std::cout << "gang" << it->first << " spied by " << it->second.size() << std::endl;
            for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
                lp->set_a(*it2, constrain, 1);
            }
            lp->set_r(constrain, CGAL::SMALLER);
            lp->set_b(constrain, 24);
            constrain++;
        }
    }
    std::cout << "constrains: " << constrain << std::endl;
    // solve it!
    Solution s = CGAL::solve_linear_program(*lp, ET());
    //std::cout << s.is_infeasible() << " " << CGAL::to_double(s.objective_value()) << " < " << z << std::endl;
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
    int n;
    std::cin >> n;
    while(n--){
        testcase();
    }
    return 0;
}