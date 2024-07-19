#ifndef KARP_RABIN_HPP
#define KARP_RABIN_HPP

#include <iostream>

using namespace std;

class KarpRabin{
    private:
        uint64_t prime_number;
        uint64_t hash;
        uint16_t alphabet_size;
        
    public:
        KarpRabin(uint16_t alphabet_size, uint64_t prime_number);
        uint64_t hashString(string sequence);
        uint64_t rehash(uint64_t old_hash, char old_char, char new_char, uint32_t pattern_size);
        bool run(string text, string pattern);
        bool string_compare(string text, string pattern);
        
};

#endif