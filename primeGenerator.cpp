#include <iostream>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "primeGenerator.hpp"
#include <bitset>

using namespace std;

PrimeGenerator::PrimeGenerator() {
    seed = time(0);
    srand(seed);
    prime = 0;
}


bool PrimeGenerator::verify(int N, int y){
    int z  = N-1;
    int w = 0;
    while (z % 2 == 0){
        z /= 2;
        w++;
    }
    bool P1 = (__gcd(N,y) == 1) ? true : false;
    bool P2 = (  static_cast<uint64_t>( pow(y,z) ) % N == 1) ? true : false;
    bool P3 = false;
    cout << "z: " << z << endl;
    for (int i = 0; i < w; i++){
        // BUG: powers may return huge numbers. optimized pow(base, exponent, modulo) should be implemented
        int temp = static_cast<uint64_t>( pow(y, static_cast<int>(pow(2,i))*z) ) % N;
        cout << "boh " << temp << endl;
        if ( temp == N-1 ){
            P3 =  true;
        }
    }
    cout << "P1: " << P1 << endl;
    cout << "P2: " << P2 << endl;
    cout << "P3: " << P3 << endl;

    if ( (P1 == false) || ((P2 || P3) == false)){
        return true;
    }
    return false;
}

bool PrimeGenerator::MillerRabin_test(int N, int k){
    for (int i = 0; i < k; i++){
        int y = rand() % (N-2) + 2;
        if (verify(N,y)){
            return false;
        }
    }
    return true;
}

uint64_t PrimeGenerator::getPrime(int n){
    int N = (1 << n) + (genBitsSequence(n-2) << 1) + 1;
    while (MillerRabin_test(N, 10) == false){
        N += 2;
    }
    return N;
}

uint64_t PrimeGenerator::genBitsSequence(int bits){
    int bit;
    uint64_t sequence = 0;
    for (int i = 0; i < bits; i++){
        bit = rand() % 2;
        sequence = sequence + bit;
        if (i < bits - 1){
            sequence = sequence << 1;
        }
    }
    return sequence;

}
