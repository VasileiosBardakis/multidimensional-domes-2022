// Segment Tree implementation for stab queries
// Usage: compile with make, run with ./segment.out
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

#define MINVAL int64_t(-3.6e7)
#define MAXVAL int64_t(3.6e7)
#define NUMOFINTERVALS 1e6

#define OUTPUT_QUERY_RESULTS true

// Set to true to output run time of insert and query to output file
#define TEST_INSERT_RUNTIME false
#define TEST_QUERY_RUNTIME false

#if TEST_INSERT_RUNTIME
ofstream ofsInsert("results/insertionTime_e7.csv");
#endif

#if TEST_QUERY_RUNTIME
ofstream ofsQueryMatches("results/queryMatches_e7.csv");
ofstream ofsQueryTime("results/queryTime_e7.csv");
#endif

struct Interval {
    int64_t start;
    int64_t end;

    Interval(int64_t start, ino64_t end) : start(start), end(end) {}
};

// Basic building block for making the segment tree
struct Node {
    vector<Interval> intervals;
    int64_t left, right;
    Node* leftChild;
    Node* rightChild;

    Node(int64_t left, int64_t right) : left(left), right(right), leftChild(nullptr), rightChild(nullptr) {}
};

int insertInterval(Node* node, Interval interval){
    int64_t start = interval.start;
    int64_t end = interval.end;

    if (start <= node->left && end >= node->right){
        node->intervals.push_back(interval);
        return 0; //Don't need to search further down this path
    } else {
        int64_t avg = (node->left + node->right)/2;

        if(start < avg){
            // Allocate new node if it does not exist
            if (!node->leftChild){ node->leftChild = new Node(node->left, avg);}
            insertInterval(node->leftChild, interval);
        }

        if (end > avg){
            // Allocate new node if it does not exist
            if (!node->rightChild){ node->rightChild = new Node(avg, node->right);}
            insertInterval(node->rightChild, interval);
        }
    }
    return 0;
}

//TODO: Delete interval.

// Interval should be in the format of [x, x+1] strictly!
int stabQuery(Node* node, Interval interval){
    int64_t start = interval.start;
    int64_t end = interval.end;
    int64_t avg = (node->left + node->right)/2;

    if (end - start != 1) {
        cerr << "Incorect usage of stabQuery, enter two sequential numbers!";
        return -1;
    }

    if(start < avg){
        // Stop searching if no other node exists
        if (!node->leftChild){ return 0;}
        stabQuery(node->leftChild, interval);

#if OUTPUT_QUERY_RESULTS
        for (int i=0; i<node->intervals.size(); i++){ 
            cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "], ";
        }
#endif

#if TEST_QUERY_RUNTIME
        ofsQueryMatches << node->intervals.size() << "+";
#endif
    } else if (end > avg){
        // Stop searching if no other node exists
        if (!node->rightChild){ return 0;}
        stabQuery(node->rightChild, interval);

#if OUTPUT_QUERY_RESULTS
        for (int i=0; i<node->intervals.size(); i++){ 
            cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "], ";
        }
#endif

#if TEST_QUERY_RUNTIME
        ofsQueryMatches << node->intervals.size() << "+";
#endif
    }
    return 0;
}

// For debugging purposes, prints each node and all the intervals it contains
int printTree(Node* node){
    if (node->leftChild) { printTree(node->leftChild); }
    std::cout << node->left << ", " << node->right << ": ";
    for (int i=0; i<node->intervals.size(); i++){ 
        cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "] ";
    }
    std::cout << endl;
    if (node->rightChild) { printTree(node->rightChild); }
    return 0;
}

// Generates intervals to be inserted into the segment tree
void generateIntervals(vector<Interval> &intervals, int64_t left, int64_t right, int64_t numIntervals){
    for (int i=0; i<numIntervals; i++){
        int64_t start = rand() % (right - left) + left;
        int64_t end = rand() % (right - start) + start + 1; // +1 to ensure right >= end > start >= left
        intervals.push_back(Interval(start, end));
    }
}

// Generated intervals to be used for queries
void generateQueryIntervals(vector<Interval> &intervals, int64_t left, int64_t right, int64_t numIntervals){
    for (int i=0; i<numIntervals; i++){
        int64_t start = rand() % (right - left) + left;
        int64_t end = start + 1;
        intervals.push_back(Interval(start, end));
    }
}

int main(){
    vector<Interval> intervals;
    generateIntervals(intervals, MINVAL, MAXVAL, NUMOFINTERVALS);
    Node* root = new Node(MINVAL, MAXVAL);

    for (Interval interval : intervals){
#if TEST_INSERT_RUNTIME
        auto startTimeInsert = high_resolution_clock::now();
#endif

        insertInterval(root, interval);

#if TEST_INSERT_RUNTIME
        auto stopTimeInsert = high_resolution_clock::now();
        auto durationInsert = duration_cast<microseconds>(stopTimeInsert - startTimeInsert);
        ofsInsert << durationInsert.count() << "\n";
#endif
    }

#if TEST_QUERY_RUNTIME
    vector<Interval> queryIntervals;
    generateQueryIntervals(queryIntervals, MINVAL, MAXVAL, NUMOFINTERVALS);
    for (Interval interval : queryIntervals){
        auto startTimeQuery = high_resolution_clock::now();

        stabQuery(root, interval);

        auto stopTimeQuery = high_resolution_clock::now();
        auto durationQuery = duration_cast<microseconds>(stopTimeQuery - startTimeQuery);
        ofsQueryTime << durationQuery.count() << "\n";
        ofsQueryMatches << "\n";
    }
#endif

    // printTree(root);
    std::cout << "\n";

#if TEST_INSERT_RUNTIME
    ofsInsert.flush();
    ofsInsert.close();
#endif

#if TEST_QUERY_RUNTIME
    ofsQueryTime.flush();
    ofsQueryTime.close();
    ofsQueryMatches.flush();
    ofsQueryMatches.close();
#endif
}