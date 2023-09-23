#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
using namespace std;
using Point = pair<float,float>;

//reference instead of value for performance
void readPoints(const vector<Point>& points) {
    cout << "Reading points..." << endl;
    for (const auto& point: points) {
        cout << setprecision(6) << point.first << " " << setprecision(6) << point.second << endl;
    }
    cout << endl;
}

int orientation(Point &p, Point &q, Point &r) {
    /*
    Given a vector of points, calculate orientation of top 3
    Orientation of p, q, r
    0: colinear
    1: clockwise
    2: counter-clockwise
    */
    //cout << "Calculating slope for: " << endl;
    //cout << p.first << " " << p.second << endl; 
    //cout << q.first << " " << q.second << endl; 
    //cout << r.first << " " << r.second << endl; 

    float slope = (q.second - p.second) * (r.first - q.first)
              - (q.first - p.first) * (r.second - q.second);

    //cout << "Slope: " << slope;
    
    //epsilon value
    // decreasing epsilon makes more artifacts appear
    if (fabs(slope) < 1e-8) {
        if (slope!=0)
            cout << "near 0 slope found: " << slope << endl;
        return 0; // collinear
    }
    
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

bool compare_x(Point p, Point q) {
    if (p.first == q.first) 
        return (p.second > q.second);
    return (p.first < q.first);
}

bool compare_y(Point p, Point q) {
    return (p.second < q.second);
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
    //readPoints(points);

    for (const auto& point: points) {
        result.push_back(point);
        //readPoints(result);

        //Can't compute orientation with 2 elements
        /*
        if (point == points.begin()[0] || point == points.begin()[1]) {
            //TODO: Possibility of two identical points messing this up?
            //cout << endl;
            //cout << point.first << " " << point.second << " entered continue" << endl;
            continue;
        }
        */

        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        /*
        while(orientation(result[result.size()-3], result[result.size()-2], result[result.size()-1]) == 2 && result.size() >= 3)
            result.erase(result.end()-2);
        }
        */
        while(result.size() >= 3) {
            int orientationValue = orientation(result[result.size()-3], result[result.size()-2], result[result.size()-1]);
            switch (orientationValue) {
                case 0: //colinear
                    //find tallest and remove the rest
                    //sort a subset based on y coordinate
                    sort(result.end()-3, result.end()-1, compare_y);
                    result.erase(result.end()-3, result.end()-2);
                    break;

                case 1: //clock
                    break;

                case 2: //counterclock
                    result.erase(result.end()-2);
                    break;
            }
            if (orientationValue != 2)
                break;
        }
    }

    cout << "Left to right finished." << endl;
    //readPoints(result);

    /*
    Right to left scan:
    Everything is in reverse now
    -//-
    */
    //https://stackoverflow.com/questions/8542591/c11-reverse-range-based-for-loop
    vector<Point> rstack;
    cout << "Right to left starting..." << endl;

    //Iterating in reverse using a for loop and auto
    for (auto rit = points.rbegin(); rit != points.rend(); ++rit) {

        rstack.push_back(*rit);

        //Can't compute orientation with 2 elements
        //???: Difference between begin(), end() etc.
        //TODO: MIGHT NOT BE NEEDED!
        /*
        if (*rit == *(points.end()-2) || *rit == *(points.end())) 
            continue;
        */

        //???: if (stack.size() >= 3)
        //While orientation of last 3 is counter clockwise, keep subtracting middle element from the convex hull stack.
        /*
        while(orientation(rstack[rstack.size()-3], rstack[rstack.size()-2], rstack[rstack.size()-1]) == 2 && rstack.size() >= 3)
            rstack.erase(rstack.end()-2);
       while(rstack.size() >= 3) {
            if (orientation(rstack[rstack.size()-3], rstack[rstack.size()-2], rstack[rstack.size()-1]) == 2)
                rstack.erase(rstack.end()-2);
            else break;
        }
        */
        while(rstack.size() >= 3) {
            int orientationValue = orientation(rstack[rstack.size()-3], rstack[rstack.size()-2], rstack[rstack.size()-1]);
            switch (orientationValue) {
                case 0: //colinear
                    //find shortest and remove the rest
                    //sort a subset based on y coordinate
                    sort(rstack.end()-3, rstack.end()-1, compare_y);
                    rstack.erase(rstack.end()-2, rstack.end()-1);
                    break;

                case 1: //clock
                    break;

                case 2: //counterclock
                    rstack.erase(rstack.end()-2);
                    break;
            }
            if (orientationValue != 2)
                break;
        }
    }

    cout << "Right to left finished." << endl;
    //readPoints(rstack);

    //Reduce
    //inefficient
    //https://cplusplus.com/reference/vector/vector/insert/
    //result becomes lstack and rstack inserted to its end
    //cut first and last because they are identical
    cout << "Reducing..." << endl;
    result.insert(result.end(), rstack.begin()+1, rstack.end()-1);
    //readPoints(result);
}