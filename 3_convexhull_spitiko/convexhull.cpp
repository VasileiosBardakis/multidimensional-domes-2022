#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include "convexhull.h"
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
   //TODO: y or no for graph
    vector<Point> points, result;

    int limit = 10;
    int current_points = 0;

    cout << "Limit dataset to how many points?" << endl;
    cin >> limit;
    
    //https://stackoverflow.com/questions/14391327/how-to-get-duration-as-int-millis-and-float-seconds-from-chrono
    auto start = high_resolution_clock::now();

    io::CSVReader<3> in("airbnb_listings_usa.csv");
    in.read_header(io::ignore_extra_column, "latitude", "longitude", "price");
    float latitude; float longitude; int price;
    while (in.read_row(latitude, longitude, price))
        {
            //TODO: Is this check worse than culling after with vector functions?
            if (current_points >= limit) break;
            points.push_back({longitude, latitude});
            current_points++;
        }

    cout.precision(5);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parsing took: " << duration.count() << "ms." << endl;

    start = high_resolution_clock::now();
    
    convex_hull(points, result);
    
    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    auto duration_in_micro = duration_cast<microseconds>(stop - start);
    cout << "Convex hull took: " << duration.count() << "ms or " << duration_in_micro.count() << "us." << endl;
    cout << points.size() << " points on the dataset" << endl;
    cout << result.size() << " points on the convex hull" << endl;
    
    cout << "Writing points.csv..." << endl;
    ofstream pointsFile;
    pointsFile.open("points.csv");
    for (auto p : points) {
        pointsFile << p.first << "," << p.second << "\n";
    }
    pointsFile.close();

    cout << "Writing convex.csv..." << endl;
    ofstream convexFile;
    convexFile.open("convex.csv");    
    for (auto p : result) {
        convexFile << p.first << "," << p.second << "\n";
    }
    convexFile.close();

    return 0;
}