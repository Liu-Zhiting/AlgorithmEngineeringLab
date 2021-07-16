#include "utils.hpp"

void SortArray::dispose()
{
    if (nullptr == data)
        return;
    delete[] data;
    data = nullptr;
}

SortArray::SortArray(const SortArray &other)
{
    size = other.size;
    data = new int32_t[other.size];
    memcpy(data, other.data, other.size);
}

bool SortArray::operator==(const SortArray &other) const
{
    if (size != other.size)
        return false;
    return (0 == memcmp(data, other.data, size));
}

SortArray &SortArray::operator=(const SortArray &other)
{
    if (this == &other)
        return *this;
    dispose();
    size = other.size;
    data = new int32_t[size];
    memcpy(data, other.data, other.size*sizeof(int32_t));
    return *this;
}

bool SortArray::load_data_text(const char *filename)
{
    if (nullptr != data)
        return false;

    ifstream fin;
    fin.open(filename);
    fin >> size;
    data = new int32_t[size];
    for (int i = 0; i < size; i++)
        fin >> data[i];
    fin.close();
    return true;
}

bool SortArray::load_data_binary(const char *filename)
{
    if (nullptr != data)
        return false;

    ifstream fin;
    fin.open(filename);
    fin.read((char *)&size, sizeof(uint32_t));
    data = new int32_t[size];
    for (int i = 0; i < size; i++)
        fin.read((char *)&data[i], sizeof(int32_t));
    fin.close();
    return true;
}
