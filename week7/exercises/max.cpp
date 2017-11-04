#include <iostream>
#include <cassert>
#include <cmath>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int testcase(int p, int a, int b) {
  // by default, we have a nonnegative LP with Ax <= b

  // now set the non-default entries
  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  if(p == 1){
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  // x + y ≤ 4
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); // 4x + 2y ≤ ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  // −x + y ≤ 1

    lp.set_c(Y, -b);                                         // by
    lp.set_d(X, X, 2*a);                                   // -ax^2 but maximize cT x ⇥ minimize  cT x and negate resulting value

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_quadratic_program(lp, ET());
    assert (s.solves_linear_program(lp));
    if(s.is_optimal()){
        std::cout << floor((-1) * to_double(s.objective_value())) << std::endl; //<-
    } else if (s.is_unbounded()){
        std::cout << "unbounded" << std::endl;
    } else 
        std::cout << "no" << std::endl;
  } else {
    Program lp (CGAL::SMALLER, false, 0, true, 0);
    lp.set_a(X, 0,  -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);  // -x - y ≤ 4
    lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b); // - 4x - 2y - z2 <= ab
    lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);  // x - y <= 1
    lp.set_u(Y, true, 0);                                   // -y <= 0
    lp.set_u(X, true, 0);                                   // -x <= 0

    lp.set_l(Z, true, 0);   // Lower bound for z: 0
    lp.set_u(Z, false, 0);  // No upper bound for z

    lp.set_c(Y, b);                                         // by
    lp.set_d(X, X, 2*a);                                   // ax^2
    lp.set_d(Z, Z, 2);                                      //z^4
    
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_quadratic_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if(s.is_optimal()){
        if(ceil(to_double(s.objective_value())) == -0){
            std::cout << "0" << std::endl;
        } else 
            std::cout << ceil(to_double(s.objective_value())) << std::endl;
    } else if (s.is_unbounded()){
        std::cout << "unbounded" << std::endl;
    } else 
        std::cout << "no" << std::endl;
  }

  return 0;
}

int main(){
    std::cout <<std::setprecision(15);
    int p, a, b;
    while(true){
        std::cin >> p;
        if(p == 0)
            break;
        std::cin >> a >> b;
        testcase(p, a, b);
    }
    return 0;
}
