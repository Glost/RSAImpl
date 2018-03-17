/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Pseudorandom generator
 */

#include "random.h"

namespace RSAImpl {

    int Random::nextInt()
    {
        unsigned long long nextLong = A * _last + C;
        int next = nextLong % M;
        _last = next;
        return next;
    }

    LongInt Random::nextLongInt(int size)
    {
        Byte* data = new Byte[size + 1];

        for (int i = 1; i <= size; ++i)
            data[i] = ((double) nextInt()) / M >= 0.5 ? 1 : 0;

        data[0] = 0;

        LongInt next((const Byte*) data, size);
        delete[] data;
        return next;
    }

    LongInt Random::nextPrimeLongInt(int size)
    {
        LongInt next(0);

        do
            next = nextLongInt(size);
        while (!next.isProbablyPrime());

        return next;
    }

    LongInt Random::nextRelativelyPrimeLongInt(const LongInt& other, int size)
    {
        LongInt zeroValue = LongInt::zero();
        LongInt oneValue = LongInt::one();

        LongInt next(0);

        do
            next = nextPrimeLongInt(size) % other;
        while (next == zeroValue || LongInt::gcd(next, other) != oneValue);

        return next;
    }

};
