#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include <iterator>
#include "convexhull_benchmark.h"
#include "csv.h"

using namespace std;
using namespace std::chrono;

#define CSV_EXPORT true
#define MAXNUM 6

int main()
{
    /*
    Point:
    1.longitude (x)
    2.latitude (y)
    */
   //TODO: cin << yes or no for graph
    vector<Point> points, result;
    
    //https://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono
    auto start = high_resolution_clock::now();

    io::CSVReader<3> in("airbnb_listings_usa.csv");
    in.read_header(io::ignore_extra_column, "latitude", "longitude", "price");
    double latitude; double longitude; int price;
    while (in.read_row(latitude, longitude, price)) {
            points.push_back({longitude, latitude});
    }

    /** Benchmarking **/
    int i = 1;
    for (i = 1; i < MAXNUM; i++) {
        //https://stackoverflow.com/questions/421573/best-way-to-extract-a-subvector-from-a-vector
        vector<Point>::const_iterator first = points.begin();
        vector<Point>::const_iterator last = points.begin() + 3 * pow(10,i);
        vector<Point> newVec(first, last);

        auto duration = chrono::milliseconds::zero();
        auto duration_in_micro = chrono::microseconds::zero();

        // Average of 3 executions for a better measurement
        for (int j=0; j<3; j++) {
        result.clear();
        auto start = high_resolution_clock::now();
        convex_hull(newVec, result);
        auto stop = high_resolution_clock::now();
        duration += duration_cast<milliseconds>(stop - start);
        duration_in_micro += duration_cast<microseconds>(stop - start);
        }
        cout.precision(5);
        cout << newVec.size() << " points on the dataset" << endl;
        cout << result.size() << " points on the convex hull" << endl;
        cout << duration.count()/3  << "ms or " << duration_in_micro.count()/3 << "us" << endl;
        cout << endl;
    }
    // last subset benchmarked
    vector<Point>::const_iterator first = points.begin();
    vector<Point>::const_iterator last = points.begin() + 3 * pow(10,i-1);
    vector<Point> newVec(first, last);
#if CSV_EXPORT
    cout << "Writing points.csv..." << endl;
    ofstream pointsFile;
    pointsFile.open("points.csv");

    for (auto p : newVec)
        pointsFile << p.first << "," << p.second << "\n";

    pointsFile.close();

    cout << "Writing convex.csv..." << endl;
    ofstream convexFile;
    convexFile.open("convex.csv");
    for (auto p : result) {
        convexFile << p.first << "," << p.second << "\n";
    }
    convexFile.close();
#endif
    return 0;
}