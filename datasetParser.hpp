#ifndef DATASETPARSER_HPP
#define DATASETPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "gfaGraph.hpp"

class DatasetParser {
    private:
        std::string filename;
        std::ifstream file;
    public:
        DatasetParser(std::string filename);
        ~DatasetParser();

        void openFile(std::string filename);
        void closeFile();
        GfaGraph* parse();
        GfaGraph* parse2();
        void readSegments(GfaGraph *graph);
        void readLinks(GfaGraph *graph);
        void handleHeader(std::string line);
        void handleSegment(GfaGraph *graph, std::string line);
        void handleLink(GfaGraph *graph, std::string line);
        std::vector<std::string> split(std::string line, char delimiter);
};

#endif
