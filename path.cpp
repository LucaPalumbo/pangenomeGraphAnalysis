
#include "path.hpp"

string Path::getSequence(){
    string seq_tot = "";
    for (int i = 0; i < segments.size(); i++){
        if (orientations[i] == '+'){
            seq_tot += segments[i].sequence;
        } else {
            seq_tot += segments[i].complementary();
        }
    }
    return seq_tot;
}

void Path::printPath(){
    for (int i = 0; i < segments.size(); i++){
        cout << segments[i].name << orientations[i] << " ";
        if (i < segments.size() - 1){
            cout << "-> ";
        }
    }
    cout << endl;
}