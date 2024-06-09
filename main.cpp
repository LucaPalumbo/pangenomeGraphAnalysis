#include <iostream>
#include "datasetParser.cpp"
#include "gfaGraph.cpp"

using namespace std;

int main() {
    DatasetParser parser("datasets/example.gfa");
    GfaGraph *graph = parser.parse();
    graph->printSegments();
    graph->printLinks();
    delete graph;
    return 0;
}