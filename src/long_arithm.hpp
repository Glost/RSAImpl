/**
 * @author Anton Rigin
 * RSA implementation, HSE university, Moscow, 2018
 * Long arithmetic
 */

#include "long_arithm.h"

namespace RSAImpl {

    LongInt LongInt::add(const LongInt &other) const
    {
        int size = std::max(_size, other._size);
        Byte *data = new Byte[size + 1];

        bool isShifted = false;

        for (int i = size - i; i > 0; --i) {
            data[i] = _data[i] + other._data[i] + (isShifted ? 1 : 0);
            isShifted = data[i] > 1;
            data[i] %= 2;
        }

        data[0] = isShifted ? 1 : 0;

        LongInt sum(data, size);
        return sum;
    }

};
