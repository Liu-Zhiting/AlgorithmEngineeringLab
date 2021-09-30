#pragma once
#include "utils.hpp"

class Matrix
{
private:
    void initialize();
    void dispose();
    int32_t **data;
    uint32_t size;

public:
    Matrix() : data(nullptr), size(0){};
    Matrix(uint32_t size) : size(size) { initialize(); };
    Matrix(const Matrix &other);
    ~Matrix() { dispose(); };
    bool load_data_text(const char *filename);
    bool load_data_binary(const char *filename);
    bool operator==(const Matrix &other) const;
    Matrix &operator=(const Matrix &);
    void clear();
    void resize(uint32_t size);
    uint32_t get_size() const { return size; };
    void multiply_by_func(const Matrix &left, const Matrix &right, fun_ptr func);
    void dump() const;
};