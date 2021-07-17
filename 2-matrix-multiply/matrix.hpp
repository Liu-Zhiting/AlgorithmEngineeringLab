#pragma once
#include "utils.hpp"

class MatArray
{
private:
    void initialize();
    void dispose();

public:
    int32_t** data;
    uint32_t size;

    MatArray():data(nullptr),size(0){};
    MatArray(uint32_t size):size(size){initialize();};
    MatArray(const MatArray& other);
    ~MatArray(){dispose();};
    bool load_data_text(const char* filename);
    bool load_data_binary(const char* filename);
    bool operator==(const MatArray& other) const;
    MatArray& operator=(const MatArray&);
    void clear();
    void resize(uint32_t size);

};