#ifndef PRIMEGENERATOR_HPP
#define PRIMEGENERATOR_HPP

#include <iostream>

class PrimeGenerator {
    private:
        int seed;
        uint64_t prime;
    public:
        PrimeGenerator();
        PrimeGenerator(int seed);
        uint64_t getPrime(int n);
        uint64_t genBitsSequence(int n);
        bool MillerRabin_test(int N, int k);
        bool verify(int N, int y);
};

#endif