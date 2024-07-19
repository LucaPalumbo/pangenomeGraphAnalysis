#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <iostream>
#include <string>

using namespace std;

class Segment{
    public:
        int id;
        string name;
        string sequence;
        string complementary();
};

#endif