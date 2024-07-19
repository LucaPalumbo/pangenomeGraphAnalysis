#ifndef DATASETPARSER_HPP
#define DATASETPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "gfaGraph.hpp"

class DatasetParser {
    private:
        string filename;
        ifstream file;
    public:
        DatasetParser(string filename);
        ~DatasetParser();

        void openFile(string filename);
        void closeFile();
        GfaGraph* parse();
        GfaGraph* parse2();
        void readSegments(GfaGraph *graph);
        void readLinks(GfaGraph *graph);
        void handleHeader(string line);
        void handleSegment(GfaGraph *graph, string line);
        void handleLink(GfaGraph *graph, string line);
        vector<string> split(string line, char delimiter);
};

#endif
