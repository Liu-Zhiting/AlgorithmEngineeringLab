#pragma once
#include <cstring>
class Bitmap
{
private:
    unsigned int capacity;
    char *value;

public:
    Bitmap(const unsigned int capacity) : capacity(capacity)
    {
        value = new char[(capacity / 8 + 1)];
        clear();
    }

    ~Bitmap() { delete[] value; }

    bool at(unsigned int index) const
    {
        return (bool)(value[index / 8] & (1 << index % 8));
    }

    void set(unsigned int index, bool state)
    {
        if (state)
            value[index / 8] |= (1 << index % 8);
        else
            value[index / 8] &= ~(1 << index % 8);
    }

    void clear() { memset(value, 0, (capacity / 8 + 1) * sizeof(char)); }

    bool is_empty() const
    {
        for (int i = 0; i < (capacity / 8 + 1); i++)
        {
            if (value[i] != 0)
                return false;
        }
        return true;
    }

    unsigned int get_capacity() const { return capacity; }

    unsigned int get_num_of_1() const
    {
        auto count_1 = [](char byte) -> unsigned int
        {
            unsigned int result = 0;
            for (int i = 0; i < 8; i++)
            {
                result += byte & 1;
                byte >>= 1;
            }
            return result;
        };
        unsigned int result = 0;
        for (int i = 0; i < (capacity / 8 + 1); i++)
            result += count_1(value[i]);
        return result;
    }
};