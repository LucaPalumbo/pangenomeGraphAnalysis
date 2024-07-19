#include "segment.hpp"
#include <unordered_map>
#include <string>

string Segment::complementary(){
    static unordered_map<char, char> complement = {
        {'A', 'T'},
        {'T', 'A'},
        {'C', 'G'},
        {'G', 'C'}
    };
    string comp(sequence.size(), ' ');
    for (int i = 0; i < sequence.size(); i++){
        comp[ sequence.size() - i - 1 ] = complement[sequence[i]];
    }
    return comp;
}