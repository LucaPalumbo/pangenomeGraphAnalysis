#ifndef GFAGRAPH_CPP
#define GFAGRAPH_CPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct Segment {
    int id;
    string name;
    string sequence;
};

struct Link
{
    int from;
    char fromOrient;
    int to;
    char toOrient;
    string overlap;
};


class GfaGraph {
    private:
        int nSegments;
        int nLinks;
        vector<Segment> segments;
        unordered_map<string, int> segmentIndex;
        vector<vector<Link>> links;

    public:
        GfaGraph(){

        }
        ~GfaGraph(){

        }
        void addSegment(Segment segment){
            segments.push_back(segment);
        }
        void addSegment(string name, string sequence){
            Segment segment;
            segment.id = segments.size();
            segment.name = name;
            segment.sequence = sequence;
            segments.push_back(segment);
            segmentIndex[name] = segment.id;
            links.push_back(vector<Link>());
        }
        
        void addLink(string from, char fromOrient, string to, char toOrient, string overlap){
            Link link;
            link.from = segmentIndex[from];
            link.fromOrient = fromOrient;
            link.to = segmentIndex[to];
            link.toOrient = toOrient;
            link.overlap = overlap;
            links[link.from].push_back(link);
        }


        void printSegments(){
            for (Segment segment : segments){
                cout << "Segment: " << segment.id << " " << segment.name << " " << segment.sequence << endl;
            }
        }
        void printLinks(){
            for (int i = 0; i < links.size(); i++){
                for (Link link : links[i]){
                    cout << "Link: " << link.from << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
                }
            }
        }
};


#endif