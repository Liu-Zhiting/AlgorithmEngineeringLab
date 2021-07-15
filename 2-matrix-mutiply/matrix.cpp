#include "utils.hpp"

MatArray::MatArray(const MatArray& other)
{
    size = other.size;
    data = new int32_t[other.size];
    memcpy(data, other.data, other.size);
}

bool MatArray::load_data_text(const char* filename)
{
    if(nullptr != data) return false;

    ifstream fin;
    fin.open(filename);
    fin >> size;
    data = new int32_t[size];
    for(int i = 0; i < size;i++)
        fin >> data[i];
    fin.close();
    return true;
}

bool MatArray::load_data_binary(const char* filename)
{
    if(nullptr != data) return false;

    ifstream fin;
    fin.open(filename);
    fin.read((char*)&size,sizeof(uint32_t));
    data = new int32_t[size];
    for(int i = 0; i < size;i++)
        fin.read((char*)&data[i],sizeof(int32_t));
    fin.close();
    return true;
}

bool MatArray::operator==(const MatArray& other) const
{
    if(size != other.size) return false;
    return (0 == memcmp(data,other.data,size));
}