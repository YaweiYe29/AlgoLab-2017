#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<double> Solution;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef std::pair<K::Point_2, int> Sensor;
typedef std::pair<K::Point_2, double> MPE;
/* Problem analysis:
 * We need to find optimal k for which we can deactivate all sensors.
 * To deactivate a sensor we need to push ip*||p - q||^2 energy,
 * However, problem is that there are henchmen. If radius of mpe reach nearest henchman we are lost. 
 * Another thing where we can fail is when we acquired more enery than we actually have i.e. sum(Ip) > Imax
 * Therefore, we will implement binary search to find smallest valid value of k. k = [0, m]. We start at m and will try to lower the value. log(n) ~ log(200) ~ 7
 * We will precompute maximal allowed distance for each mpe by finding closest henchman and compute distance to him.
 * We will use Delaunay triangulation for that. -> h*log(h) + n*log(h) -> 10e6*log(10e6) + 10000*log(10e6) ~ 23 486 032
 * Than in each iteration of binary search we will create Linear program with m variables (one per MPEs intensity) and n constrains (intensity per sensor)
 * Complexity of LP should be fine since min(n,m) <= 100
 * Overall complexity: h*log(h) + n*log(h) + log(m)*O(LP(n,m) + nm)
 */
bool deactivate(int k, std::vector<Sensor> &sensors, std::vector<MPE> &mpes, int Imax){
    Program lp (CGAL::LARGER, true, 0, false, 0);
    for(int i = 0; i < sensors.size(); i++){
        K::Point_2 curr_pt = sensors[i].first;
        int E = sensors[i].second;
        for(int j = 0; j < k; j++){
            double distance = CGAL::squared_distance(curr_pt, mpes[j].first);       // distance between ith sensor and jth mpe
            if(distance <= mpes[j].second){                                         // if distance is smaller than to nearest henchman add it to sum
                lp.set_a(j, i, (double)((double)1/distance));
            }
        }
        lp.set_b(i, E);                                                             // sum of intesities of reachable mpes mus be greater than required intesity
    }

    for(int i = 0; i < k; i++){
        lp.set_c(i, 1);                                                             // minimize intesities
    }

    Solution s = CGAL::solve_linear_program(lp, 0.0);
    assert (s.solves_linear_program(lp));
    return s.objective_value() <= Imax && !s.is_infeasible();                                             // 
}

void testcase(){
    int n, m, h, Imax;          // n -> n.of sensors(max 100), m -> n.of mpes, h -> n.of henchmen, Imax max intensity allowed
    std::cin >> n >> m >> h >> Imax;
    std::vector<Sensor> sensors;        sensors.reserve(n);
    std::vector<MPE> mpes;              mpes.reserve(m);
    std::vector<K::Point_2> henchmen;   henchmen.reserve(h);

    for(int i = 0; i < n; i++){ // load sensors
        K::Point_2 pt;
        int E;
        std::cin >> pt >> E;
        sensors.push_back(Sensor(pt, E));
    }
    for(int i = 0; i < m; i++){ // load mpes
        K::Point_2 pt;
        std::cin >> pt;
        mpes.push_back(MPE(pt, DBL_MAX));
    }
    for(int i = 0; i < h; i++){ // load henchmen
        K::Point_2 pt;
        std::cin >> pt;
        henchmen.push_back(pt);
    }
    if(h > 0){
        Triangulation t;            // create triangulation of henchmen
        t.insert(henchmen.begin(), henchmen.end());
        for(int i = 0; i < m; i++){
            K::Point_2 n_pt = t.nearest_vertex(mpes[i].first)->point();
            mpes[i].second = CGAL::squared_distance(n_pt, mpes[i].first);
        }
    }

    int low = 1, high = m + 1, k; 
    int minK = INT32_MAX;
    while(low < high){          // bin. search for optimal k
        k = low + (high - low) / 2;
        bool res = deactivate(k, sensors, mpes, Imax);
        if(res && k < minK)     // if we can deactivate with k mpes and k is minimal that set minK
            minK = k;
        if(res)
            high = k;           // try find smaller k
        else
            low = k + 1;        // try find bigger valid k
    }
    if(minK == INT32_MAX) std::cout << "impossible\n";
    else std::cout << minK << std::endl;
}

int main(){
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}