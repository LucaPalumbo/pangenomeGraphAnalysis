#ifndef PATH_HPP
#define PATH_HPP


#include <iostream>
#include <vector>
#include <string>
#include "segment.hpp"

using namespace std;

class Path {
    public:
        string from;
        string to;
        vector<Segment> segments;
        vector<char> orientations;
        string getSequence();
        void printPath();
};

#endif