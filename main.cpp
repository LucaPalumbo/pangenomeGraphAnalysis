#include <iostream>
#include "pangenomeGraph.hpp"
#include "datasetParser.hpp"
#include "karpRabin.hpp"
#include "primeGenerator.hpp"

using namespace std;


int main() {
    //DatasetParser parser("datasets/example.gfa");
    DatasetParser parser("datasets/dataset1.gfa");
    //DatasetParser parser("datasets/build8.gfa");
    //DatasetParser parser("datasets/build1.gfa");
    //DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/chrX.hprc-v1.0-pggb.gfa");
    //DatasetParser parser("datasets/example2var.gfa");

    cout << "[+] Parsing graph" << endl;
    PangenomeGraph *graph = parser.parse();
    cout << "[+] Graph parsed" << endl;

    //if (graph->isCyclic()){
    //    cout << "The graph is cyclic" << endl;
    //    cout << "[+] Removing backward links" << endl;
    //    graph->removeBackwardLinks();
    //} 
    //else {
    //    cout << "The graph is acyclic" << endl;
    //}

    //getSDPairs(graph);

    string source  = "20";
    string destination = "24";
    //if ( graph->pathExists(source, destination) ){
    //    cout << "Path from " << source << " to " << destination << " exists" << endl;
    //} else {
    //    cout << "Path from " << source << " to " << destination << " does not exist" << endl;
    //}


    vector<Path> paths = graph->findNPaths(source, destination,5);
    for( Path path: paths){
        cout << "path: " << endl;
        path.printPath();
        cout << path.getSequence() << endl;
    } 
    //graph->dijkstra(source, '+', destination, '+').printPath();
    
    KarpRabin kr(4, 101);
    string pattern = "AGAG";
    
    bool A = kr.run(paths, pattern);
    cout << "kr.run(paths, pattern): " << A << endl;  
    //string sequence;
    //for( Path path: paths){
    //    sequence = path.getSequence();
    //    cout << "Sequence: " << sequence << endl;
    //    cout << "Pattern: " << pattern << endl;
    //    cout << "Pattern found: " << kr.run(sequence, pattern) << endl;
    //}
    
    //PrimeGenerator pg;
    //cout << pg.verify(101, 65) << endl;
    //cout << pg.MillerRabin_test(102, 100) << endl;
    //cout << pg.getPrime(10) << endl;
    delete graph;
    return 0;
}