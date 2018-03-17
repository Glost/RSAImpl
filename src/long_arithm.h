/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Long arithmetic
 */

#ifndef RSAIMPL_LONG_ARITHM_H
#define RSAIMPL_LONG_ARITHM_H

#include <cstring>
#include <string>
#include <stdexcept>
#include <algorithm>

namespace RSAImpl {

    typedef unsigned char Byte;

    static const int TEST_FOR_PRIME_ATTEMPTS_COUNT = 30;

    class LongInt {

    public:

        LongInt(long long longLong, bool needTrim = true);

        LongInt(const LongInt& other, bool needTrim = true);

        LongInt(const Byte* data, int size, bool needTrim = true);

    protected:

        LongInt(Byte* data, int size, bool needTrim = true);

    public:

        ~LongInt();

    public:

        LongInt add(const LongInt& other) const;

        LongInt subtract(const LongInt& other) const;

        LongInt signedSubtract(const LongInt& other) const;

        LongInt multiply(const LongInt& other) const;

        LongInt divideBy(const LongInt& other) const;

        LongInt divideBy(const LongInt& other, LongInt& remainder) const;

        LongInt mod(const LongInt& other) const;

        LongInt minus() const;

        bool isNonNegative() const { return _data[0] == 0; }

        bool isNegative() const { return _data[0] == 1; }

        LongInt shiftLeft(int bytes) const;

        LongInt sqr() const { return multiply(*this); }

        LongInt pow(const LongInt& pow) const;

        LongInt pow(const LongInt& pow, const LongInt& onMod) const;

        LongInt inv(const LongInt& onMod) const;

        int compareTo(const LongInt& other) const;

        int signedCompareTo(const LongInt& other) const;

        bool isProbablyPrime(int attemptsCount = TEST_FOR_PRIME_ATTEMPTS_COUNT) const;

        LongInt setSize(int size) const;

        LongInt getSubLongInt(int begin, int n) const;

        long long toLongLong() const;

        char toChar() const;

    public:

        void operator=(const LongInt& other);

    public:

        static LongInt add(const LongInt& a, const LongInt& b) { return a.add(b); }

        static LongInt subtract(const LongInt& a, const LongInt& b) { return a.subtract(b); }

        static LongInt signedSubtract(const LongInt& a, const LongInt& b) { return a.signedSubtract(b); }

        static LongInt multiply(const LongInt& a, const LongInt& b) { return a.multiply(b); }

        static LongInt divide(const LongInt& a, const LongInt& b) { return a.divideBy(b); }

        static LongInt divide(const LongInt& a, const LongInt& b, LongInt& mod) { return a.divideBy(b, mod); }

        static LongInt mod(const LongInt& a, const LongInt& b) { return a.mod(b); }

        static LongInt gcd(const LongInt& a, const LongInt& b);

        static LongInt gcd(const LongInt& a, const LongInt& b, LongInt& s, LongInt& t);

        static LongInt zero() { return LongInt(0); }

        static LongInt one() { return LongInt(1); }

        static LongInt two() { return LongInt(2); }

        static LongInt ten() { return LongInt(10); }

    protected:

        static LongInt multiplyKaratsuba(const LongInt& a, const LongInt& b);

        static const Byte* trim(const Byte* data, int& size);

        static inline void copyBytes(Byte* dst, Byte* src, int size) { memcpy(dst, src, size); }

        static inline void setToZero(Byte* data, int size) { memset(data, 0, size); }

    private:

        const Byte* _data = nullptr;

        int _size;

        bool _isTrimmed = false;

    };

    LongInt operator+(const LongInt& a, const LongInt& b) { return a.add(b); }

    LongInt operator-(const LongInt& a, const LongInt& b) { return a.subtract(b); }

    LongInt operator*(const LongInt& a, const LongInt& b) { return a.multiply(b); }

    LongInt operator/(const LongInt& a, const LongInt& b) { return a.divideBy(b); }

    LongInt operator%(const LongInt& a, const LongInt& b) { return a.mod(b); }

    LongInt operator<<(const LongInt& longInt, int bytes) { return longInt.shiftLeft(bytes); }

    bool operator>(const LongInt& a, const LongInt& b) { return a.compareTo(b) > 0; }

    bool operator>=(const LongInt& a, const LongInt& b) { return a.compareTo(b) >= 0; }

    bool operator<(const LongInt& a, const LongInt& b) { return a.compareTo(b) < 0; }

    bool operator<=(const LongInt& a, const LongInt& b) { return a.compareTo(b) <= 0; }

    bool operator==(const LongInt& a, const LongInt& b) { return a.compareTo(b) == 0; }

    bool operator!=(const LongInt& a, const LongInt& b) { return a.compareTo(b) != 0; }

}

#include "long_arithm.hpp"

#endif //RSAIMPL_LONG_ARITHM_H
