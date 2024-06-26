#include <iostream>
#include "gfaGraph.hpp"
#include "datasetParser.hpp"

using namespace std;

int main() {
    DatasetParser parser("datasets/example.gfa");
    //DatasetParser parser("datasets/dataset1.gfa");
    //DatasetParser parser("datasets/build5.gfa");
    //DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/chrX.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/example2var.gfa");

    cout << "[+] Parsing graph" << endl;
    GfaGraph *graph = parser.parse();
    cout << "[+] Graph parsed" << endl;


    if (graph->isCyclic()){
        cout << "The graph is cyclic" << endl;
        cout << "[+] Removing backward links" << endl;
        graph->removeBackwardLinks();
    } 
    else {
        cout << "The graph is acyclic" << endl;
    }


    vector<string> sources =  graph->findSources();
    vector<string> dests =  graph->findDestinations();
    if (graph->pathExists(sources[0], dests[0])){
        cout << "Path from " << sources[0] << " to " << dests[0] << " exists" << endl;
    } else {
        cout << "Path from " << sources[0] << " to " << dests[0] << " does not exist" << endl;
    }


    delete graph;
    return 0;
}