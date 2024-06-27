#include <iostream>
#include "gfaGraph.hpp"
#include "datasetParser.hpp"

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
    //DatasetParser parser("datasets/dataset1.gfa");
    //DatasetParser parser("datasets/build1.gfa");
    DatasetParser parser("datasets/chrY.hprc-v1.0-pggb.gfa");
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




    //vector<string> sources =  graph->findSources();
    //vector<string> dests =  graph->findDestinations();


    string s = "45560";
    string d = "45567";
    //interaction(graph);

    cout <<  graph->pathExists(s, d) << endl;

    //vector<string> dests =  graph->findDestinations();
    //string d = "45567";
    //d = "51804";
    //graph->printLinks(s);
    //cout << graph->pathExists(s, d) << endl;


    //cout << graph->pathExists(s, d) << endl;
    //for (string source : sources) {
    //    for (string dest : dests) {
    //        //cout << "Destination: " << dest << endl;
    //        if (graph->pathExists(source, dest)){
    //            cout << "Path from " << source << " to " << dest << " exists" << endl;
    //            d = dest;
    //            s = source;
    //        } else {
    //            //cout << "Path from " << sources[0] << " to " << dest << " does not exist" << endl;
    //        }
    //    }
    //}

    //vector<Path> paths =  graph->findNPaths(s, d, 5);
    //for (Path path : paths) {
    //    cout << "Path from " << path.from << " to " << path.to << " with segments: ";
    //    for (int i = 0; i < path.segments.size(); i++) {
    //        cout << path.segments[i] << path.orientations[i];
    //        if (i < path.segments.size() - 1)
    //            cout << " -> ";
    //    }
    //    cout << endl;
    //}


    delete graph;
    return 0;
}