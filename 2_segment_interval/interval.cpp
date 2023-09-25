// Segment Tree implementation for interval queries
// Utilizes an avl tree to store the intervals
// Usage: compile with make, run with ./interval.out
// Add folder "resultsInterval" to create the csv files.
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

#define MINVAL int64_t(-3.6e6)
#define MAXVAL int64_t(3.6e6)
#define NUMOFINTERVALS 1e2

#define OUTPUT_QUERY_RESULTS true

// Set to true to output run time of insert and query to output file
#define TEST_INSERT_RUNTIME false
#define TEST_QUERY_RUNTIME true
#define TEST_DELETE_RUNTIME false

#if TEST_INSERT_RUNTIME
ofstream ofsInsert("resultsInterval/insertionTime.csv");
#endif

#if TEST_QUERY_RUNTIME
ofstream ofsQuery("resultsInterval/queryTime.csv");
#endif

#if TEST_DELETE_RUNTIME
ofstream ofsDelete("resultsInterval/deletionTime.csv");
#endif

struct Interval {
    int64_t start;
    int64_t end;

    Interval(int64_t start, int64_t end) : start(start), end(end) {}
};

// Comparator logic for intervals
bool operator<(const Interval& a, const Interval& b){
    if(a.start != b.start) return a.start < b.start;
    else return a.end > b.end;
};

bool operator>(const Interval& a, const Interval& b){
    if(a.start != b.start) return a.start > b.start;
    else return a.end < b.end;
};

bool operator<=(const Interval& a, const Interval& b){
    if(a.start != b.start) return a.start <= b.start;
    else return a.end >= b.end;
};

bool operator>=(const Interval& a, const Interval& b){
    if(a.start != b.start) return a.start >= b.start;
    else return a.end <= b.end;
};

bool operator==(const Interval& a, const Interval& b){
    return (a.start == b.start) && (a.end == b.end);
};

// Basic building block for making the interval tree
struct Node {
    Interval interval;
    int64_t max;
    Node* leftChild;
    Node* rightChild;

    Node(Interval &interval) : interval(interval), max(-1), leftChild(nullptr), rightChild(nullptr) {}
};

bool overlapping(Interval a, Interval b){
    return (a.start <= b.end) && (b.start <= a.end);
}

int calculateHeight(struct Node* node){
    if (node == nullptr) return -1;
    int leftHeight = calculateHeight(node->leftChild);
    int rightHeight = calculateHeight(node->rightChild);
    return ((leftHeight>rightHeight) ?  leftHeight : rightHeight) + 1;
}

int calculateBalance(struct Node* node){
    if (node == NULL) return 0;
    return calculateHeight(node->rightChild) - calculateHeight(node->leftChild);
}

struct Node* rightRotate (struct Node* node){
    struct Node* leftNode = node->leftChild;
    struct Node* middleTree = leftNode->rightChild;

    node->leftChild = middleTree;
    leftNode->rightChild = node;

    // Update max if needed 
    // node is A and B is leftNode as per the powerpoint slides
    // A.max = max{A.interval.right, Br.max, Ar.max}
    // B.max = max{B.interval.right, Bl.max, A.max}
    int64_t midTreeMax = (middleTree == nullptr) ? INT64_MIN : middleTree->max;
    int64_t rightChildMax = (node->rightChild == nullptr) ? INT64_MIN : node->rightChild->max;

    node->max = max(node->interval.end, rightChildMax);
    node->max = max(node->max, midTreeMax);

    int64_t leftLeftChildMax = (leftNode->leftChild == nullptr) ? INT64_MIN : leftNode->leftChild->max;

    leftNode->max = max(leftNode->interval.end, leftLeftChildMax);
    leftNode->max = max(leftNode->max, node->max);


    return leftNode;
}

struct Node* leftRotate(struct Node* node){
    struct Node* rightNode = node->rightChild;
    struct Node* middleTree = rightNode->leftChild;

