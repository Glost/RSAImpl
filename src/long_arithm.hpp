/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Long arithmetic
 */

#include "long_arithm.h"

namespace RSAImpl {

    LongInt::LongInt(long long longLong, bool needTrim)
    {
        int size = 8 * sizeof(long long);
        Byte* data = new Byte[size + 1];

        data[0] = longLong < 0 ? 1 : 0;
        if (longLong < 0)
            longLong *= -1;

        for (int i = size; i > 0; --i)
        {
            data[i] = longLong % 2;
            longLong /= 2;
        }

        if (data[1] == 0)
        {
            if (needTrim)
            {
                _isTrimmed = true;

                int trimmedSize = size;
                const Byte* trimmedData = trim(data, trimmedSize);

                if (trimmedData != data)
                    delete[] data;

                _data = trimmedData;
                _size = trimmedSize;

                return;
            }

            _isTrimmed = false;
        }
        else
            _isTrimmed = true;

        _data = data;
        _size = size;
    }

    LongInt::LongInt(const LongInt& other, bool needTrim) : _size(other._size), _isTrimmed(other._isTrimmed)
    {
        _data = new Byte[other._size + 1];
        copyBytes((Byte*) _data, (Byte*) other._data, other._size + 1);

        if (needTrim)
        {
            const Byte* trimmedData = trim(_data, _size);

            if (trimmedData != _data)
                delete[] _data;

            _data = trimmedData;
            _isTrimmed = true;
        }
    }

    LongInt::LongInt(const Byte* data, int size, bool needTrim)
    {
        if (size > 1 && data[1] == 0)
        {
            if (needTrim)
            {
                _isTrimmed = true;

                int trimmedSize = size;
                const Byte* trimmedData = trim(data, trimmedSize);

                _data = trimmedData;
                _size = trimmedSize;

                return;
            }

            _isTrimmed = false;
        }
        else
            _isTrimmed = true;

        _data = new Byte[size + 1];
        copyBytes((Byte*) _data, (Byte*) data, size + 1);
        _size = size;
    }

    LongInt::LongInt(Byte* data, int size, bool needTrim)
    {
        if (size > 1 && data[1] == 0)
        {
            if (needTrim)
            {
                _isTrimmed = true;

                int trimmedSize = size;
                const Byte* trimmedData = trim(data, trimmedSize);

                if (trimmedData != data)
                    delete[] data;

                _data = trimmedData;
                _size = trimmedSize;

                return;
            }

            _isTrimmed = false;
        }
        else
            _isTrimmed = true;

        _data = data;
        _size = size;
    }

    LongInt::~LongInt()
    {
        if (_data != nullptr)
        {
            delete[] _data;
            _data = nullptr;
        }
    }

    LongInt LongInt::add(const LongInt& other) const
    {
        int size = std::max(_size, other._size) + 1;
        Byte* data = new Byte[size + 1];

        LongInt thisNorm = setSize(size);
        LongInt otherNorm = other.setSize(size);

        bool isShifted = false;

        for (int i = size; i > 0; --i)
        {
            data[i] = thisNorm._data[i] + otherNorm._data[i] + (isShifted ? 1 : 0);
            isShifted = data[i] > 1;
            data[i] %= 2;
        }

        data[0] = 0;

        return LongInt(data, size);
    }

    LongInt LongInt::subtract(const LongInt& other) const
    {
        int size = std::max(_size, other._size);
        Byte* data = new Byte[size + 1];

        LongInt thisNorm = setSize(size);
        LongInt otherNorm = other.setSize(size);

        bool isShifted = false;

        for (int i = size; i > 0; --i)
        {
            data[i] = thisNorm._data[i] + otherNorm._data[i] + (isShifted ? 1 : 0);
            isShifted = thisNorm._data[i] < otherNorm._data[i]
                        || (isShifted && thisNorm._data[i] == otherNorm._data[i]);
            data[i] %= 2;
        }

        data[0] = 0;

        return LongInt(data, size);
    }

    LongInt LongInt::signedSubtract(const LongInt& other) const
    {
        LongInt res(0);

        if (isNegative() && other.isNegative())
            res = signedCompareTo(other) < 0 ? minus().subtract(other.minus()).minus()
                    : other.minus().subtract(minus());
        else if (isNegative() && other.isNonNegative())
            res = minus().add(other).minus();
        else if (isNonNegative() && other.isNegative())
            res = add(other.minus());
        else
            res = signedCompareTo(other) < 0 ? other.subtract(*this).minus() : subtract(other);

        return res.isNegative() && res == zero() ? res.minus() : res;
    }

