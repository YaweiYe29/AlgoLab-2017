#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round down to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n, int m){
    // 2n number of constrains
    // m number of variables
    Program * lp = new Program(CGAL::LARGER, true, 0, false, 0);
    // go for bounds and them to 2n constrains as b
    for(int i = 0; i < n; i++){
        int lower, upper;
        std::cin >> lower >> upper;
        lp->set_b(i, lower);
        lp->set_b(i + n, upper);
        lp->set_r(i, CGAL::LARGER);
        lp->set_r(i + n, CGAL::SMALLER);
    }

    // each food(variables) will appear in every constrain multiplied with number of appropriate nutrient
    for(int i = 0; i < m; i++){
        int price;
        std::cin >> price;
        lp->set_c(i, price);
        for(int j = 0; j < n; j++){
            int nutrient;
            std::cin >> nutrient;
            lp->set_a(i, j, nutrient);
            lp->set_a(i, j + n, nutrient);
        }
    }

    // solve it!
    Solution s = CGAL::solve_linear_program(*lp, ET());
    if(s.is_infeasible()){
        std::cout << "No such diet.\n";
    } else {
        std::cout << std::setprecision(12) << floor_to_double(s.objective_value()) << std::endl;
    }
}

int main(){
    int n, m;
    std::cin >> n >> m;
    while(n != 0 && m != 0){
        testcase(n, m);
        std::cin >> n >> m;
    }
    return 0;
}