    node->rightChild = middleTree;
    rightNode->leftChild = node;

    // Update max if needed
    // reversed, B is node and A is rightNode as per the powerpoint slides
    // A.max = max{A.interval.right, Br.max, Ar.max}
    // B.max = max{B.interval.right, Bl.max, A.max}
    int64_t midTreeMax = (middleTree == nullptr) ? INT64_MIN : middleTree->max;
    int64_t leftChildMax = (node->leftChild == nullptr) ? INT64_MIN : node->leftChild->max;

    node->max = max(node->interval.end, midTreeMax);
    node->max = max(node->max, leftChildMax);

    int64_t rightRightChildMax = (rightNode->rightChild == nullptr) ? INT64_MIN : rightNode->rightChild->max;

    rightNode->max = max(rightNode->interval.end, rightRightChildMax);
    rightNode->max = max(rightNode->max, node->max);

    return rightNode;

}

// returns next in order node
struct Node* nextInOrder(struct Node* node){
    struct Node* tempNode = node->rightChild;
    while (tempNode->leftChild != nullptr){
        tempNode = tempNode->leftChild;
    }
    return tempNode;
}

Node* insertInterval(Node* node, Interval interval){
    if (node == nullptr){
        // Base case empty node
        node = new Node(interval);
    } else if (interval<node->interval){
        // Case interval < node's interval, go to left subtree.
        node->leftChild = insertInterval(node->leftChild, interval);
    } else if (interval>node->interval){
        // Case interval > node's interval, go to right subtree.
        node->rightChild = insertInterval(node->rightChild, interval);
    } else{
        // Interval is equal, discard.
        return node;
    }

    // Update max if needed
    if (node->max < interval.end){
        node->max = interval.end;
    }
    int balance = calculateBalance(node);

    // Rotations
    // Right Right Case
    if (balance > 1 && interval > node->rightChild->interval){
        return leftRotate(node);
    }
    // Left Left Case
    if (balance < -1 && interval < node->leftChild->interval){
        return rightRotate(node);
    }
    // Right Left Case
    if (balance > 1){
        node->rightChild = rightRotate(node->rightChild);
        return leftRotate(node);
    }
    // Left Right Case
    if (balance < -1){
        node->leftChild = leftRotate(node->leftChild);
        return rightRotate(node);
    }
    // No rotations needed
    return node;
}

Node* deleteInterval(Node* node, Interval interval){
    if (node == nullptr) return node;

    if (interval == node->interval){
        // Node to be deleted found
        if (node->rightChild == nullptr || node->leftChild == nullptr){
            // One child case
            if (node->leftChild != nullptr){
                struct Node* tempNode = node->leftChild;
                *node = *tempNode;
                delete tempNode;
            } else if (node->rightChild != nullptr){
                struct Node* tempNode = node->rightChild;
                *node = *tempNode;
                delete tempNode;
            } else {
                // No child case
                delete node;
                node = nullptr;
            }
        } else {
            // two children case, find inorder successor
            struct Node* tempNode = nextInOrder(node);
            node->interval = tempNode->interval;
            node->rightChild = deleteInterval(node->rightChild, node->interval);
        }
    } else {
         if (node && node->leftChild && node->leftChild->max >= interval.start){
        // If leftChild exists and max value of left subtree is bigger/equal
        //than start value of interval, then candidates in left subtree
        node->leftChild = deleteInterval(node->leftChild, interval);
    } 

    if (node && node->rightChild && node->interval.start <= interval.end){
        // If rightChild exists and start value of node is less/equal than 
        //start value of interval, then candidate in right subtree.
        node->rightChild = deleteInterval(node->rightChild, interval);
    } 
    }

    //if after deletion we have 0 nodes, return.
    if(node == nullptr){
        return node;
    }

    int balance = calculateBalance(node);

    // Rotations
    // Right Right Case
    if (balance > 1 && calculateBalance(node->rightChild)>=0){
        return leftRotate(node);
    }
    // Left Left Case
    if (balance < -1 && calculateBalance(node->leftChild)<=0){
        return rightRotate(node);
    }
    // Right Left Case
    if (balance > 1){
        node->rightChild = rightRotate(node->rightChild);
        return leftRotate(node);
    }
    // Left Right Case
    if (balance < -1){
        node->leftChild = leftRotate(node->leftChild);
        return rightRotate(node);
    }
    // No rotations needed
    return node;

}
void searchInterval(Node* node, Interval interval){
    if (node == nullptr) return;

    if (overlapping(interval, node->interval)){
#if OUTPUT_QUERY_RESULTS
        cout << '[' << node->interval.start << ',' << node->interval.end << "], ";
#endif
    }

    // If leftChild exists and max value of left subtree is bigger/equal
    //than start value of interval, then candidates in left subtree
    if (node->leftChild && node->leftChild->max >= interval.start){
        searchInterval(node->leftChild, interval);
    }

    // If rightChild exists and start value of node is less/equal than 
    //start value of interval, then candidate in right subtree.
    if (node->rightChild && node->interval.start <= interval.end){
        searchInterval(node->rightChild, interval);
    }

}

