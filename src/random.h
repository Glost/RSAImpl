/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Pseudorandom generator
 */

#ifndef RSAIMPL_RANDOM_H
#define RSAIMPL_RANDOM_H

#include "long_arithm.h"

namespace RSAImpl {

    static const int A = 1103515245;
    static const int C = 12345;
    static const unsigned long long M = 2147483648;
    static const int DEFAULT_SEED = 3274983;
    static const int DEFAULT_LONG_INT_SIZE = 128;

    class Random {

    public:

        Random(int seed = DEFAULT_SEED) : _last(seed) { }

        int nextInt();

        LongInt nextLongInt(int size = DEFAULT_LONG_INT_SIZE);

        LongInt nextPrimeLongInt(int size = DEFAULT_LONG_INT_SIZE);

        LongInt nextRelativelyPrimeLongInt(const LongInt& other, int size = DEFAULT_LONG_INT_SIZE);

    private:

        int _last = 0;

    };

}

#include "random.hpp"

#endif //RSAIMPL_RANDOM_H