    LongInt LongInt::multiply(const LongInt& other) const
    {
        LongInt zeroValue = zero();
        LongInt oneValue = one();

        if (*this == zeroValue || other == zeroValue)
            return zero();

        if (*this == oneValue)
            return isNegative() ? other.minus() : LongInt(other);

        if (other == oneValue)
            return other.isNegative() ? minus() : LongInt(*this);

        LongInt resKaratsuba = multiplyKaratsuba(*this, other);
        return resKaratsuba.isNonNegative() && (isNegative() != other.isNegative())
               ? resKaratsuba.minus() : resKaratsuba;
    }

    LongInt LongInt::divideBy(const LongInt& other) const
    {
        Byte* data = new Byte[2];
        LongInt remainder(data, 1);

        return divideBy(other, remainder);
    }

    LongInt LongInt::divideBy(const LongInt& other, LongInt& remainder) const
    {
        if (other > *this)
        {
            remainder = *this;
            return zero();
        }

        int size = std::max(_size, other._size);

        Byte* data = new Byte[size + 1];
        setToZero(data, size + 1);

        LongInt part = getSubLongInt(1, other._size);

        for (int i = other._size; i <= _size; ++i)
        {
            if (other > part)
                data[i] = 0;
            else
            {
                data[i] = 1;
                part = part - other;
            }

            if (i < _size)
                part = (part << 1) + LongInt(_data[i + 1]);
        }

        remainder = LongInt(part);

        return LongInt(data, size);
    }

    LongInt LongInt::mod(const LongInt& other) const
    {
        Byte* data = new Byte[2];
        LongInt remainder(data, 1);

        divideBy(other, remainder);
        return remainder;
    }

    LongInt LongInt::minus() const
    {
        Byte* data = new Byte[_size + 1];
        copyBytes(data, (Byte*) _data, _size + 1);
        data[0] = 1 - data[0];

        return LongInt(data, _size);
    }

    LongInt LongInt::shiftLeft(int bytes) const
    {
        int size = _size + bytes;
        Byte* data = new Byte[size + 1];

        copyBytes(&data[1], (Byte*) &_data[1], _size);
        setToZero(&data[1 + _size], bytes);

        return LongInt(data, size, false);
    }

    LongInt LongInt::pow(const LongInt& pow) const
    {
        if (!pow._isTrimmed)
            throw std::invalid_argument("pow should be trimmed");

        if (pow == zero())
            return one();

        if (pow == one())
            return LongInt(*this);

        LongInt result = (*this).sqr();

        for (int i = 2; i < pow._size; ++i)
        {
            if (pow._data[i] == 1)
                result = result * *this;

            result = result.sqr();
        }

        if (pow._data[pow._size] == 1)
            result = result * *this;

        return result;
    }

    LongInt LongInt::pow(const LongInt& pow, const LongInt& onMod) const
    {
        if (!pow._isTrimmed)
            throw std::invalid_argument("pow should be trimmed");

        if (onMod < two())
            throw std::invalid_argument("onMon should be not less than 2");

        if (pow == zero())
            return one();

        if (pow == one())
            return mod(onMod);

        LongInt result = mod(onMod).sqr().mod(onMod);

        for (int i = 2; i < pow._size; ++i)
        {
            if (pow._data[i] == 1)
                result = (result * *this).mod(onMod);

            result = result.sqr().mod(onMod);
        }

        if (pow._data[pow._size] == 1)
            result = (result * *this).mod(onMod);

        return result;
    }

    LongInt LongInt::inv(const LongInt& onMod) const
    {
        LongInt s(0);
        LongInt t(0);

        LongInt gcdRes = gcd(*this, onMod, s, t);

        if (gcdRes != one())
            return zero();

        return s.isNegative() ? onMod - s.minus() : s;
    }

    int LongInt::compareTo(const LongInt& other) const
    {
        if (_isTrimmed && other._isTrimmed)
        {
            if (_size > other._size)
                return 1;

            if (_size < other._size)
                return -1;
        }

        int size = std::max(_size, other._size);

        LongInt thisNorm = setSize(size);
        LongInt otherNorm = other.setSize(size);

        for (int i = 1; i <= size; ++i)
        {
            if (thisNorm._data[i] == otherNorm._data[i])
                continue;

            if (thisNorm._data[i] > otherNorm._data[i])
                return 1;

            return -1;
        }

        return 0;
    }

    int LongInt::signedCompareTo(const LongInt& other) const
    {
        if (_data[0] == 0 && other._data[0] == 1)
            return 1;

        if (_data[0] == 1 && other._data[0] == 0)
            return -1;

        return (1 - 2 * _data[0]) * compareTo(other);
    }

