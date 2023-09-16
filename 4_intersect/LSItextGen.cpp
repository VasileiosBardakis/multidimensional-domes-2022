#include <iostream>
#include <fstream>
#include <ctime>   // For time(NULL)
#include <cstdlib> // For rand() and srand()
using namespace std;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned>(time(NULL)));

    // Number of total test cases
    int numTotalTestCases = 200000;

    // Batch size for generating test cases
    int batchSize = 1000;

    // Range for random coordinates
    int minX = -100;
    int maxX = 100;
    int minY = -100;
    int maxY = 100;

    // Create and open the output file
    ofstream outputFile("LSIinput.txt");

    if (!outputFile.is_open()) {
        cerr << "Error: Failed to open the output file." << endl;
        return 1;
    }

    // Write the number of total test cases to the file
    outputFile << numTotalTestCases << endl;

    for (int i = 0; i < numTotalTestCases; i += batchSize) {
        int numTestCasesInBatch = min(batchSize, numTotalTestCases - i);

        for (int j = 0; j < numTestCasesInBatch; j++) {
            // Generate random points for two line segments
            int x1 = rand() % (maxX - minX + 1) + minX;
            int y1 = rand() % (maxY - minY + 1) + minY;
            int x2 = rand() % (maxX - minX + 1) + minX;
            int y2 = rand() % (maxY - minY + 1) + minY;
            int x3 = rand() % (maxX - minX + 1) + minX;
            int y3 = rand() % (maxY - minY + 1) + minY;
            int x4 = rand() % (maxX - minX + 1) + minX;
            int y4 = rand() % (maxY - minY + 1) + minY;

            // Write the points to the file
            outputFile << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
            outputFile << x3 << " " << y3 << " " << x4 << " " << y4 << endl;
        }
    }

    // Close the output file
    outputFile.close();

    cout << "Input file generated successfully." << endl;

    return 0;
}
