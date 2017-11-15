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


void testcase(int n, int d) {
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    // variable  that we will maximize
    const int R = d;

    // for each inequality
    // load it and create it
    // but on the right side add to vector a ||a||R 
    // i.e.: ai1x1 + ... + aidxd + ||a||R <= bi 
    // ||a|| = sqrt(sum(i = 0 to d-1 of ai^2))
    for (int i = 0; i < n; i++){
        int a, b, sum = 0;
        for (int j = 0; j < d; j++){
            std::cin >> a;
            sum += a*a;
            lp.set_a(j, i, a);
        }
        std::cin >> b;
        sum = sqrt(sum);
        lp.set_a(R, i, sum);
        lp.set_b(i, b);
    }

    lp.set_l(R, true, 0);
    lp.set_c(R, -1);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    // if is optimal than print reversed value(in case of 0 not)
    if(s.is_optimal()){
        int value = floor((int)to_double(s.objective_value()));
        if(value != 0)
            value = value * (-1);
        std::cout << value  << std::endl;
    } else if(s.is_unbounded()) // if is unbounded print out "inf"
        std::cout << "inf" << std::endl;
    else    // else "none"
        std::cout << "none" << std::endl;
}

int main(){
    std::cout <<std::setprecision(15);
    int n, m;
    while(true){
        std::cin >> n;
        if(n == 0)
            break;
        std::cin >> m;
        testcase(n, m);
    }
    return 0;
}
