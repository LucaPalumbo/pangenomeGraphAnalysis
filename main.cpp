#include <iostream>
#include "datasetParser.cpp"
#include "gfaGraph.cpp"

using namespace std;

int main() {
    //DatasetParser parser("datasets/example.gfa");
    //DatasetParser parser("datasets/dataset1.gfa");
    //DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");
    DatasetParser parser("datasets/build1.gfa");

    GfaGraph *graph = parser.parse();
    //graph->printSegments();
    //graph->printLinks();
    if (graph->isCyclic()){
        cout << "The graph is cyclic" << endl;
    } else {
        cout << "The graph is acyclic" << endl;
    }
    delete graph;
    return 0;
}