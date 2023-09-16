#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

struct Point {
    int x, y;
    void read(){ scanf("%d %d", &x, &y); }
    Point operator +(const Point& b) const { return Point{x+b.x, y+b.y}; }
    Point operator -(const Point& b) const { return Point{x-b.x, y-b.y}; }
    ll operator *(const Point& b) const { return (ll) x * b.y - (ll) y * b.x; }
    void operator +=(const Point& b) { x += b.x; y += b.y; }
    void operator -=(const Point &b) { x -= b.x; y -= b.y; }
    void operator *=(const int k) { x *= k; y *= k; }

    ll cross(const Point& b, const Point& c) const {
        return (b - *this) * (c - *this);
    }
};

int T;
Point P[4];

Point lineintersect(Point P1, Point P2, Point P3, Point P4) {
    // Initialize the intersection point
    Point intersection;
    
    // Parallel case
    if ((P2.x - P1.x) * (P4.y - P3.y) == (P2.y - P1.y) * (P4.x - P3.x)) {
        // Collinear case, check bounding boxes
        if (P1.x <= max(P3.x, P4.x) && P1.x >= min(P3.x, P4.x) &&
            P1.y <= max(P3.y, P4.y) && P1.y >= min(P3.y, P4.y)) {
            intersection = P1;
        } else if (P2.x <= max(P3.x, P4.x) && P2.x >= min(P3.x, P4.x) &&
                   P2.y <= max(P3.y, P4.y) && P2.y >= min(P3.y, P4.y)) {
            intersection = P2;
        }
    } else {
        ll s1 = (P1.x - P3.x) * (P4.y - P3.y) - (P1.y - P3.y) * (P4.x - P3.x);
        ll s2 = (P2.x - P3.x) * (P4.y - P3.y) - (P2.y - P3.y) * (P4.x - P3.x);
        if ((s1 < 0 && s2 < 0) || (s1 > 0 && s2 > 0)) {
            // Non-intersecting case
            intersection.x = -1;
            intersection.y = -1;
        } else {
            // Calculate the intersection point
            ll denom = (P2.x - P1.x) * (P4.y - P3.y) - (P2.y - P1.y) * (P4.x - P3.x);
            ll numX = ((P1.x * P2.y - P1.y * P2.x) * (P3.x - P4.x) - (P1.x - P2.x) * (P3.x * P4.y - P3.y * P4.x));
            ll numY = ((P1.x * P2.y - P1.y * P2.x) * (P3.y - P4.y) - (P1.y - P2.y) * (P3.x * P4.y - P3.y * P4.x));
            intersection.x = numX / denom;
            intersection.y = numY / denom;
        }
    }
    
    return intersection;
}

int main() {
    // Open the input file for reading
    ifstream inputFile("LSIinput.txt");

    // Check if the file was successfully opened
    if (!inputFile.is_open()) {
        cerr << "Error: Failed to open the input file." << endl;
        return 1;
    }

    // Redirect standard output to the output file
    freopen("LSIoutput.txt", "w", stdout);

    inputFile >> T; // Read the number of test cases from the file

    for (int t = 0; t < T; t++) {
        for (int i = 0; i < 4; i++) {
            // Read the x and y coordinates for each point from the file
            inputFile >> P[i].x >> P[i].y;
        }
        Point intersection = lineintersect(P[0], P[1], P[2], P[3]);
        if (intersection.x == -1 && intersection.y == -1) {
            cout << "NO" << endl;
        } else {
            cout << "YES " << intersection.x << " " << intersection.y << endl;
        }
    }

    // Close the input file
    inputFile.close();

    return 0;
}