    bool LongInt::isProbablyPrime(int attemptsCount) const
    {
        if (attemptsCount < 0)
            throw std::invalid_argument("attemptsCount should be non-negative");

        LongInt zeroValue = zero();
        LongInt oneValue = one();

        if (*this == oneValue)
            return false;

        LongInt powValue = *this - one();

        int i = 0;
        for (int a = 2; i < attemptsCount; ++a)
        {
            LongInt aValue(a);

            if (aValue.mod(*this) == zeroValue)
                continue;

            if (aValue.pow(powValue, *this) != oneValue)
                return false;

            ++i;
        }

        return true;
    }

    LongInt LongInt::setSize(int size) const
    {
        if (size == _size)
            return LongInt(*this, false);

        Byte* data = new Byte[size + 1];

        copyBytes(&data[size > _size ? size - _size + 1 : 1],
                  (Byte*) &_data[size >= _size ? 1 : _size - size + 1],
                  size >= _size ? _size : size);
        setToZero(&data[0], size > _size ? size - _size + 1 : 1);

        return LongInt(data, size, false);;
    }

    LongInt LongInt::getSubLongInt(int begin, int size) const
    {
        if (begin < 1 || begin + size > _size + 1)
            throw std::invalid_argument("begin or size are not in borders of LongInt");

        Byte* data = new Byte[size + 1];

        copyBytes(&data[1], (Byte*)&_data[begin], size);
        data[0] = 0;

        return LongInt(data, size, false);
    }

    long long LongInt::toLongLong() const
    {
        long long longLongValue = 0;

        for (int i = _size; i > 0; --i)
            longLongValue = (_data[i] << (_size - i)) + longLongValue;

        if (_data[0] == 1)
            longLongValue *= -1;

        return longLongValue;
    }

    char LongInt::toChar() const
    {
        char charValue = 0;

        for (int i = _size; i > 0; --i)
            charValue = (_data[i] << (_size - i)) + charValue;

        if (_data[0] == 1)
            charValue *= -1;

        return charValue;
    }

    void LongInt::operator=(const LongInt& other)
    {
        if (_data != nullptr)
            delete[] _data;

        _data = new Byte[other._size + 1];
        copyBytes((Byte*) _data, (Byte*) other._data, other._size + 1);

        _size = other._size;
        _isTrimmed = other._isTrimmed;
    }

    LongInt LongInt::gcd(const LongInt& a, const LongInt& b)
    {
        LongInt s(0);
        LongInt t(0);

        return gcd(a, b, s, t);
    }

    LongInt LongInt::gcd(const LongInt& a, const LongInt& b, LongInt& s, LongInt& t)
    {
        if (a == zero())
        {
            s = zero();
            t = one();

            return b;
        }

        LongInt sPrev(0);
        LongInt tPrev(0);

        LongInt gcdRes = gcd(b % a, a, sPrev, tPrev);

        s = tPrev.signedSubtract((b / a) * sPrev);
        t = sPrev;

        return gcdRes;
    }

    LongInt LongInt::multiplyKaratsuba(const LongInt& a, const LongInt& b)
    {
        int size = a._size + b._size;

        if (size < 8 * sizeof(char) - 1)
        {
            char charA = a.toChar();
            char charB = b.toChar();

            char charProd = charA * charB;

            return LongInt(charProd);
        }

        int n = std::max(a._size, b._size);
        LongInt normA = a.setSize(n);
        LongInt normB = b.setSize(n);

        int m = n / 2;
        int mCeil = n % 2 == 0 ? m : m + 1;

        LongInt a1 = normA.getSubLongInt(1, mCeil);
        LongInt b1 = normB.getSubLongInt(1, mCeil);
        LongInt a0 = normA.getSubLongInt(mCeil + 1, m);
        LongInt b0 = normB.getSubLongInt(mCeil + 1, m);

        LongInt prod00 = multiplyKaratsuba(a0, b0);
        LongInt prod11 = multiplyKaratsuba(a1, b1);

        LongInt sumA = a0 + a1;
        LongInt sumB = b0 + b1;

        LongInt sumProd = multiplyKaratsuba(sumA, sumB);

        LongInt prod = prod00 + ((sumProd - prod00 - prod11) << m) + (prod11 << (2 * m));
        return prod;
    }

    const Byte* LongInt::trim(const Byte* data, int& size)
    {
        int trimmedSize = size;

        for(int i = 1; i < size; ++i)
        {
            if (data[i] == 0)
                --trimmedSize;
            else
                break;
        }

        if (trimmedSize != size)
        {
            Byte* trimmedData = new Byte[trimmedSize + 1];
            copyBytes(&trimmedData[1], (Byte*) &data[size - trimmedSize + 1], trimmedSize);
            trimmedData[0] = data[0];
            size = trimmedSize;

            return trimmedData;
        }

        return data;
    }

};