// Generates intervals to be inserted into the segment tree
void generateIntervals(vector<Interval> &intervals, int64_t left, int64_t right, int64_t numIntervals){
    for (int i=0; i<numIntervals; i++){
        int64_t start = rand() % (right - left) + left;
        int64_t end = rand() % (right - start) + start + 1; // +1 to ensure right >= end > start >= left
        intervals.push_back(Interval(start, end));
    }
}

int main(){
    srand(time(0));
    vector<Interval> intervals;
    generateIntervals(intervals, MINVAL, MAXVAL, NUMOFINTERVALS);
    Node *root = nullptr;

    for(Interval interval : intervals){
#if TEST_INSERT_RUNTIME
    auto startTimeInsert = high_resolution_clock::now();
#endif
        root = insertInterval(root, interval);
#if TEST_INSERT_RUNTIME
        auto stopTimeInsert = high_resolution_clock::now();
        auto durationInsert = duration_cast<microseconds>(stopTimeInsert - startTimeInsert);
        ofsInsert << durationInsert.count() << "\n";
#endif
    }

#if TEST_QUERY_RUNTIME
    vector<Interval> queryIntervals;
    generateIntervals(queryIntervals, MINVAL, MAXVAL, 5);
    for (Interval queryInterval : queryIntervals){
        auto startTimeQuery = high_resolution_clock::now();

        cout << "Search query for [" << queryInterval.start << ", " << queryInterval.end << "]: ";
        searchInterval(root, queryInterval);
        cout << "\n\n";
        
        auto stopTimeQuery = high_resolution_clock::now();
        auto durationQuery = duration_cast<microseconds>(stopTimeQuery - startTimeQuery);
        ofsQuery << durationQuery.count() << "\n";
    }
#endif

#if TEST_DELETE_RUNTIME
    for (int i=0; i<(1e2); i++){
        int64_t randDelIndex = rand() % intervals.size();
        auto startTimeDelete = high_resolution_clock::now();

        cout << "Deletion of interval: [" << intervals[randDelIndex].start << ", " << intervals[randDelIndex].end << "]:\n";
        root = deleteInterval(root, intervals[randDelIndex]);

        auto stoptimeDelete = high_resolution_clock::now();
        auto durationdelete = duration_cast<microseconds>(stoptimeDelete - startTimeDelete);
        ofsDelete << durationdelete.count() << "\n";
    }
#endif

#if TEST_INSERT_RUNTIME
    ofsInsert.flush();
    ofsInsert.close();
#endif

#if TEST_QUERY_RUNTIME
    ofsQuery.flush();
    ofsQuery.close();
#endif

#if TEST_DELETE_RUNTIME
    ofsDelete.flush();
    ofsDelete.close();
#endif

    return 0;
}
