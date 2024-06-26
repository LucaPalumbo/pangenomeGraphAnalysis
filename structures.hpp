#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Segment {
    int id;
    string name;
    string sequence;
};

struct Link {
    int from;
    char fromOrient;
    int to;
    char toOrient;
    string overlap;
};

struct Path {
    string from;
    string to;
    vector<string> segments;
    vector<char> orientations;
};