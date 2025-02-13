#ifndef PANGENOMEGRAPH_HPP
#define PANGENOMEGRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <queue>
#include "link.hpp"
#include "segment.hpp"
#include "path.hpp"

using namespace std;


class PangenomeGraph {
    private:
 
        bool isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus);
        void removeBackwardLinksUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove);
        bool pathExistsUtil(int v, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus);
        void findNPathsUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n);
    
    public:
        vector<Segment> segments;
        unordered_map<string, int> segmentIndex;
        vector<vector<Link>> links;
        
        PangenomeGraph();
        ~PangenomeGraph();

        void addSegment(Segment segment);
        void addSegment(string name, string sequence);
        void addLink(string from, char fromOrient, string to, char toOrient, string overlap);
        void printSegments();
        void printLinks();
        void printLinks(string segmentName);
        string getSegmentName(int id);
        bool isCyclic();
        void removeBackwardLinks();
        vector<string> findDestinations();
        vector<string> findSources();
        bool pathExists(string from, string to);
        vector<Path> findNPaths(string from, string to, int n);
        bool isSegmentVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus);
        void setVectorWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, bool value);

        Path dijkstra(string from, char fromOrient, string to, char toOrient);
};

#endif