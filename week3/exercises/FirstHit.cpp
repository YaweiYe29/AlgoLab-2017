#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <cmath>
#include <boost/optional.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void ReplaceClosest(boost::optional<Point>& currClosest, const Point& potential, Ray& ray){
    if(currClosest){
        if(CGAL::squared_distance(ray.source(), potential) < CGAL::squared_distance(ray.source(), *currClosest))
           currClosest = potential;
    } else {
        currClosest = potential;
    }
}

void ComputeRay(){
    int n;
    long p1x, p1y, p2x, p2y;
    long x1, x2, y1, y2;
    
    
    Ray ray;
    Segment barrier;
    
    std::cin >> n;
    while (n != 0)
    {
        boost::optional<Point> closest;
        std::cin >> p1x >> p1y >> p2x >> p2y;
        //std::cout << "[" << p1x << ";" << p1y << "] [" << p2x << ";" << p2y << "]\n";
        ray = Ray(Point(p1x, p1y), Point(p2x, p2y));
        int i;
        for(i = 0; i < n; i++){
            std::cin >> x1 >> y1 >> x2 >> y2;
            //std::cout << "[" << x1 << ";" << y1 << "] [" << x2 << ";" << y2 << "]\n";
            barrier = Segment(Point(x1,y1), Point(x2, y2));
            if(CGAL::do_intersect(ray, barrier)){
                auto intersection = CGAL::intersection(ray, barrier);
                if (const Point* ip = boost::get<Point>(&*intersection)){
                    ReplaceClosest(closest, *ip, ray);
                } else if (const Segment* is = boost::get<Segment>(&*intersection)){
                    ReplaceClosest(closest, is->source(), ray);
                    ReplaceClosest(closest, is->target(), ray);
                } else
                    exit(1);
                
                //std::cout << "[" << floor_to_double(closest->x()) << ";" << floor_to_double(closest->y()) << "]\n";
            }
        }
        if(!closest)
            std::cout << "no\n";
        else{
            Point& finalP = closest.get();
            std::cout << floor_to_double(finalP.x()) << " " << floor_to_double(finalP.y()) << "\n";
        }
        std::cin >> n;
    }
}

int main()
{
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    ComputeRay();
    return 0;
}
