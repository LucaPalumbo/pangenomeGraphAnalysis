#include "datasetParser.hpp"

using namespace std;

DatasetParser::DatasetParser(string filename) {
    this->filename = filename;
    openFile(filename);
}

DatasetParser::~DatasetParser() {
    file.close();
}

void DatasetParser::openFile(string filename) {
    file.open(filename);
    if (!file.is_open()) {
        cout << "Error: Impossible to open the file" << endl;
        exit(1);
    }
}

void DatasetParser::closeFile() {
    file.close();
}

GfaGraph* DatasetParser::parse() {
    GfaGraph *graph = new GfaGraph();
    char type;
    string line;

    while (getline(file, line)) {
        type = line[0];
        switch (type) {
            case 'H':
                handleHeader(line);
                break;
            case 'S':
                handleSegment(graph, line);
                break;
            case 'L':
                handleLink(graph, line);
                break;
            default:
                break;
        }
    }
    return graph;
}

GfaGraph* DatasetParser::parse2(){
    GfaGraph *graph = new GfaGraph();
    readSegments(graph);
    file.clear();
    file.seekg(0);
    readLinks(graph);
    return graph;
}

void DatasetParser::readSegments(GfaGraph *graph) {
    string line;
    while (getline(file, line)) {
        if (line[0] == 'S') {
            handleSegment(graph, line);
        }
    }
}

void DatasetParser::readLinks(GfaGraph *graph){
    string line;
    while (getline(file, line)) {
        if (line[0] == 'L') {
            handleLink(graph, line);
        }
    }

}

void DatasetParser::handleHeader(string line) {
    // Handle the header line
}

void DatasetParser::handleSegment(GfaGraph *graph, string line) {
    vector<string> data = split(line, '\t');
    string name = data[1];
    string sequence = data[2];
    graph->addSegment(name, sequence);
}

void DatasetParser::handleLink(GfaGraph *graph, string line) {
    vector<string> data = split(line, '\t');
    if (data.size() != 6) {
        cout << "Error: Invalid link line" << endl;
        exit(1);
    }
    string from = data[1];
    string fromOrient = data[2];
    string to = data[3];
    string toOrient = data[4];
    string overlap = data[5];
    graph->addLink(from, fromOrient[0], to, toOrient[0], overlap);
}

vector<string> DatasetParser::split(std::string line, char delimiter) {
    vector<string> tokens;
    string token;
    for (char c : line) {
        if (c == delimiter) {
            tokens.push_back(token);
            token = "";
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}
