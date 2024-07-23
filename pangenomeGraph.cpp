#include "pangenomeGraph.hpp"

using namespace std;

PangenomeGraph::PangenomeGraph() {}

PangenomeGraph::~PangenomeGraph() {}

void PangenomeGraph::addSegment(Segment segment) {
    segments.push_back(segment);
}

void PangenomeGraph::addSegment(string name, string sequence) {
    Segment segment;
    segment.id = segments.size();
    segment.name = name;
    segment.sequence = sequence;
    segments.push_back(segment);
    segmentIndex[name] = segment.id;
    links.push_back(vector<Link>());
}

void PangenomeGraph::addLink(string from, char fromOrient, string to, char toOrient, string overlap) {
    Link link;
    link.from = segmentIndex[from];
    link.fromOrient = fromOrient;
    link.to = segmentIndex[to];
    link.toOrient = toOrient;
    link.overlap = overlap;
    links[segmentIndex[from]].push_back(link);
}

void PangenomeGraph::printSegments() {
    for (Segment segment : segments) {
        cout << "Segment: " << segment.id << " " << segment.name << " " << segment.sequence << endl;
    }
}

void PangenomeGraph::printLinks() {
    for (int i = 0; i < links.size(); i++) {
        for (Link link : links[i]) {
            cout << "Link: " << i << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
        }
    }
}

void PangenomeGraph::printLinks(string segmentName) {
    int segmentId = segmentIndex[segmentName];
    for (Link link : links[segmentId]) {
        cout << "Link: " << getSegmentName(segmentId) << " " << link.fromOrient << " " << getSegmentName(link.to) << " " << link.toOrient << " " << link.overlap << endl;
    }
}

string PangenomeGraph::getSegmentName(int id) {
    return segments[id].name;
}

bool PangenomeGraph::isSegmentVisitedWithOrientation(int seg_id, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    if (orientation == '+') {
        return visitedPlus[seg_id];
    } else {
        return visitedMinus[seg_id];
    }
}


void PangenomeGraph::setVectorWithOrientation(int seg_id, char orientation, vector<bool> &vectorPlus, vector<bool> &vectorMinus, bool value) {
    if (orientation == '+') {
        vectorPlus[seg_id] = value;
    } else {
        vectorMinus[seg_id] = value;
    }
}

bool PangenomeGraph::isCyclicUtil(int vertex, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus) {
    if (!isSegmentVisitedWithOrientation(vertex, orientation, visitedPlus, visitedMinus)) {
        // Mark the current node as visited and part of recursion stack
        setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
        setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, true);

        for (Link link : links[vertex]) {
            // consider only links that start with the correct orientation
            if (link.fromOrient == orientation) {
                // if node is not visited, then recurse on it
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus) && isCyclicUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
                    return true;
                } 
                // if node is visited and is in the recursion stack, then there is a cycle
                else {
                    if ((link.toOrient == '+' && recStackPlus[link.to]) || (link.toOrient == '-' && recStackMinus[link.to])) {
                        return true;
                    }
                }
            }
        }
    }
    // if execution reaches here, then there is no cycle in this path, so remove the node from the recursion stack
    setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, false);
    return false;
}

bool PangenomeGraph::isCyclic() {
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);

    for (Segment segment : segments) {
        if (isCyclicUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
            return true;
        }
        if (isCyclicUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
            return true;
        }
    }
    return false;
}

void PangenomeGraph::removeBackwardLinksUtil(int vertex, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove) {
    if (!isSegmentVisitedWithOrientation(vertex, orientation, visitedPlus, visitedMinus)) {
        // Mark the current node as visited and part of recursion stack
        setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
        setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, true);

        for (Link link : links[vertex]) {
            // consider only links that start with the correct orientation
            if (link.fromOrient == orientation) {
                // if node is not visited, then recurse on it
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus)) {
                    removeBackwardLinksUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                }
                // if node is visited and is in the recursion stack, then there is a cycle, so add the link to the list of links to remove
                if ((recStackMinus[link.to] && link.toOrient == '-') || (recStackPlus[link.to] && link.toOrient == '+')) {
                    linksToRemove.push_back(link);
                }
            }
        }
    }
    setVectorWithOrientation(vertex, orientation, recStackPlus, recStackMinus, false);
}

void PangenomeGraph::removeBackwardLinks() {
    auto start = std::chrono::high_resolution_clock::now();
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);
    vector<Link> linksToRemove;

    for (Segment segment : segments) {
        removeBackwardLinksUtil(segment.id, '+', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
        removeBackwardLinksUtil(segment.id, '-', visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
    }

    cout << "Links to remove: " << linksToRemove.size() << endl;

    // complex line to remove links from the links vector
    for (const auto linkPtr : linksToRemove) {
        links[linkPtr.from].erase(std::remove_if(links[linkPtr.from].begin(), links[linkPtr.from].end(), [linkPtr](const Link& link) {
            return link.from == linkPtr.from && link.fromOrient == linkPtr.fromOrient && link.to == linkPtr.to && link.toOrient == linkPtr.toOrient && link.overlap == linkPtr.overlap;
        }), links[linkPtr.from].end());
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " milliseconds" << endl;
}

vector<string> PangenomeGraph::findDestinations() {
    // a destination has no outgoing links
    vector<string> destinations;
    for (Segment segment : segments) {
        if (links[segment.id].size() == 0) {
            destinations.push_back(segment.name);
        }
    }
    return destinations;
}

vector<string> PangenomeGraph::findSources() {
    // a source has no incoming links
    vector<bool> isSource(segments.size(), true);
    vector<string> sources;

    for (Segment segment : segments) {
        for (Link link : links[segment.id]) {
            isSource[link.to] = false;
        }
    }
    for (int i = 0; i < isSource.size(); i++) {
        if (isSource[i]) {
            sources.push_back(segments[i].name);
        }
    }
    return sources;
}

bool PangenomeGraph::pathExistsUtil(int vertex, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    // exit condition
    if (vertex == to) {
        return true;
    }
    setVectorWithOrientation(vertex, orientation, visitedPlus, visitedMinus, true);
    for (Link link : links[vertex]) {
        if (link.fromOrient == orientation) {
            if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus) && pathExistsUtil(link.to, link.toOrient, to, visitedPlus, visitedMinus)  ){
                return true;
            }
        }
    }
    // if execution reaches here, then there is no path on this branch
    return false;
}

