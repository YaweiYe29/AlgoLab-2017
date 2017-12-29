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

// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

// round down to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int p, int a, int b){
    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    if(p == 1){
        // x, y >= 0
        Program * qp = new Program(CGAL::SMALLER, true, 0, false, 0);
        // x + y <= 4
        qp->set_a(X, 0, 1); qp->set_a(Y, 0, 1); qp->set_b(0, 4);
        // 4x + 2y <= ab
        qp->set_a(X, 1, 4); qp->set_a(Y, 1, 2); qp->set_b(1, a * b);
        // -x + y <= 1
        qp->set_a(X, 2, -1); qp->set_a(Y, 2, 1); qp->set_b(2, 1);
        // max by - ax^2
        qp->set_c(Y, -b); qp->set_d(X, X, 2*a);
        // solve, assert and print
        Solution s1 = CGAL::solve_quadratic_program(*qp, ET());
        assert(s1.solves_quadratic_program(*qp));
        if(!s1.is_optimal())
            std::cout << "no\n";
        else if (s1.is_unbounded())
            std::cout << "unbounded\n";
        else
            std::cout << std::setprecision(12) << (-1) * ceil_to_double(s1.objective_value()) << std::endl;
    } else {
        Program * qp = new Program(CGAL::LARGER, false, 0, false, 0);
        // x + y >= -4
        qp->set_a(X, 0, 1); qp->set_a(Y, 0, 1); qp->set_b(0, -4);
        // 4x + 2y + z^2 >= -ab
        qp->set_a(X, 1, 4); qp->set_a(Y, 1, 2); qp->set_a(Z, 1, 1); qp->set_b(1, (-1) * a * b);
        // -x + y >= -1
        qp->set_a(X, 2, -1); qp->set_a(Y, 2, 1); qp->set_b(2, -1);
        // min ax^2 + by + z^4
        qp->set_d(X, X, 2*a); qp->set_c(Y, b); qp->set_d(Z, Z, 2);
        // x, y <= 0
        qp->set_u(X, true, 0);
        qp->set_u(Y, true, 0);
        // solve, asset and print
        Solution s2 = CGAL::solve_quadratic_program(*qp, ET());
        assert(s1.solves_quadratic_program(*qp));
        if(!s2.is_optimal())
            std::cout << "no\n";
        else if (s2.is_unbounded())
            std::cout << "unbounded\n";
        else
            std::cout << std::setprecision(12) << ceil_to_double(s2.objective_value()) << std::endl;
    }
}

int main(){
    int p, a, b;
    std::cin >> p;
    while(p != 0){
        std::cin >> a >> b;
        testcase(p, a, b);
        std::cin >> p;
    }
    return 0;
}