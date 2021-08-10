#pragma once
#include "utils.hpp"

class Array
{
private:
    void dispose();
    int32_t *data;
    uint32_t size;
public:
    Array() : data(nullptr), size(0){};
    Array(uint32_t size) : size(size) { data = new int32_t[size]; };
    Array(const Array &other);
    ~Array() { dispose(); };
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    bool operator==(const Array &other) const;
    Array &operator=(const Array &other);
    void sort_with_func(fun_ptr func) { func(data, size); };
    uint32_t get_size() const { return size;};
};