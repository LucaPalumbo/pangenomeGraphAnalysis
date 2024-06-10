#ifndef DATASETPARSER_CPP
#define DATASETPARSER_CPP

#include <iostream>
#include <fstream>
#include <string>
#include "gfaGraph.cpp"

using namespace std;


class DatasetParser {
    private:
        string filename;
        ifstream file;
    public:
        DatasetParser(string filename){
            this->filename = filename;
            openFile(filename);
        }
        ~DatasetParser(){
            file.close();
        }

        void openFile(string filename){
            file.open(filename);
            if (!file.is_open()){
                cout << "Error: Impossible to open the file" << endl;
                exit(1);
            }
        }

        GfaGraph* parse(){
            GfaGraph *graph = new GfaGraph();
            char type;
            string line;

            while ( getline(file, line) ) {
                type = line[0];
                switch (type){
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

        void handleHeader(string line){
            //cout << "Header: " << line << endl;
        }
        void handleSegment(GfaGraph *graph, string line){
            vector<string> data = split(line, '\t');
            /*if (data.size() != 3){
                cout << "Error: Invalid segment line" << endl;
                cout << line << endl;
                exit(1);
            }*/
            string name = data[1];
            string sequence = data[2];
            graph->addSegment(name, sequence);

        }
        void handleLink(GfaGraph *graph, string line){
            vector<string> data = split(line, '\t');
            if (data.size() != 6){
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

        vector<string> split(string line, char delimiter){
            vector<string> tokens;
            string token;
            for (char c : line){
                if (c == delimiter){
                    tokens.push_back(token);
                    token = "";
                } else {
                    token += c;
                }
            }
            tokens.push_back(token);
            return tokens;
        }
};

#endif