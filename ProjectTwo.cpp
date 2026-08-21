//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Alfredo Trevizo
// Version     : 1.0
// Description : CS-300 Project Two - ABCU Program 
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// ====================== Course Object ======================
// This struct represents a course with its number, title, and prerequisites.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() {}
};

// ====================== Binary Search Tree Node ======================
// This struct represents a node in the binary search tree, containing a
// Course object and pointers to left and right child nodes.
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse) : Node() {
        course = aCourse;
    }
};

// ====================== Binary Search Tree Class ======================
// This class implements a binary search tree to store Course objects,
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void destroyTree(Node* node);
    Node* searchNode(Node* node, string courseNumber);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Course course);
    void PrintCourseList();
    Course Search(string courseNumber);
    bool IsEmpty();
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor that will help free all nodes in the tree
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
//Check if the tree is empty
bool BinarySearchTree::IsEmpty() {
    return root == nullptr;
}
//public insert function
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}
// recursive function helper 
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}
// public print course list function
void BinarySearchTree::PrintCourseList() {
    cout << "Here is a sample schedule:" << endl << endl;
    inOrder(root);
    cout << endl;
}
//resurective function to print the course list in order
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }
}
// public search function
Course BinarySearchTree::Search(string courseNumber) {
    Node* result = searchNode(root, courseNumber);
    if (result != nullptr) {
        return result->course;
    }
    Course empty;
    return empty;
}
// recursive function to search for a node
Node* BinarySearchTree::searchNode(Node* node, string courseNumber) {
    if (node == nullptr || node->course.courseNumber == courseNumber) {
        return node;
    }
    if (courseNumber < node->course.courseNumber) {
        return searchNode(node->left, courseNumber);
    }
    return searchNode(node->right, courseNumber);
}

// Helper functions 
string toUpper(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}
// Trim whitespace from both ends of a string
string trim(const string& str) {
	// Find the first and last non-whitespace characters
    size_t first = str.find_first_not_of(" \t\r\n");
	// If the string is all whitespace, return an empty string
    if (first == string::npos) return "";
	// Find the last non-whitespace character
    size_t last = str.find_last_not_of(" \t\r\n");
	// Return the substring that excludes leading and trailing whitespace
    return str.substr(first, last - first + 1);
}

// Load courses from file
void loadCourses(string filename, BinarySearchTree& bst) {
    cout << "Loading file " << filename << endl;
	//open the file
    ifstream file(filename);
	// Check if the file was opened successfully
    if (!file.is_open()) {
        cout << "Error: Unable to open file check type." << filename << endl;
        return;
    }
	// Read the file line by line
    string line;
    int count = 0;

    

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() < 2) {
            continue;  // skip bad lines
        }
		// Create a Course object and populate it
        Course course;
        course.courseNumber = toUpper(tokens[0]);
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(toUpper(tokens[i]));
            }
        }

        bst.Insert(course);
        count++;
    }

    file.close();
    cout << count << " courses read." << endl;
}
//
void printCourse(Course course) {
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// ====================== Main ======================
int main(int argc, char* argv[]) {
    // Default file name 
    string filename = "CS 300 ABCU_Advising_Program_Input.csv";

    // Allow command-line override
    if (argc >= 2) {
        filename = argv[1];
    }

    BinarySearchTree bst;
    int choice = 0;
    string courseNum;

    cout << "Welcome to the ABCU course viewer." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1:
            
            loadCourses(filename, bst);
            break;

        case 2:
            if (bst.IsEmpty()) {
                cout << "Please load the data first." << endl;
            }
            else {
                bst.PrintCourseList();
            }
            break;

        case 3:
            if (bst.IsEmpty()) {
                cout << "Please load the data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                cin >> courseNum;
                courseNum = toUpper(courseNum);
                Course found = bst.Search(courseNum);
                printCourse(found);
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}