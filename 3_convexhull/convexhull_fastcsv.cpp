//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/convex_hull_2.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include "convexhull_fastcsv.h"
#include "csv.h"
#include <iomanip>

using namespace std;
using namespace std::chrono;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef K::Point_2 Point_2;
//typedef std::vector<Point_2> Points;

int main()
{
    //Point:
    //1.longitude (x)
    //2.latitude (y)
    using Point = std::pair<int,int>;
    vector<Point> points, result;

    int lowPrice = 0;
    int highPrice = 999;
    int limit;

    cout << "Enter the lowest price of airbnb: " << endl;
    cin >> lowPrice;

    cout << "Enter the highest price of airbnb: " << endl;
    cin >> highPrice;

    cout << "Limit dataset to how many points?" << endl;
    cin >> limit;
    
    
    //https://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono
    auto start = high_resolution_clock::now();

    io::CSVReader<3> in("airbnb_listings_usa.csv");
    in.read_header(io::ignore_extra_column, "latitude", "longitude", "price");
    float latitude; double longitude; int price;
    while (in.read_row(latitude, longitude, price))
        if ((price >= lowPrice) && (price <= highPrice)) {
            points.push_back({longitude, latitude});
        }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parsing took: " << duration.count() << setprecision(5) << "ms." << endl;

    start = high_resolution_clock::now();
    //convexhull function
    //CGAL::convex_hull_2(points.begin(), points.end(), back_inserter(result));
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    auto duration_in_micro = duration_cast<microseconds>(stop - start);
    cout << "Convex hull took: " << duration.count() << setprecision(5) << "ms or " << duration_in_micro.count() << "us." << endl;


    //for (int i = 0; i < 5; i++) {

    //    Points::const_iterator first = points.begin();
    //    Points::const_iterator last = points.begin() + 3 * pow(10, i);

    //    Points newVec(first, last);

    //    start = high_resolution_clock::now();
    //    CGAL::convex_hull_2(newVec.begin(), newVec.end(), back_inserter(result));
    //    stop = high_resolution_clock::now();
    //    duration = duration_cast<milliseconds>(stop - start);
    //    auto duration_in_micro = duration_cast<microseconds>(stop - start);
    //    cout << 3 * pow(10, i) << ": ";
    //    cout << "Convex hull took: " << duration.count() << setprecision(5) << "ms or " << duration_in_micro.count() << "us" << endl;
    //}

    cout << points.size() << " points on the dataset" << endl;
    cout << result.size() << " points on the convex hull" << endl;

    // Using auto with a range-based for loop to iterate through the vector of pairs
    int i=0;
    for (const auto& point : result) {
        cout << i << ". x: " << point.first << " y: " << point.second << endl;

    }

    return 0;
}