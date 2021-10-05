#pragma once
#include <cstring>
class Bitmap
{
private:
    unsigned int capacity;
    unsigned char *value;
    unsigned char *zero;
    const unsigned int __popcount_tab[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
    };

public:
    Bitmap(const unsigned int capacity) : capacity(capacity)
    {
        value = new unsigned char[(capacity / 8 + 1)];
        zero = new unsigned char[(capacity / 8 + 1)];
        memset(zero, 0, (capacity / 8 + 1) * sizeof(unsigned char));
        clear();
    }

    ~Bitmap()
    {
        delete[] value;
        delete[] zero;
    }

    bool at(unsigned int index) const
    {
        return (value[index >> 3] & (1 << index & 0x7));
    }

    void set(unsigned int index, bool state)
    {
        if (state)
            value[index / 8] |= (1 << index % 8);
        else
            value[index / 8] &= ~(1 << index % 8);
    }

    void clear() { memset(value, 0, (capacity / 8 + 1) * sizeof(unsigned char)); }

    bool is_empty() const { return (0 == memcmp(zero, value, (capacity / 8 + 1))); }

    unsigned int get_capacity() const { return capacity; }

    unsigned int get_popcount() const
    {
        unsigned int result = 0;
        for (int i = 0; i < (capacity / 8 + 1); i++)
            result += __popcount_tab[value[i]];
        return result;
    }
};