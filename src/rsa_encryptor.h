/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * RSA encryptor
 */

#ifndef RSAIMPL_RSA_ENCRYPTOR_H
#define RSAIMPL_RSA_ENCRYPTOR_H

#include <iostream>

#include "long_arithm.h"
#include "random.h"

namespace RSAImpl {

    class RSAEncryptor {

    public:

        RSAEncryptor(int seed = DEFAULT_SEED,
                int size = 2 * DEFAULT_LONG_INT_SIZE,
                int testForPrimeAttemptsCount = TEST_FOR_PRIME_ATTEMPTS_COUNT)
                : _size(size), _seed(seed), _testForPrimeAttemptsCount(testForPrimeAttemptsCount) { init(); }

        void encrypt(std::istream& input, std::ostream& output) const;

        void decrypt(std::istream& input, std::ostream& output) const;

        LongInt encrypt(const LongInt& message) const;

        LongInt decrypt(const LongInt& message) const;

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
