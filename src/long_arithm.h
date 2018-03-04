/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Long arithmetic
 */

#ifndef RSAIMPL_LONG_ARITHM_H
#define RSAIMPL_LONG_ARITHM_H

#include <vector>
#include <algorithm>

namespace RSAImpl {

    using Byte = unsigned char;

    class LongInt {

    public:

        LongInt(const Byte* data, const int size) : _data(data), _size(size) { }

    public:

        LongInt add(const LongInt& other) const;

    public:

        static LongInt add(const LongInt& a, const LongInt& b) { return a.add(b); }

    public:

        static LongInt operator+(const LongInt& a, const LongInt& b) { return a.add(b); }

        LongInt operator+=(const LongInt& other) { return add(other); }

    private:

        const Byte* _data = nullptr;

        const int _size;

    };

}

#include "long_arithm.hpp"

#endif //RSAIMPL_LONG_ARITHM_H
