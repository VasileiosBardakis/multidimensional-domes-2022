#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include "convexhull_fastcsv.h"
#include "csv.h"

using namespace std;
using namespace std::chrono;

int main()
{
    /*
    Point:
    1.longitude (x)
    2.latitude (y)
    */
    vector<Point> points, result;

    int lowPrice = 0;
    int highPrice = 999;
    int limit = 10;
    int current_points = 0;
    /*TODO: Might remove these after, only used for debugging.
    cout << "Enter the lowest price of airbnb: " << endl;
    cin >> lowPrice;

    cout << "Enter the highest price of airbnb: " << endl;
    cin >> highPrice;

    */
    cout << "Limit dataset to how many points?" << endl;
    cin >> limit;
    
    //https://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono
    auto start = high_resolution_clock::now();

    io::CSVReader<3> in("airbnb_listings_usa.csv");
    in.read_header(io::ignore_extra_column, "latitude", "longitude", "price");
    float latitude; float longitude; int price;
    while (in.read_row(latitude, longitude, price))
    //    if ((price >= lowPrice) && (price <= highPrice)) 
        {
            if (current_points >= limit) break;
            points.push_back({longitude, latitude});
            current_points++;
        }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parsing took: " << duration.count() << setprecision(5) << "ms." << endl;

    start = high_resolution_clock::now();
    
    convex_hull(points, result);
    
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    auto duration_in_micro = duration_cast<microseconds>(stop - start);
    cout << "Convex hull took: " << duration.count() << setprecision(5) << "ms or " << duration_in_micro.count() << "us." << endl;

    /* Benchmarking 
    for (int i = 0; i < 5; i++) {

        Points::const_iterator first = points.begin();
        Points::const_iterator last = points.begin() + 3 * pow(10, i);

        Points newVec(first, last);

        start = high_resolution_clock::now();
        CGAL::convex_hull_2(newVec.begin(), newVec.end(), back_inserter(result));
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        auto duration_in_micro = duration_cast<microseconds>(stop - start);
        cout << 3 * pow(10, i) << ": ";
        cout << "Convex hull took: " << duration.count() << setprecision(5) << "ms or " << duration_in_micro.count() << "us" << endl;
    }
    */
    cout << points.size() << " points on the dataset" << endl;
    cout << result.size() << " points on the convex hull" << endl;
    
    //For a lot of points the performance of the below code takes a big hit due to linear read/write
    cout << "Writing points.csv..." << endl;
    ofstream pointsFile;
    pointsFile.open("points.csv");
    for (auto p : points) {
        pointsFile << p.first << "," << p.second << endl;
    }
    pointsFile.close();

    cout << "Writing convex.csv..." << endl;
    ofstream convexFile;
    convexFile.open("convex.csv");    
    for (auto p : result) {
        convexFile << p.first << "," << p.second << endl;
    }
    convexFile.close();

    return 0;
}