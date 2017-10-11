#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <boost/optional.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <map>


#include <algorithm>
#include <climits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Line_2 Line;
typedef K::Point_2 Point;
typedef K::Triangle_2 Triangle;
typedef K::Segment_2 S;

using namespace std;

struct MapPart{
    Triangle* triangle;
    vector<int> containedLegs;
};

Triangle* ConstructTriangle(int* points){
    Line ln1 = Line(Point(points[0], points[1]), Point(points[2], points[3]));
    Line ln2 = Line(Point(points[4], points[5]), Point(points[6], points[7]));
    Line ln3 = Line(Point(points[8], points[9]), Point(points[10], points[11]));

    const Point* p1;
    const Point* p2;
    const Point* p3;
    auto inter = CGAL::intersection(ln1, ln2);
    auto inter2 = CGAL::intersection(ln2, ln3);
    auto inter3 = CGAL::intersection(ln1, ln3);
    p1 = boost::get<Point>(&*inter);
    p2 = boost::get<Point>(&*inter2);
    p3 = boost::get<Point>(&*inter3);

    return new Triangle(*p1, *p2, *p3);
}

bool IsInside(Triangle* triangle, S* segment){
    if(CGAL::do_intersect(*triangle, *segment)){
        auto in = CGAL::intersection(*triangle, *segment);
        if (const S* s2 = boost::get<S>(&*in)){
            return *s2 == *segment;
        }
    }
    return false;
}

Point* NextPoint(){
    int tmpX, tmpY;
    cin >> tmpX >> tmpY;
    return new Point(tmpX, tmpY);
}

bool AreDuplicated(int* uIn, vector<int> t){
    for(int i = 0; i < t.size(); i++){
        if(uIn[t[i]] <= 1)
            return false;
    }
    return true;
}

void Hike(){
    int m, n;
    int tP[12];
    int minCHF = INT_MAX, b = 0;
    cin >> m >> n;

    S legs[m-1];
    MapPart maps[n];
    Point prevPoint = *NextPoint();
    int legsInsideCount[m-1];
    set<int> uniqueLegsIn;

    for(int i = 0; i < m - 1; i++){
        Point curr = *NextPoint();
        legs[i] = S(prevPoint, curr);
        prevPoint = curr;
        legsInsideCount[i] = 0;
    }

    for(int i = 0; i < n; i++){
        cin >> tP[0] >> tP[1] >> tP[2] >> tP[3] >> tP[4] >> tP[5] >> tP[6] >> tP[7] >> tP[8] >> tP[9] >> tP[10] >> tP[11];
        maps[i].triangle = ConstructTriangle(tP);

        for(int j = 0; j < m - 1; j++)
            if(IsInside(maps[i].triangle, &legs[j]))  
                maps[i].containedLegs.push_back(j);
    }

    bool first = true;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < maps[i].containedLegs.size(); j++){
            int leg = maps[i].containedLegs[j];
            uniqueLegsIn.insert(leg);
            legsInsideCount[leg]++;
        }
        while(AreDuplicated(legsInsideCount, maps[b].containedLegs)){
            for(int j = 0; j < maps[b].containedLegs.size(); j++){
                legsInsideCount[maps[b].containedLegs[j]]--;
            }
            b++;
        }
        if(uniqueLegsIn.size() == m - 1){
            if(i - b + 1 < minCHF){
                minCHF = i - b + 1;
            }
        }
    }

    cout << minCHF << endl;
}

int main(){
    int n;
    cin >> n;
    for(int i = 0; i < n; i++){
        Hike();
    }
    return 0;
}