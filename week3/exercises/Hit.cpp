//
//  FirstHit.cpp
//  
//
//  Created by Jakub Lichman on 10/5/17.
//
//

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <cmath>
#include <boost/optional.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

void ComputeRay(){
    int n;
    long p1x, p1y, p2x, p2y;
    long x1, x2, y1, y2;
    
    
    Ray ray;
    Segment barrier;
    bool hit = false;
    //std::cout<<"cau";
    std::cin >> n;
    while (n != 0)
    {
        std::cin >> p1x >> p1y >> p2x >> p2y;
        ray = Ray(Point(p1x, p1y), Point(p2x, p2y));

        for(int i = 0; i < n; i++){
            std::cin >> x1 >> y1 >> x2 >> y2;
            barrier = Segment(Point(x1,y1), Point(x2, y2));

            if(!hit && CGAL::do_intersect(ray, barrier)){
                hit = true;
            }
        }

        if(hit) std::cout << "yes\n";
        else std::cout << "no\n";

        hit = false;
        std::cin >> n;
    }
}




int main()
{
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    ComputeRay();
    return 0;
}