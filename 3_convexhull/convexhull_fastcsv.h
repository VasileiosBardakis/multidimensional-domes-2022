#include <vector>
#include <iostream>
using Point = pair<int,int>;

int orientation(Point p, Point q, Point r) {
    /*
    Orientation of p, q, r
    0: colinear
    1: clockwise
    2: counter-clockwise
    
    */
}

bool onSegment(Point p, Point q) {
    /*
    true:   
    false:
    */

}

bool doesIntersect(Point p, Point q) {
    /*
    Using x and y members of p and q,
    true:   intersect
    false:  not intersect
    */
}

bool compare_x(Point p, Point q) {
    return (p.first < q.first);
}

void convex_hull(vector<Point> points, vector<Point> result) {
    /*
    Iterates Points using vector iterator,
    finds its convex hull,
    stores it in result
    */
   

    //Sort based on x-coordinate
    sort(points.begin(), points.end(), compare_x);

    /*
    Left to right scan:
    For each point added, check orientation
    if counter-clockwise call pop (recursive)
    if clockwise continue
    TODO: colinear?
    */
    for (const auto& point: points) {
        //push next point
        result.push_back(point);

        //Cant compute orientation with 2 elements
        if (point == points.front() || point == points.begin()[1]) 
            continue;



        //
    }

    /*
    Right to left scan:
    -//-
    */

}