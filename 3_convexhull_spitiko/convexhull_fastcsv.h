#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
using Point = pair<float,float>;

void readPoints(const vector<Point> points) {
    cout << "Reading points..." << endl;
    for (const auto& point: points) {
        cout << point.first << " " << point.second << endl;
    }
    cout << endl;
}

int orientation(Point p, Point q, Point r) {
    /*
    Given a vector of points, calculate orientation of top 3
    Orientation of p, q, r
    0: colinear
    1: clockwise
    2: counter-clockwise
    */
    //TODO: In point_2d terms
    cout << "Calculating slope for: " << endl;
    cout << p.first << " " << p.second << endl; 
    cout << q.first << " " << q.second << endl; 
    cout << r.first << " " << r.second << endl; 

    float slope = (q.second - p.second) * (r.first - q.first)
              - (q.first - p.first) * (r.second - q.second);

    cout << "Slope: " << slope << endl << endl;
    if (slope == 0)
    return 0; // collinear
 
    return (slope > 0) ? 1 : 2; // clock or counter-clock wise
    /*
    https://www.geeksforgeeks.org/orientation-3-ordered-points/
    int slope = (p2.y - p1.y) * (p3.x - p2.x)
            - (p2.x - p1.x) * (p3.y - p2.y);
 
    if (slope == 0)
        return 0; // collinear
 
    return (slope > 0) ? 1 : 2; // clock or counter-clock wise
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
    stores it in lstack, rstack
    and combines in result
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
    vector<Point> lstack;
    readPoints(points);
    for (const auto& point: points) {
        lstack.push_back(point);
        readPoints(lstack);

        //Can't compute orientation with 2 elements
        if (point == points.begin()[0] || point == points.begin()[1]) {
            /*
            TODO: Possibility of two identical points messing this up?
            cout << endl;
            cout << point.first << " " << point.second << " entered continue" << endl;
            */
            continue;

        }

        //???: if (stack.size() >= 3)
        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        while(orientation(lstack[lstack.size()-3], lstack[lstack.size()-2], lstack[lstack.size()-1]) == 2 && lstack.size() >= 3)
            lstack.erase(lstack.end()-2);
    }

    cout << "lstack" << endl;
    readPoints(lstack);

    /*
    Right to left scan:
    Everything is in reverse now
    -//-
    */
    //https://stackoverflow.com/questions/8542591/c11-reverse-range-based-for-loop
    vector<Point> rstack;
    //Iterating in reverse using a for loop and auto
    for (auto rit = points.rbegin(); rit != points.rend(); ++rit) {

        rstack.push_back(*rit);

        //Can't compute orientation with 2 elements
        //TODO: Replace with end()
        //???: Difference between begin(), end() etc.
        if (*rit == *(points.end()-2) || *rit == *(points.end())) 
            continue;

        //???: if (stack.size() >= 3)
        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        while(orientation(rstack[rstack.size()-3], rstack[rstack.size()-2], rstack[rstack.size()-1]) == 2 && rstack.size() >= 3)
            rstack.erase(rstack.end()-2);
    }

    cout << "rstack" << endl;
    readPoints(rstack);

    //Reduce



}