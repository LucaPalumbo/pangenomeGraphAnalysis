#ifndef GFAGRAPH_CPP
#define GFAGRAPH_CPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

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

        void setVisitedAndRecStackWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus){
            if (orientation == '+'){
                visitedPlus[v] = true;
                recStackPlus[v] = true;
            } else {
                visitedMinus[v] = true;
                recStackMinus[v] = true;
            }
        }

        bool isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visiteMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus){
            //cout << "Visiting: " << v << " " << orientation << endl;
            if (!isSegmentVisitedWithOrientation(v, orientation, visitedPlus, visiteMinus)){

                setVisitedAndRecStackWithOrientation(v, orientation, visitedPlus, visiteMinus, recStackPlus, recStackMinus);

                for (Link link : links[v]){
                    //cout << v << " Checking link: " << link.to << " oriented: " << link.toOrient << endl;
                    if (link.fromOrient == orientation){
                        if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visiteMinus) && isCyclicUtil(link.to, link.toOrient, visitedPlus, visiteMinus, recStackPlus, recStackMinus)){
                            //cout << "Cycle detected at segment: " << getSegmentName(v) << " " << orientation << " -> " << getSegmentName(link.to) << " " << link.toOrient << endl;
                            return true;
                        } 
                        else {
                            if ((link.toOrient == '+' && recStackPlus[link.to]) || (link.toOrient == '-' && recStackMinus[link.to])){
                                //cout << "Cycle detected at segment: " << getSegmentName(v) << " " << orientation << " -> " << getSegmentName(link.to) << " " << link.toOrient << endl;
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
            cout << "[+] Checking for cycles" << endl;
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

        void removeBackwardLinksUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove){
            if (!isSegmentVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus)){

                setVisitedAndRecStackWithOrientation(v, orientation, visitedPlus, visitedMinus, recStackPlus, recStackMinus);
                
                for (Link link : links[v]){
                    if (link.fromOrient == orientation){
                        if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus)){
                            removeBackwardLinksUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                        }
                        if ( (recStackMinus[link.to] && link.toOrient == '-') || (recStackPlus[link.to] && link.toOrient == '+') ){
                            linksToRemove.push_back(link);
                            //cout << "Link da rimuovere: {" << link.from << ", " << link.fromOrient << "} -> {" << link.to << ", " << link.toOrient << "}\n";
                        }
                    }
                }
            }
            if (orientation == '+'){
                recStackPlus[v] = false;
            } else {
                recStackMinus[v] = false;
            }
        }


        void removeBackwardLinks(){
            vector<bool> visitedPlus(segments.size(), false);
            vector<bool> visitedMinus(segments.size(), false);
            vector<bool> recStackPlus(segments.size(), false);
            vector<bool> recStackMinus(segments.size(), false);
            vector<Link> linksToRemove;

            for (Segment segment : segments){
                if (!visitedPlus[segment.id]){
                    removeBackwardLinksUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                }
                if (!visitedMinus[segment.id]){
                    removeBackwardLinksUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                }
            }

            //cout << "Links to remove: " << linksToRemove.size() << endl;
            //cout << linksToRemove[linksToRemove.size()-1]->from << " " << linksToRemove[linksToRemove.size()-1]->fromOrient << " " << linksToRemove[linksToRemove.size()-1]->to << " " << linksToRemove[linksToRemove.size()-1]->toOrient << " " << linksToRemove[linksToRemove.size()-1]->overlap << endl;
            for (const auto linkPtr : linksToRemove) {
                cout << "Link da rimuovere: {" << getSegmentName(linkPtr.from) << ", " << linkPtr.fromOrient << "} -> {" << getSegmentName(linkPtr.to) << ", " << linkPtr.toOrient << "}\n";
                // rimuovi
                links[linkPtr.from].erase(std::remove_if(links[linkPtr.from].begin(), links[linkPtr.from].end(), [linkPtr](const Link& link) {
                    return link.from == linkPtr.from && link.fromOrient == linkPtr.fromOrient && link.to == linkPtr.to && link.toOrient == linkPtr.toOrient && link.overlap == linkPtr.overlap;
                }), links[linkPtr.from].end());
            }
        }
        
};


#endif