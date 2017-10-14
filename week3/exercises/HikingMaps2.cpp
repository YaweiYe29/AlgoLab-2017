#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <boost/optional.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <map>


#include <algorithm>
#include <climits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 S;

using namespace std;

struct Line{
    Point x;
    Point y;
public: Line(Point p1, Point p2){
        x = p1;
        y = p2;
    }
    Line(){}

    void swap(){
        Point tmp = x;
        x = y;
        y = tmp;
    }
};

struct Triangle{
    Line line1;
    Line line2;
    Line line3;

public: Triangle(Line ln1, Line ln2, Line ln3){
        line1 = ln1;
        line2 = ln2;
        line3 = ln3;
    }
};

struct MapPart{
    Triangle* triangle;
    vector<int> containedLegs;
};

Triangle* ConstructTriangle(int* points){
    Line ln1 = Line(Point(points[0], points[1]), Point(points[2], points[3]));
    Line ln2 = Line(Point(points[4], points[5]), Point(points[6], points[7]));
    Line ln3 = Line(Point(points[8], points[9]), Point(points[10], points[11]));

    if(!left_turn(ln1.x, ln1.y, ln2.x)) 
        ln1.swap();

    if(!left_turn(ln2.x, ln2.y, ln3.x)) 
        ln2.swap();

    if(!left_turn(ln3.x, ln3.y, ln1.x)) 
        ln3.swap();

    return new Triangle(ln1, ln2, ln3);
}

bool right(Line line, Point p){
    return right_turn(line.x, line.y, p);
}

bool IsInside(Triangle* triangle, Line* segment){
    return !right(triangle->line1, segment->x) && !right(triangle->line2, segment->x) && !right(triangle->line3, segment->x)
        && !right(triangle->line1, segment->y) && !right(triangle->line2, segment->y) && !right(triangle->line3, segment->y);
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

    Line legs[m-1];
    MapPart maps[n];
    Point prevPoint = *NextPoint();
    int legsInsideCount[m-1];
    set<int> uniqueLegsIn;

    for(int i = 0; i < m - 1; i++){
        Point curr = *NextPoint();
        legs[i] = Line(prevPoint, curr);
        prevPoint = curr;
        legsInsideCount[i] = 0;
    }

    for(int i = 0; i < n; i++){
        cin >> tP[0] >> tP[1] >> tP[2] >> tP[3] >> tP[4] >> tP[5] >> tP[6] >> tP[7] >> tP[8] >> tP[9] >> tP[10] >> tP[11];
        maps[i].triangle = ConstructTriangle(tP);

        for(int j = 0; j < m - 1; j++)
            if(IsInside(maps[i].triangle, &legs[j])) {
                maps[i].containedLegs.push_back(j);
            }
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