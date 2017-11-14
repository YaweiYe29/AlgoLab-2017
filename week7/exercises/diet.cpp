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


void testcase(int n, int m) {
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    //for each variable read and set its lower and upper bound
    for (int i = 0; i < n; i++){
        int min_n, max_n;
        std::cin >> min_n >> max_n;
        lp.set_r(i, CGAL::SMALLER);
        lp.set_b(i, max_n);

        lp.set_r(n+i, CGAL::LARGER);
        lp.set_b(n+i, min_n);
    }

    for(int i = 0; i < m; i++){
        int price;
        std::cin >> price;
        lp.set_c(i, price);
        for(int j = 0; j < n; j++){
            int nutrition;
            std::cin >> nutrition;
            lp.set_a(i, j, nutrition);
            lp.set_a(i, n+j, nutrition);
        }
    }

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if(s.is_optimal()){
        std::cout << floor(to_double(s.objective_value())) << std::endl;
    } else 
        std::cout << "No such diet." << std::endl;
}

int main(){
    std::cout <<std::setprecision(15);
    int n, m;
    while(true){
        std::cin >> n >> m;
        if(n == 0 && m == 0)
            break;
        testcase(n, m);
    }
    return 0;
}
