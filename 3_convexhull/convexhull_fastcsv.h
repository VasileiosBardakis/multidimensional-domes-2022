#include <vector>
#include <iostream>
using namespace std;
using Point = pair<int,int>;

int orientation(Point p, Point q, Point r) {
    /*
    Given a vector of points, calculate orientation of top 3
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

void convex_hull(vector<Point> &points, vector<Point> &result) {
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
        result.push_back(point);

        //Can't compute orientation with 2 elements
        if (point == points.begin()[0] || point == points.begin()[1]) 
            continue;

        //???: if (stack.size() >= 3)
        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        while(orientation(result[result.size()-3], result[result.size()-2], result[result.size()-1]) == 2)
            result.erase(result.end()-2);
    }

    /*
    Right to left scan:
    -//-
    */
    //https://stackoverflow.com/questions/8542591/c11-reverse-range-based-for-loop
    for (const auto& point: points) {
        result.push_back(point);

        //Can't compute orientation with 2 elements
        if (point == points.begin()[0] || point == points.begin()[1]) 
            continue;

        //???: if (stack.size() >= 3)
        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        while(orientation(result[result.size()-3], result[result.size()-2], result[result.size()-1]) == 2)
            result.erase(result.end()-2);
    }


}