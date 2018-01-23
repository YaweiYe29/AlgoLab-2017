#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <set>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
 
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

typedef K::Point_2 P;
typedef std::pair<P,int> IPoint;

// round up to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int getSizeOfDifference(std::vector<P> s1, std::vector<P> s2){
    std::vector<P> p(s1.size() + s2.size());
    std::vector<P> p2(s1.size() + s2.size());
    auto it = std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), p.begin());
    auto it2 = std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), p2.begin());
    return (it2 - p2.begin()) - (it - p.begin());
}

void testcase() {
    int n, m, c;                // n -> number of warehouses, m -> number of stadiums, c -> number of contour lines
    std::cin >> n >> m >> c; 

    /*  Problem analysis:
        - linear program where we are trying to maximize profit -> (lp minimizes so we will negate variables and then revert solution to pos. number)
        - we have 0 < n <= 200, 0 < m <= 20, 0 <= c <= 10e6
        - we will have at most 20*200 = 4000 variables (for each edge (warehouse, stadium))
        - we will have 20(alkohol content smaller) + 20(demand equal) + 200(sum of vars of ith warehouse smaller than supply) = 240 constrains
        - min(4000, 240) = 240 which is not smaller than 50 but it is sufficient
        - to deal with contour lines... For 60p (where c <= 100) we can affort O(nmc) = (200 + 20) * 100 = 22 000 and so iterate through all circles
        - for 100p we know that there are again just 100 c. lines that are essential to us. So we reduce them with DT. The center of line thats closest warehouse
          or stadium lies not in its radius is not essential to us. By this method we select just up to 100 c. lines.
    */

    // by default, we have a nonnegative LP with Ax <= b
    Program lp (CGAL::SMALLER, true, 0, false, 0);

    std::vector<P> objects(n + m);
    long max_supply = -1;
    for(int i = 0; i < n; i++){
        int x, y, s, a;     // (x,y) -> position, s -> supply(amount of beer), s -> alkohol content in %
        std::cin >> x >> y >> s >> a;
        objects[i] = P(x, y);
        for(int j = 0; j < m; j++){
            lp.set_a(i * m + j, j, a);
            lp.set_a(i * m + j, j + m, 1);
        }
        lp.set_r(2 * m + i, CGAL::SMALLER);
        lp.set_b(2 * m + i, s);
    }

    for(int i = 0; i < m; i++){
        int x, y, d, u; // (x, y) -> position, d -> demand(amount of beer), u -> upper limit of alcohol consumed
        std::cin >> x >> y >> d >> u;
        objects[i + n] = P(x, y);
        lp.set_r(i, CGAL::SMALLER);
        lp.set_b(i, u * 100);
        lp.set_r(i + m, CGAL::EQUAL);
        lp.set_b(i + m, d);
        for(int j = 0; j < n; j++){
            lp.set_a(j * m + i, 2 * m + j, 1);
        }
    }
    
    std::vector<std::vector<int>> revenues(n, std::vector<int>(m));     // load revenues into vector
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            int r;
            std::cin >> r;
            revenues[i][j] = r;
        }
    }

    Triangulation t;
    t.insert(objects.begin(), objects.end());
    std::vector<IPoint> c_lines;                                     // load lines as vector
    for(int i = 0; i < c; i++){
        int x, y, r;
        std::cin >> x >> y >> r;
        P pt(x, y);
        auto nearest = t.nearest_vertex(pt);
        if(r*r >= CGAL::squared_distance(nearest->point(), pt))
            c_lines.push_back(IPoint(P(x, y), r*r));
    }
    assert(c_lines.size() <= 100);

    std::vector<std::vector<P>> circles(n + m);                            // compute for each object circles it lies in
    for(int i = 0; i < (n + m); i++){
        for(auto it = c_lines.begin(); it != c_lines.end(); ++it){
            if(CGAL::squared_distance(objects[i], it->first) <= it->second)
                circles[i].push_back(it->first);
        }
        std::sort(circles[i].begin(), circles[i].end());
    }

    for(int i = 0; i < n; i++){                                         // go for each edge and check through how many circles it passes
        for(int j = 0; j < m; j++){
            int r = revenues[i][j] * 100;                               // and add revenue to objective function
            int intersections = getSizeOfDifference(circles[i], circles[n + j]);
            assert(intersections <= 100);
            r-=intersections;
            lp.set_c(i * m + j, ET((-1) * r));
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // output solution
    if(s.is_infeasible())
        std::cout << "RIOT!\n";
    else {
        double res = floor_to_double(((-1) * s.objective_value()) / 100);
        std::cout << std::setprecision(15) << res << std::endl;
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while(t--){
        testcase();
    }
    return 0;
}