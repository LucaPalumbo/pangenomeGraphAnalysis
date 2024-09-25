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
                        int prev_size, vector<bool> &visitedPlus, vector<bool> &visitedMinus)
{

    // aggiungo il segmento alla sequenza tenendo conto dell'orientazione
    if (orientation == '+')
    {
        sequence += graph.segments[segment_index].sequence;
    }
    else
    {
        sequence += graph.segments[segment_index].complementary();
    }

    // se la sequenza è vuota, calcolo l'hash della prima sottostringa
    if (sequence.empty())
    {
        hash = hashString(sequence.substr(0, pattern.size()));
    }

    // Questo while si occupa di controllare se in questa sequenza è presente il pattern
    while (sequence_offset + pattern.size() <= sequence.size())
    {
        // alla prima chiamata hash è -1, lo setto al valore dell'hash della prima sottostringa
        if (hash == -1)
        {
            hash = hashString(sequence.substr(sequence_offset, pattern.size()));
        }
        // per le altre chiamate ricorsive, ricalcolo l'hash della sottostringa
        else
        {
            hash = rehash(hash, sequence[sequence_offset - 1], sequence[sequence_offset - 1 + pattern.size()], pattern.size());
        }
        // controllo se il segmento è già stato visitato. Se sì, non lo visito di nuovo evitando di controllare due volte la stessa sequenza
        if (graph.isSegmentVisitedWithOrientation(segment_index, orientation, visitedPlus, visitedMinus) && sequence_offset >= prev_size)
        {
            cout << "segment already visited: " << graph.getSegmentName(segment_index) << endl;
            return false;
        }
        // se gli hash corrispodono controllo anche le stringhe per evitare collisioni. Se il pattern è stato trovato, ritorno true
        if (hash == pattern_hash && string_compare(sequence.substr(sequence_offset, pattern.size()), pattern))
        {
            cout << "Pattern found in sequence: " << sequence << endl;
            return true;
        }
        // incremento l'offset della sequenza per controllare la prossima sottostringa
        sequence_offset++;
    }

    for (Link link : graph.links[segment_index])
    {
        if (link.fromOrient == orientation)
        {
            // chiamata ricorsiva per il segmento successivo
            if (runUtil(graph, sequence, sequence_offset, hash, link.to, link.toOrient, sequence.size(), visitedPlus, visitedMinus))
            {
                return true;
            }
        }
    }
    // se arrivo qui ho esplorato completamente questo vertice quindi lo marco come visitato
    graph.setVectorWithOrientation(segment_index, orientation, visitedPlus, visitedMinus, true);
    return false;
}

void KarpRabin::getGraphFromPaths(PangenomeGraph &graph,vector<Path> paths)
{
    unordered_map<string, bool> added_segments;
    unordered_map<string, bool> added_paths;
    // Aggiungo i segmenti al grafo dei percorsi
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
    // Aggiungo i link al grafo dei percorsi
    for (Path path : paths)
    {
        for (int i = 0; i < path.segments.size(); i++)
        {
            if (i < path.segments.size() - 1)
            {
                // link_name è un identificatore univoco dei link. E' la concatenazione dei nomi dei segmenti e delle orientazioni
                string link_name = path.segments[i].name + path.orientations[i] + path.segments[i + 1].name + path.orientations[i + 1];
                if (added_paths.find(link_name) == added_paths.end())
                {
                    graph.addLink(path.segments[i].name, path.orientations[i], path.segments[i + 1].name, path.orientations[i + 1], "");
                    added_paths[link_name] = true;
                }
            }
        }
    }
}

bool KarpRabin::run(vector<Path> paths, string pattern)
{
    this->pattern = pattern;
    pattern_hash = hashString(pattern);
    uint32_t pattern_size = pattern.size();

    // crea il grafo dei percorsi sorgente-destinazione
    PangenomeGraph graph;
    getGraphFromPaths(graph, paths);

    graph.printLinks();
    graph.printSegments();

    // inizializza i vettori ausiliari per la visita dei segmenti per non controllare due volte le stesse sequenze
    vector<bool> visitedPlus(graph.segments.size(), false);
    vector<bool> visitedMinus(graph.segments.size(), false);
    string sequence = "";
    // lancio la funzione ricorsiva
    if (runUtil(graph, sequence, 0, -1, 0, '+', 0, visitedPlus, visitedMinus) || runUtil(graph, sequence, 0, -1, 0, '-', 0, visitedPlus, visitedMinus))
    {
        return true;
    }
    return false;
}