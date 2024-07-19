#include <iostream>
#include "gfaGraph.hpp"
#include "datasetParser.hpp"
#include "karpRabin.hpp"

using namespace std;

void interaction(GfaGraph* g){
    while ( true ){
        string seg;
        cin >> seg;
        cout << endl;
        g->printLinks(seg);
    }
}





int main() {
    //DatasetParser parser("datasets/example.gfa");
    DatasetParser parser("datasets/dataset1.gfa");
    //DatasetParser parser("datasets/build1.gfa");
    //DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/chrX.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/example2var.gfa");

    cout << "[+] Parsing graph" << endl;
    GfaGraph *graph = parser.parse2();
    cout << "[+] Graph parsed" << endl;



    if (graph->isCyclic()){
        cout << "The graph is cyclic" << endl;
        cout << "[+] Removing backward links" << endl;
        graph->removeBackwardLinks();
    } 
    else {
        cout << "The graph is acyclic" << endl;
    }


    //getSDPairs(graph);

    string source  = "1";
    string destination = "3204";
    //cout << "Path from " << source << " to " << destination << " exists: " << graph->pathExists(source, destination) << endl;
    
    vector<Path> paths = graph->findNPaths(source, destination, 42);
    //int i = 0;
    //for (Path path : paths) {
    //    cout << "Path: " << i++ << endl;
    //    path.printPath();
    //    cout << endl;
    //}

    KarpRabin kr(4, 101);
    string pattern = "AAATTATAG";
    string sequence;
    for( Path path: paths){
        sequence = path.getSequence();
        cout << "Sequence: " << sequence << endl;
        cout << "Pattern: " << pattern << endl;
        cout << "Pattern found: " << kr.run(sequence, pattern) << endl;
    }


    delete graph;
    return 0;
}