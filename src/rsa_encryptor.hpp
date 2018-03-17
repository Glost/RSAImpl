/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * RSA encryptor
 */

#include "rsa_encryptor.h"

namespace RSAImpl {

    Byte* RSAEncryptor::encrypt(const Byte* message) const
    {
        // TODO: implement.
    }

    Byte* RSAEncryptor::decrypt(const Byte* message) const
    {
        // TODO: implement.
    }

    LongInt RSAEncryptor::encrypt(LongInt message) const
    {
        LongInt encrypted = message.pow(_e, _n);
        return encrypted;
    }

    LongInt RSAEncryptor::decrypt(LongInt message) const
    {
        LongInt decrypted = message.pow(_d, _n);
        return decrypted;
    }

    void RSAEncryptor::init()
    {
        Random random(_seed);

        LongInt p = random.nextPrimeLongInt(_size / 2);
        LongInt q = random.nextPrimeLongInt(_size / 2);
        _n = p * q;
        LongInt oneValue = LongInt::one();
        LongInt funcEuler = (p - oneValue) * (q - oneValue);

        _e = random.nextRelativelyPrimeLongInt(funcEuler, _size / 4);
        _d = _e.inv(funcEuler);
    }

}
