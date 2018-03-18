/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * RSA encryptor
 */

#include "rsa_encryptor.h"

namespace RSAImpl {

    void RSAEncryptor::encrypt(std::istream& input, std::ostream& output) const
    {
        if (input.fail() || output.fail())
            throw std::invalid_argument("invalid input or output stream, try again");

        input.seekg(0, std::ios_base::end);
        long long inputSize = input.tellg();
        input.seekg(0, std::ios_base::beg);

        int blockSize = _size / 8 + (_size % 8 != 0 ? 1 : 0);
        int diff = (blockSize - inputSize % blockSize) % blockSize;
        int blocks = inputSize / blockSize + (diff != 0 ? 1 : 0);

        output.write((char*) diff, sizeof(int));

        for (int i = 0; i < blocks; ++i)
        {
            Byte* block = new Byte[blockSize];
            int bytes = blockSize - (i == blocks - 1 ? diff : 0);

            input.read((char*) block, bytes);

            if (i == blocks - 1 && diff != 0)
                memset(&block[blockSize - diff], 0, diff);

            LongInt blockLongInt = LongInt::fromBytes(block, 8 * blockSize);
            LongInt encryptedBlockLongInt = encrypt(blockLongInt).setSize(8 * blockSize);
            Byte* encryptedBlock = encryptedBlockLongInt.toBytes();

            output.write((char*) encryptedBlock, blockSize);
        }
    }

    void RSAEncryptor::decrypt(std::istream& input, std::ostream& output) const
    {
        if (input.fail() || output.fail())
            throw std::invalid_argument("invalid input or output stream, try again");

        input.seekg(0, std::ios_base::end);
        long long inputSize = input.tellg();
        input.seekg(0, std::ios_base::beg);

        int blockSize = _size / 8 + (_size % 8 != 0 ? 1 : 0);
        int blocks = (inputSize - sizeof(int)) / blockSize;
        int diff = 0;
        input.read((char*) &diff, sizeof(int));

        for (int i = 0; i < blocks; ++i)
        {
            Byte* block = new Byte[blockSize];
            input.read((char*) block, blockSize);

            LongInt blockLongInt = LongInt::fromBytes(block, 8 * blockSize);
            LongInt decryptedBlockLongInt = decrypt(blockLongInt).setSize(8 * blockSize);

            int bytes = blockSize - (i == blocks - 1 ? diff : 0);

            if (i == blocks - 1 && diff != 0)
                decryptedBlockLongInt = decryptedBlockLongInt.getSubLongInt(1, 8 * bytes);

            Byte* decryptedBlock = decryptedBlockLongInt.toBytes();
            output.write((char*) decryptedBlock, bytes);
        }
    }

    LongInt RSAEncryptor::encrypt(const LongInt& message) const
    {
        LongInt encrypted = message.pow(_e, _n);
        return encrypted;
    }

    LongInt RSAEncryptor::decrypt(const LongInt& message) const
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
