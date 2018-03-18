/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * The main() method file.
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "long_arithm.h"
#include "random.h"
#include "rsa_encryptor.h"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4)
    {
        std::cerr << "The number of command-line should be not less from 2 to 4" << std::endl;
        return -1;
    }

    if (std::strcmp(argv[1], "-d") == 0)
    {
        if (argc < 3)
        {
            std::cerr << "The number of command-line for decrypting should be from 3 to 4" << std::endl;
            return -1;
        }

        std::string inputFileName(argv[2]);
        std::ifstream inputFile(inputFileName, std::ios::binary);

        if (!inputFile.is_open())
        {
            std::cerr << "Cannot open the input file for reading" << std::endl;
            return -1;
        }

        std::string outputFileName = inputFileName + std::string(".rsadecr");
        std::ofstream outputFile(outputFileName, std::ios::binary);

        if (!outputFile.is_open())
        {
            std::cerr << "Cannot open the output file for writing" << std::endl;
            return -1;
        }

        int seed = 0;

        if (argc == 4)
        {
            seed = std::strtol(argv[3], nullptr, 10);
            if (seed == 0)
                std::cerr << "Incorrect seed value, trying to use default seed value" << std::endl;
        }

        RSAImpl::RSAEncryptor rsaEncryptor = seed != 0 ? RSAImpl::RSAEncryptor(seed) : RSAImpl::RSAEncryptor();

        rsaEncryptor.decrypt(inputFile, outputFile);

        inputFile.close();
        outputFile.close();
    }
    else
    {
        if (argc < 2 || argc > 3)
        {
            std::cerr << "The number of command-line for decrypting should be from 2 to 3" << std::endl;
            return -1;
        }

        std::string inputFileName(argv[1]);
        std::ifstream inputFile(inputFileName, std::ios::binary);

        if (!inputFile.is_open())
        {
            std::cerr << "Cannot open the input file for reading" << std::endl;
            return -1;
        }

        std::string outputFileName = inputFileName + std::string(".rsa");
        std::ofstream outputFile(outputFileName, std::ios::binary);

        if (!outputFile.is_open())
        {
            std::cerr << "Cannot open the output file for writing" << std::endl;
            return -1;
        }

        int seed = 0;

        if (argc == 3)
        {
            seed = std::strtol(argv[2], nullptr, 10);
            if (seed == 0)
                std::cerr << "Incorrect seed value, trying to use default seed value" << std::endl;
        }

        RSAImpl::RSAEncryptor rsaEncryptor = seed != 0 ? RSAImpl::RSAEncryptor(seed) : RSAImpl::RSAEncryptor();

        rsaEncryptor.encrypt(inputFile, outputFile);

        inputFile.close();
        outputFile.close();
    }

    return 0;
}
