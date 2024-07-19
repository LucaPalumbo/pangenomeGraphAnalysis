#ifndef GFAGRAPH_HPP
#define GFAGRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include "segment.hpp"
#include "link.hpp"
#include "path.hpp"

using namespace std;



class GfaGraph {
    private:
        vector<Segment> segments;
        unordered_map<string, int> segmentIndex;
        vector<vector<Link>> links;

    public:
        GfaGraph();
        ~GfaGraph();

        void addSegment(Segment segment);
        void addSegment(string name, string sequence);
        void addLink(string from, char fromOrient, string to, char toOrient, string overlap);
        void printSegments();
        void printLinks();
        void printLinks(string segmentName);
        string getSegmentName(int id);
        bool isSegmentVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus);
        void setVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, bool value);
        bool isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus);
        bool isCyclic();
        void removeBackwardLinksUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove);
        void removeBackwardLinks();
        vector<string> findDestinations();
        vector<string> findSources();
        bool pathExistsUtil(int v, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus);
        bool pathExists(string from, string to);
        vector<Path> findNPaths(string from, string to, int n);
        void findNPathsUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n);
};

#endif