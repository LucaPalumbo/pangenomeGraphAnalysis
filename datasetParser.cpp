#include "datasetParser.hpp"

DatasetParser::DatasetParser(std::string filename) {
    this->filename = filename;
    openFile(filename);
}

DatasetParser::~DatasetParser() {
    file.close();
}

void DatasetParser::openFile(std::string filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "Error: Impossible to open the file" << std::endl;
        exit(1);
    }
}

GfaGraph* DatasetParser::parse() {
    GfaGraph *graph = new GfaGraph();
    char type;
    std::string line;

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

void DatasetParser::handleHeader(std::string line) {
    // Handle the header line
}

void DatasetParser::handleSegment(GfaGraph *graph, std::string line) {
    std::vector<std::string> data = split(line, '\t');
    std::string name = data[1];
    std::string sequence = data[2];
    graph->addSegment(name, sequence);
}

void DatasetParser::handleLink(GfaGraph *graph, std::string line) {
    std::vector<std::string> data = split(line, '\t');
    if (data.size() != 6) {
        std::cout << "Error: Invalid link line" << std::endl;
        exit(1);
    }
    std::string from = data[1];
    std::string fromOrient = data[2];
    std::string to = data[3];
    std::string toOrient = data[4];
    std::string overlap = data[5];
    graph->addLink(from, fromOrient[0], to, toOrient[0], overlap);
}

std::vector<std::string> DatasetParser::split(std::string line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
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
