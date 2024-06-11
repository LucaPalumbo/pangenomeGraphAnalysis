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
    // int from; <- Not needed
    char fromOrient;
    int to;
    char toOrient;
    string overlap;
};


class GfaGraph {
    private:
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
            //link.from = segmentIndex[from];
            link.fromOrient = fromOrient;
            link.to = segmentIndex[to];
            link.toOrient = toOrient;
            link.overlap = overlap;
            links[segmentIndex[from]].push_back(link);
        }


        void printSegments(){
            for (Segment segment : segments){
                cout << "Segment: " << segment.id << " " << segment.name << " " << segment.sequence << endl;
            }
        }
        void printLinks(){
            for (int i = 0; i < links.size(); i++){
                for (Link link : links[i]){
                    cout << "Link: " << i << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
                }
            }
        }
        void printLinks(string segmentName){
            int segmentId = segmentIndex[segmentName];
            for (Link link : links[segmentId]){
                cout << "Link: " << segmentId << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
            }
        }

        string getSegmentName(int id){
            return segments[id].name;
        }


        bool isSegmentVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus){
            if (orientation == '+'){
                return visitedPlus[v];
            } else {
                return visitedMinus[v];
            }
        }

        bool isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visiteMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus){
            //cout << "Visiting: " << v << " " << orientation << endl;
            if (!isSegmentVisitedWithOrientation(v, orientation, visitedPlus, visiteMinus)){
                if (orientation == '+'){
                    visitedPlus[v] = true;
                    recStackPlus[v] = true;
                } else {
                    visiteMinus[v] = true;
                    recStackMinus[v] = true;
                }

                for (Link link : links[v]){
                    //cout << v << " Checking link: " << link.to << " oriented: " << link.toOrient << endl;
                    if (link.fromOrient == orientation){
                        if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visiteMinus) && isCyclicUtil(link.to, link.toOrient, visitedPlus, visiteMinus, recStackPlus, recStackMinus)){
                            cout << "Cycle detected at segment: " << getSegmentName(v) << " " << orientation << " -> " << getSegmentName(link.to) << " " << link.toOrient << endl;
                            return true;
                        } 
                        else {
                            if (link.toOrient == '+' && recStackPlus[link.to]){
                                cout << "Cycle detected at segment: " << getSegmentName(v) << " " << orientation << " -> " << getSegmentName(link.to) << " " << link.toOrient << endl;
                                return true;
                            }
                            if (link.toOrient == '-' && recStackMinus[link.to]){
                                cout << "Cycle detected at segment: " << getSegmentName(v) << " " << orientation << " -> " << getSegmentName(link.to) << " " << link.toOrient << endl;
                                return true;
                            }
                        }
                    }
                }
            }
            if (orientation == '+'){
                recStackPlus[v] = false;
            } else {
                recStackMinus[v] = false;
            }
            return false;
        }


        bool isCyclic(){
            cout << "Checking for cycles" << endl;
            vector<bool> visitedPlus(segments.size(), false);
            vector<bool> visitedMinus(segments.size(), false);
            vector<bool> recStackPlus(segments.size(), false);
            vector<bool> recStackMinus(segments.size(), false);

            for (Segment segment : segments){
                //cout << "Checking segment: " << segment.id << " oriented: "<< '+' << endl;
                if (!visitedPlus[segment.id] && isCyclicUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus)){
                    return true;
                }
                //cout << "Checking segment: " << segment.id << " oriented: "<< '-' << endl;
                if (!visitedMinus[segment.id] && isCyclicUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus)){
                    return true;
                }
            }
            return false;
        }
};


#endif