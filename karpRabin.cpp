#include <cmath>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "karpRabin.hpp"
#include "path.hpp"
#include "pangenomeGraph.hpp"

using namespace std;

KarpRabin::KarpRabin(uint16_t alphabet_size, uint64_t prime_number)
{

    this->prime_number = prime_number;
    this->alphabet_size = alphabet_size;
}

uint64_t KarpRabin::hashString(string sequence)
{
    uint32_t sequence_size = sequence.size();
    uint64_t hash = 0;
    for (int i = 0; i < sequence.size(); i++)
    {
        hash += (static_cast<uint64_t>(sequence[i]) * static_cast<uint64_t>(pow(alphabet_size, sequence_size - i - 1))) % prime_number;
    }
    return hash % prime_number;
}

uint64_t KarpRabin::rehash(uint64_t old_hash, char old_char, char new_char, uint32_t pattern_size)
{
    // new_hash = (old_hash - old_char * alphabet_size^(pattern_size - 1)) * alphabet_size + new_char % prime_number
    // subtraction in modulo prime_number
    // (a - b) % p = (a % p - b % p + p) % p
    int64_t new_hash = (old_hash - (static_cast<uint64_t>(old_char) * static_cast<uint64_t>(pow(alphabet_size, pattern_size - 1))) % prime_number + prime_number) % prime_number;
    new_hash = (new_hash * alphabet_size + static_cast<uint64_t>(new_char)) % prime_number;
    return new_hash;
}

bool KarpRabin::string_compare(string text, string pattern)
{
    return !text.compare(pattern);
}

bool KarpRabin::run(string text, string pattern)
{
    uint32_t pattern_size = pattern.size();
    uint32_t text_size = text.size();
    uint64_t pattern_hash = hashString(pattern);
    uint64_t text_hash = hashString(text.substr(0, pattern_size));

    if (pattern_hash == text_hash && string_compare(text.substr(0, pattern_size), pattern))
    {
        return true;
    }

    char old_char;
    char new_char;
    for (int i = 0; i < text_size - pattern_size; i++)
    {
        old_char = text[i];
        new_char = text[i + pattern_size];
        text_hash = rehash(text_hash, old_char, new_char, pattern_size);
        if (pattern_hash == text_hash && string_compare(text.substr(i + 1, pattern_size), pattern))
        {
            return true;
        }
    }
    return false;
}

bool KarpRabin::runUtil(PangenomeGraph graph, string sequence,
                        int sequence_offset, uint64_t hash, int segment_index, char orientation,
                        vector<bool> &visitedPlus, vector<bool> &visitedMinus)
{

    graph.setVectorWithOrientation(segment_index, orientation, visitedPlus, visitedMinus, true);

    if (orientation == '-')
    {
        sequence += graph.segments[segment_index].complementary();
    }
    else
    {
        sequence += graph.segments[segment_index].sequence;
    }

    if (sequence.empty())
    {
        hash = hashString(sequence.substr(0, pattern.size()));
    }

    while (sequence_offset + pattern.size() <= sequence.size())
    {
        cout << "Sequence: " << sequence << endl;
        cout << "Sequence offset: " << sequence_offset << endl;
        cout << "Substring: " << sequence.substr(sequence_offset, pattern.size()) << endl;
        cout << "Sequence size: " << sequence.size() << endl;
        cout << "Pattern size: " << pattern.size() << endl;
        cout << "Pattern: " << pattern << endl;
        cout << "Hash: " << hash << endl;
        cout << "Pattern hash: " << pattern_hash << endl;
        cout << endl;

        if (hash == pattern_hash && string_compare(sequence.substr(sequence_offset, pattern.size()), pattern))
        {
            cout << "Pattern found in sequence: " << sequence << endl;
            return true;
        }
        hash = rehash(hash, sequence[sequence_offset], sequence[sequence_offset + pattern.size()], pattern.size());
        sequence_offset++;
    }

    for (Link link : graph.links[segment_index])
    {
        if (link.fromOrient == orientation)
        {
            if (runUtil(graph, sequence, sequence_offset, hash, link.to, link.toOrient, visitedPlus, visitedMinus))
            {
                return true;
            }
        }
    }
    return false;
}

bool KarpRabin::run(vector<Path> paths, string pattern)
{
    this->pattern = pattern;
    unordered_map<int, bool> visited;
    pattern_hash = hashString(pattern);
    uint32_t pattern_size = pattern.size();

    // RICOSTRUISCI IL GRAFO DEI PERCORSI SORGENTE-DESTINAZIONE
    unordered_map<string, bool> added_segments;
    unordered_map<string, bool> added_paths;
    PangenomeGraph graph;
    for (Path path : paths)
    {
        for (int i = 0; i < path.segments.size(); i++)
        {
            if (added_segments.find(path.segments[i].name) == added_segments.end())
            {
                graph.addSegment(path.segments[i]);
                added_segments[path.segments[i].name] = true;
            }
        }
    }
    for (Path path : paths)
    {
        for (int i = 0; i < path.segments.size(); i++)
        {
            if (i < path.segments.size() - 1)
            {
                string link_name = path.segments[i].name + path.orientations[i] + path.segments[i + 1].name + path.orientations[i + 1];
                if (added_paths.find(link_name) == added_paths.end())
                {
                    graph.addLink(path.segments[i].name, path.orientations[i], path.segments[i + 1].name, path.orientations[i + 1], "");
                    added_paths[link_name] = true;
                }
            }
        }
    }
    graph.printLinks();
    graph.printSegments();

    vector<bool> visitedPlus(graph.segments.size(), false);
    vector<bool> visitedMinus(graph.segments.size(), false);
    uint64_t hash = hashString(paths[0].getSequence().substr(0, pattern_size));
    string sequence = "";
    if (runUtil(graph, sequence, 0, hash, 0, '+', visitedPlus, visitedMinus) || runUtil(graph, sequence, 0, hash, 0, '-', visitedPlus, visitedMinus))
    {
        return true;
    }
    return false;
}