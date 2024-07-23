#include <cmath>
#include <iostream>
#include <string>
#include "karpRabin.hpp"

KarpRabin::KarpRabin(uint16_t alphabet_size, uint64_t prime_number){

    this->prime_number = prime_number;
    this->alphabet_size = alphabet_size;
}

uint64_t KarpRabin::hashString(string sequence){
    uint32_t sequence_size = sequence.size();
    uint64_t hash = 0;
    for (int i = 0; i < sequence.size(); i++){
        hash += ( static_cast<uint64_t>(sequence[i]) * static_cast<uint64_t>(pow(alphabet_size, sequence_size - i - 1)) ) % prime_number ;
    }
    return hash % prime_number;
}

uint64_t KarpRabin::rehash(uint64_t old_hash, char old_char, char new_char, uint32_t pattern_size){
    // subtraction in modulo prime_number
    // (a - b) % p = (a % p - b % p + p) % p
    int64_t new_hash = (old_hash - (static_cast<uint64_t>(old_char) * static_cast<uint64_t>(pow(alphabet_size, pattern_size - 1)))% prime_number + prime_number ) % prime_number;
    new_hash = (new_hash * alphabet_size + static_cast<uint64_t>(new_char)) % prime_number;
    return new_hash;
}

bool KarpRabin::string_compare(string text, string pattern){
    return !text.compare(pattern);
}

bool KarpRabin::run(string text, string pattern){
    uint32_t pattern_size = pattern.size();
    uint32_t text_size = text.size();
    uint64_t pattern_hash = hashString(pattern);
    uint64_t text_hash = hashString(text.substr(0, pattern_size));
    
    if (pattern_hash == text_hash && string_compare(text.substr(0, pattern_size), pattern)){
        return true;
    }

    char old_char;
    char new_char;
    for (int i = 0; i < text_size - pattern_size; i++ ){
        old_char = text[i];
        new_char = text[i+pattern_size];
        text_hash = rehash(text_hash, old_char, new_char, pattern_size);
        if (pattern_hash == text_hash && string_compare(text.substr(i+1, pattern_size), pattern)){
            return true;
        }
    }
    return false;
    
}