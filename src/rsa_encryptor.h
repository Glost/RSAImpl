/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * RSA encryptor
 */

#ifndef RSAIMPL_RSA_ENCRYPTOR_H
#define RSAIMPL_RSA_ENCRYPTOR_H

#include "long_arithm.h"
#include "random.h"

namespace RSAImpl {

    class RSAEncryptor {

    public:

        RSAEncryptor(int size = 2 * DEFAULT_LONG_INT_SIZE,
                int seed = DEFAULT_SEED,
                int testForPrimeAttemptsCount = TEST_FOR_PRIME_ATTEMPTS_COUNT)
                : _size(size), _seed(seed), _testForPrimeAttemptsCount(testForPrimeAttemptsCount) { init(); }

        Byte* encrypt(const Byte* message) const;

        Byte* decrypt(const Byte* message) const;

        LongInt encrypt(LongInt message) const;

        LongInt decrypt(LongInt message) const;

    private:

        void init();

    private:

        LongInt _n;

        LongInt _d;

        LongInt _e;

        int _seed = 0;

        int _size = 0;

        int _testForPrimeAttemptsCount = 0;

    };

}

#include "rsa_encryptor.hpp"

#endif //RSAIMPL_RSA_ENCRYPTOR_H
