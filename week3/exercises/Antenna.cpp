#include<iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;

long sqrt_and_ceil(const K::FT& n){
    long a = std::floor(CGAL::to_double(n));
    while (a-1 >= n) a -= 1;
    while (a < n) a += 1;
    return a;
}

int main(){
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    long  x, y;
    int n;
    while(true){
        std::cin >> n;
        if(n == 0)
            break;

        if(n == 1){
            std::cin >> n >> n;
            std::cout << "0" << std::endl;
            continue;
        }
        Point points[n];
        for(int i = 0; i < n; i++){
            std::cin >> x >> y;
            points[i] = Point(x, y);
        }

        Min_circle mc(points, points + n, true);

        CGAL::set_pretty_mode (std::cout);
        std::cout << sqrt_and_ceil(sqrt(mc.circle().squared_radius())) << std::endl;
    }
    return 0;
}