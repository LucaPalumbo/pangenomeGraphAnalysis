#ifndef DATASETPARSER_HPP
#define DATASETPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pangenomeGraph.hpp"

class DatasetParser {
    private:
        string filename;
        ifstream file;
    public:
        DatasetParser(string filename);
        ~DatasetParser();

        void openFile(string filename);
        void closeFile();
        PangenomeGraph* parse();
        PangenomeGraph* parse2();
        void readSegments(PangenomeGraph *graph);
        void readLinks(PangenomeGraph *graph);
        void handleHeader(string line);
        void handleSegment(PangenomeGraph *graph, string line);
        void handleLink(PangenomeGraph *graph, string line);
        vector<string> split(string line, char delimiter);
};

#endif
