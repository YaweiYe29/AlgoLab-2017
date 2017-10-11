#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#include <iostream>
#include <vector>
#include <climits>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> MinCircle;
typedef K::Point_2 Point;

double ceilN(const K::FT& n){
    double a = ceil(CGAL::to_double(n));
    while (a < n) a += 1;
    while (a - 1 >= n) a -= 1;
    return a;
}

int main(){
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    long x, y;
    int n;

    while(true){
        std::cin >> n;

        K::FT radius = std::numeric_limits<double>::max();

        if(n == 0)
            break;

        if(n == 1){
            std::cin >> n >> n;
            std::cout << "0" << std::endl;
            continue;
        }
        if(n == 2){
            std::cin >> n >> n;
            std::cin >> n >> n;
            std::cout << "0" << std::endl;
            continue;
        }

        std::vector<Point> points(n);

        for(int i = 0; i < n; i++){
            std::cin >> x >> y;
            points[i] = Point(x, y);
        }
        
        MinCircle mc(points.begin(), points.end(), true);
        for(auto it = mc.support_points_begin(); it < mc.support_points_end(); ++it){
            std::vector<Point> copy(points);
            for(auto it2 = copy.begin(); it2 < copy.end(); ++it2){
                if(*it2 == *it){
                    copy.erase(it2);
                }
            }
            MinCircle mc2(copy.begin(), copy.end(), true);
            K::FT rad = mc2.circle().squared_radius();
            if(rad < radius)
                radius = rad;
        }

        CGAL::set_pretty_mode (std::cout);
        std::cout << ceilN(sqrt(radius)) << std::endl;
    }
    return 0;
}