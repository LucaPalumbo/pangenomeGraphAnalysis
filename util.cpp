#include <string>
#include <iostream>
#include "path.hpp"
#include "pangenomeGraph.hpp"


using namespace std;


void getSDPairs(PangenomeGraph* graph){
    // than save to files
    vector<string> sources =  graph->findSources();
    vector<string> dests =  graph->findDestinations();
    for (string source : sources) {
        for (string dest : dests) {
            //cout << "Destination: " << dest << endl;
            if (graph->pathExists(source, dest)){
                cout << "Path from " << source << " to " << dest << " YES" << endl;
            } else {
                cout << "Path from " << source << " to " << dest << " NO" << endl;
            }
        }
    }
}

