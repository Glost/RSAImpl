/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * RSA encryptor
 */

#include "rsa_encryptor.h"

namespace RSAImpl {

    void RSAEncryptor::init()
    {
        Random random(_seed);

        LongInt p = random.nextPrimeLongInt(_size / 2);
        LongInt q = random.nextPrimeLongInt(_size / 2);
        _n = p * q;
        LongInt oneValue = LongInt::one();
        LongInt funcEuler = (p - oneValue) * (q - oneValue);

//        _e =   // TODO: the rest.
    }

}
