// Segment Tree implementation for stab queries
// Usage: compile with make, run with ./segment.out
// Add folder "resultsSegment" to create the csv files.
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

#define MINVAL int64_t(-3.6e3)
#define MAXVAL int64_t(3.6e3)
#define NUMOFINTERVALS 10

#define OUTPUT_QUERY_RESULTS true

// Set to true to output run time of insert and query to output file
#define TEST_INSERT_RUNTIME false
#define TEST_QUERY_RUNTIME true
#define TEST_DELETE_RUNTIME true

#if TEST_INSERT_RUNTIME
ofstream ofsInsert("resultsSegment/insertionTime.csv");
#endif

#if TEST_QUERY_RUNTIME
ofstream ofsQueryMatches("resultsSegment/queryMatches.csv");
ofstream ofsQueryTime("resultsSegment/queryTime.csv");
#endif

#if TEST_DELETE_RUNTIME
ofstream ofsDelete("resultsSegment/deletionTime.csv");
#endif

struct Interval {
    int64_t start;
    int64_t end;

    Interval(int64_t start, int64_t end) : start(start), end(end) {}
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

int deleteInterval(Node* node, Interval interval){
    int64_t start = interval.start;
    int64_t end = interval.end;

    if (start <= node->left && end >= node->right) {
        /*
        auto it = find_if(node->intervals.begin(), node->intervals.end(), isEqualInterval);
        
        // Check if the Interval was found
        if (it != node->intervals.end()) {
            // Erase the Interval from the vector
            node->intervals.erase(it); // TODO: Can iterator find object with same values but different address?
            return 0; //Interval spans node range
        */

       //Find interval to delete
        for (auto it = node->intervals.begin(); it != node->intervals.end(); /* no increment here */) {
        if (start == it->start && end == it->end) {
            it = node->intervals.erase(it); // Stops the for loop if the element is found (singular)

            if (node->intervals.size() == 0 && node->leftChild == nullptr && node->rightChild == nullptr) { 
                // No intervals or children in node, so prune it.
                // Call parent node to delete this
                return 1;
            }
        } else {
            ++it; // Move to the next element
        }
    }

    } else {
        int64_t avg = (node->left + node->right)/2;

        if(start < avg){
            if (node->leftChild){
                // No intervals or children in node, so prune it.
                int returnCode = deleteInterval(node->leftChild, interval);
                if (returnCode == 1) {
                    delete node->leftChild;
                    node->leftChild = nullptr;
                }
            }
        }

        if (end > avg){
            if (node->rightChild){
                // No intervals or children in node, so prune it.
                int returnCode = deleteInterval(node->rightChild, interval);
                if (returnCode == 1) {
                    delete node->rightChild;
                    node->rightChild = nullptr;
                }
            }
        }
    }
    return 0;
}

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
#if OUTPUT_QUERY_RESULTS
        // Print all overlapping intervals
        for (int i=0; i<node->intervals.size(); i++){ 
            cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "], ";
        }
#endif
        // Stop searching if no other node exists
        if (!node->leftChild){ return 0;}
        stabQuery(node->leftChild, interval);

#if TEST_QUERY_RUNTIME
        ofsQueryMatches << node->intervals.size() << "+";
#endif
    } else if (end > avg){
#if OUTPUT_QUERY_RESULTS
        // Print all overlapping intervals
        for (int i=0; i<node->intervals.size(); i++){ 
            cout << "[" << node->intervals[i].start << ", " << node->intervals[i].end << "], ";
        }
#endif
        // Stop searching if no other node exists
        if (!node->rightChild){ return 0;}
        stabQuery(node->rightChild, interval);

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
void generateUnitIntervals(vector<Interval> &intervals, int64_t left, int64_t right, int64_t numIntervals){
    for (int i=0; i<numIntervals; i++){
        int64_t start = rand() % (right - left) + left;
        int64_t end = start + 1;
        intervals.push_back(Interval(start, end));
    }
}

int main(){
    srand(time(0));
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
    generateUnitIntervals(queryIntervals, MINVAL, MAXVAL, 5);
    for (Interval interval : queryIntervals){
        cout << "Stab query for [" << interval.start << ", " << interval.end << "]: ";
        auto startTimeQuery = high_resolution_clock::now();
        stabQuery(root, interval);
        cout << "\n\n";

        auto stopTimeQuery = high_resolution_clock::now();
        auto durationQuery = duration_cast<microseconds>(stopTimeQuery - startTimeQuery);
        ofsQueryTime << durationQuery.count() << "\n";
        ofsQueryMatches << "\n";
    }
#endif
    
    cout << "With root: [" << root->left << ", " << root->right << "]:\n";
    printTree(root);
    std::cout << "\n";


    for (Interval interval : intervals){
#if TEST_DELETE_RUNTIME
        auto startTimeDelete = high_resolution_clock::now();
#endif

        cout << "Deletion of interval: [" << interval.start << ", " << interval.end << "]\n";
        deleteInterval(root, interval);

#if TEST_DELETE_RUNTIME
        auto stopTimeDelete = high_resolution_clock::now();
        auto durationDelete = duration_cast<microseconds>(stopTimeDelete - startTimeDelete);
        ofsDelete << durationDelete.count() << "\n";
#endif
    }
    printTree(root);

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

#if TEST_DELETE_RUNTIME
    ofsDelete.flush();
    ofsDelete.close();
#endif
}