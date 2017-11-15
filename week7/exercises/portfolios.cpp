#include <iostream>
#include <cassert>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m) {
    Program qp(CGAL::SMALLER, true, 0, false, 0);
    const int COSTS = 0;
    const int RETURNS = 1;

    qp.set_r(COSTS, CGAL::LARGER);
    qp.set_r(RETURNS, CGAL::SMALLER);

    // for each cost and return set it to first or second inequality
    for (int i = 0; i < n; i++){
        int cost, ret;
        std::cin >> cost >> ret;
        qp.set_a(i, COSTS, ret);
        qp.set_a(i, RETURNS, cost); 
    }

    // read covariance matrix and set it to minimal function
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int covariance;
            std::cin >> covariance;
            //if(i < j) continue;
            qp.set_d(i, j, 2*covariance);
        }
    }

    for (int i = 0; i < m; i++){
        int C, R, V;
        std::cin >> C >> R >> V;

        qp.set_b(COSTS, R);
        qp.set_b(RETURNS, C);

        // solve the program, using ET as the exact type
        assert(qp.is_nonnegative());
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        assert (s.solves_quadratic_program(qp));

        if(s.is_optimal()){
            ET risk = s.objective_value().numerator() / s.objective_value().denominator();
            if(risk <= V)
                std::cout << "Yes." << std::endl;
            else
                std::cout << "No." << std::endl;
        } else 
            std::cout << "No." << std::endl;
    }
}

int main(){
    int n, m;
    while(true){
        std::cin >> n >> m;
        if(n == 0 && m == 0)
            break;
        testcase(n, m);
    }
    return 0;
}
