#ifndef LINK_HPP
#define LINK_HPP

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Link {
    int from;
    char fromOrient;
    int to;
    char toOrient;
    string overlap;
};

#endif