bool PangenomeGraph::pathExists(string from, string to) {
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    return pathExistsUtil(fromId, '+', toId, visitedPlus, visitedMinus) || pathExistsUtil(fromId, '-', toId, visitedPlus, visitedMinus);
}



void PangenomeGraph::findNPathsUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n) {
    // exit condition - if we have found n paths
    if (paths.size() >= n) {
        return;
    }
    // set recursion stack and push current vertex to path
    setVectorWithOrientation(from, orientation, recStackPlus, recStackMinus, true);
    currentPath.segments.push_back(segments[from]);
    currentPath.orientations.push_back(orientation);

    // if we have reached the destination, add the path to the list of paths
    if (from == to){
        paths.push_back(currentPath);
    }
    else if (currentPath.segments.size() < maxLen) {
        for (Link link: links[from]) {
            if (link.fromOrient == orientation) {
                findNPathsUtil(link.to, link.toOrient, to, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
            }
        }
    }
    // if execution reaches here, then there is no path on this branch
    // remove the current vertex from the path and from the recursion stack
    currentPath.segments.pop_back();
    currentPath.orientations.pop_back();
    setVectorWithOrientation(from, orientation, recStackPlus, recStackMinus, false);

}

vector<Path> PangenomeGraph::findNPaths(string from, string to, int n) {
    int maxLen = 2147483647; // long max
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];

    vector<Path> paths;
    Path currentPath;
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);

    findNPathsUtil(fromId, '+', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
    findNPathsUtil(fromId, '-', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);

    return paths;
}



//  --- extra ---

struct Triple {
    int first;
    char second;
    int third;

    Triple(int f, char s, int t) : first(f), second(s), third(t) {}
};

// Definizione del comparatore
struct CompareTriple {
    bool operator()(const Triple& a, const Triple& b) {
        return a.third > b.third;
    }
};


Path PangenomeGraph::dijkstra(string from, char fromOrient, string to, char toOrient) {
    // initialize single sources
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];
    vector<int> distPlus(segments.size(), 2147483647-2); // long max
    vector<int> distMinus(segments.size(), 2147483647-2 ); // long max
    vector<int> prevPlus(segments.size(), -1);
    vector<char> prevPlusOrient(segments.size(), ' ');
    vector<int> prevMinus(segments.size(), -1);
    vector<char> prevMinusOrient(segments.size(), ' ');
    
    if (fromOrient == '+') {
        distPlus[fromId] = 0;
    }
    else {
        distMinus[fromId] = 0;
    } 

    priority_queue<Triple, vector<Triple>, CompareTriple> pq;
    pq.push( Triple(fromId, fromOrient, 0) );

    for (Segment segment: segments){
        pq.push( Triple(segment.id, '+', distPlus[segment.id]) );
        pq.push( Triple(segment.id, '-', distMinus[segment.id]) );
    }

    
    while (!pq.empty()){
        int nodeId = pq.top().first;
        char orient = pq.top().second;
        int dist = pq.top().third;
        pq.pop();

        for( Link link : links[nodeId] ){
            if (link.fromOrient == orient ){
                if ( link.toOrient == '+' ){
                    if ( distPlus[link.to] > dist + 1 ){
                        distPlus[link.to] = dist + 1;
                        prevPlus[link.to] = nodeId;
                        prevPlusOrient[link.to] = orient;
                        pq.push( Triple(link.to, '+', dist +1 ) );
                    }
                }
                else{
                    if (distMinus[link.to] > dist + 1){
                        distMinus[link.to] = dist + 1;
                        prevMinus[link.to] = nodeId;
                        prevMinusOrient[link.to] = orient;
                        pq.push( Triple(link.to, '-', dist + 1));
                    }
                }
                
            }
        }
    }


    Path path;
    path.from = from;
    path.to = to;
    vector<Segment> segs;
    vector<char> orients;
    segs.push_back(segments[segmentIndex[to]]);
    orients.push_back(toOrient);

    Segment currentNode = segments[segmentIndex[to]];
    char currentOrient = toOrient;
    Segment prevNode;
    char prevOrient;

    while (currentNode.id != fromId && currentNode.id != -1){
        if (currentOrient == '+'){
            prevNode = segments[ prevPlus[ currentNode.id ] ];
            prevOrient = prevPlusOrient[ currentNode.id ];
            segs.push_back( prevNode );
            orients.push_back( prevOrient );
            currentNode = prevNode;
            currentOrient = prevOrient;
        }
        else{
            prevNode = segments[ prevMinus[ currentNode.id ] ];
            prevOrient = prevMinusOrient[ currentNode.id ];
            segs.push_back( prevNode );
            orients.push_back( prevOrient );
            currentNode = prevNode;
            currentOrient = prevOrient;
        }

    }

    reverse(segs.begin(), segs.end());
    reverse(orients.begin(), orients.end());
    path.segments = segs;
    path.orientations = orients;
    return path;
} 