#include "gfaGraph.hpp"

GfaGraph::GfaGraph() {}

GfaGraph::~GfaGraph() {}

void GfaGraph::addSegment(Segment segment) {
    segments.push_back(segment);
}

void GfaGraph::addSegment(string name, string sequence) {
    Segment segment;
    segment.id = segments.size();
    segment.name = name;
    segment.sequence = sequence;
    segments.push_back(segment);
    segmentIndex[name] = segment.id;
    links.push_back(vector<Link>());
}

void GfaGraph::addLink(string from, char fromOrient, string to, char toOrient, string overlap) {
    Link link;
    link.from = segmentIndex[from];
    link.fromOrient = fromOrient;
    link.to = segmentIndex[to];
    link.toOrient = toOrient;
    link.overlap = overlap;
    links[segmentIndex[from]].push_back(link);
}

void GfaGraph::printSegments() {
    for (Segment segment : segments) {
        cout << "Segment: " << segment.id << " " << segment.name << " " << segment.sequence << endl;
    }
}

void GfaGraph::printLinks() {
    for (int i = 0; i < links.size(); i++) {
        for (Link link : links[i]) {
            cout << "Link: " << i << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
        }
    }
}

void GfaGraph::printLinks(string segmentName) {
    int segmentId = segmentIndex[segmentName];
    for (Link link : links[segmentId]) {
        cout << "Link: " << segmentId << " " << link.fromOrient << " " << link.to << " " << link.toOrient << " " << link.overlap << endl;
    }
}

string GfaGraph::getSegmentName(int id) {
    return segments[id].name;
}

bool GfaGraph::isSegmentVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    if (orientation == '+') {
        return visitedPlus[v];
    } else {
        return visitedMinus[v];
    }
}


void GfaGraph::setVisitedWithOrientation(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, bool value) {
    if (orientation == '+') {
        visitedPlus[v] = value;
    } else {
        visitedMinus[v] = value;
    }
}

bool GfaGraph::isCyclicUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus) {
    if (!isSegmentVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus)) {
        setVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus, true);
        setVisitedWithOrientation(v, orientation, recStackPlus, recStackMinus, true);


        for (Link link : links[v]) {
            if (link.fromOrient == orientation) {
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus) && isCyclicUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus)) {
                    return true;
                } else {
                    if ((link.toOrient == '+' && recStackPlus[link.to]) || (link.toOrient == '-' && recStackMinus[link.to])) {
                        return true;
                    }
                }
            }
        }
    }
    setVisitedWithOrientation(v, orientation, recStackPlus, recStackMinus, false);
    return false;
}

bool GfaGraph::isCyclic() {
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

void GfaGraph::removeBackwardLinksUtil(int v, char orientation, vector<bool> &visitedPlus, vector<bool> &visitedMinus, vector<bool> &recStackPlus, vector<bool> &recStackMinus, vector<Link> &linksToRemove) {
    if (!isSegmentVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus)) {
        setVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus, true);
        setVisitedWithOrientation(v, orientation, recStackPlus, recStackMinus, true);

        for (Link link : links[v]) {
            if (link.fromOrient == orientation) {
                if (!isSegmentVisitedWithOrientation(link.to, link.toOrient, visitedPlus, visitedMinus)) {
                    removeBackwardLinksUtil(link.to, link.toOrient, visitedPlus, visitedMinus, recStackPlus, recStackMinus, linksToRemove);
                }
                if ((recStackMinus[link.to] && link.toOrient == '-') || (recStackPlus[link.to] && link.toOrient == '+')) {
                    linksToRemove.push_back(link);
                }
            }
        }
    }
    setVisitedWithOrientation(v, orientation, recStackPlus, recStackMinus, false);
}

void GfaGraph::removeBackwardLinks() {
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
    for (const auto linkPtr : linksToRemove) {
        links[linkPtr.from].erase(std::remove_if(links[linkPtr.from].begin(), links[linkPtr.from].end(), [linkPtr](const Link& link) {
            return link.from == linkPtr.from && link.fromOrient == linkPtr.fromOrient && link.to == linkPtr.to && link.toOrient == linkPtr.toOrient && link.overlap == linkPtr.overlap;
        }), links[linkPtr.from].end());
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " milliseconds" << endl;
}

vector<string> GfaGraph::findDestinations() {
    vector<string> destinations;
    for (Segment segment : segments) {
        if (links[segment.id].size() == 0) {
            destinations.push_back(segment.name);
        }
    }
    for (string destination : destinations) {
        cout << "Destination: " << destination << endl;
    }
    return destinations;
}

vector<string> GfaGraph::findSources() {
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
    //for (string source : sources) {
    //    cout << "Source: " << source << endl;
    //}
    return sources;
}

bool GfaGraph::pathExistsUtil(int v, char orientation, int to, vector<bool> &visitedPlus, vector<bool> &visitedMinus) {
    if (v == to) {
        return true;
    }
    setVisitedWithOrientation(v, orientation, visitedPlus, visitedMinus, true);
    for (Link link : links[v]) {
        if (link.fromOrient == orientation) {
            if (!isSegmentVisitedWithOrientation(v, link.toOrient, visitedPlus, visitedMinus) && pathExistsUtil(link.to, link.toOrient, to, visitedPlus, visitedMinus)) {
                return true;
            }
        }
    }
    return false;
}

bool GfaGraph::pathExists(string from, string to) {
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];
    vector<bool> visitedPlus(segments.size(), false);
    vector<bool> visitedMinus(segments.size(), false);
    return pathExistsUtil(fromId, '+', toId, visitedPlus, visitedMinus) || pathExistsUtil(fromId, '-', toId, visitedPlus, visitedMinus);
}



void GfaGraph::findNPathUtil(int from, char orientation, int to, vector<Path> &paths, Path &currentPath, vector<bool> &recStackPlus, vector<bool> &recStackMinus, int maxLen, int n) {

    setVisitedWithOrientation(from, orientation, recStackPlus, recStackMinus, true);
    currentPath.segments.push_back(getSegmentName(from));
    currentPath.orientations.push_back(orientation);


    if (from == to){
        paths.push_back(currentPath);
    }
    else if (currentPath.segments.size() < maxLen) {
        for (Link link: links[from]) {
            if (link.fromOrient == orientation) {
                findNPathUtil(link.to, link.toOrient, to, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
            }
        }
    }

    currentPath.segments.pop_back();
    currentPath.orientations.pop_back();
    setVisitedWithOrientation(from, orientation, recStackPlus, recStackMinus, false);

}

vector<Path> GfaGraph::findNPath(string from, string to, int n) {
    int maxLen = 10;
    int fromId = segmentIndex[from];
    int toId = segmentIndex[to];

    vector<Path> paths;
    Path currentPath;
    vector<bool> recStackPlus(segments.size(), false);
    vector<bool> recStackMinus(segments.size(), false);

    findNPathUtil(fromId, '+', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);
    findNPathUtil(fromId, '-', toId, paths, currentPath, recStackPlus, recStackMinus, maxLen, n);

    return paths;
}