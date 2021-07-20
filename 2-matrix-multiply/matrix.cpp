#include "utils.hpp"
#include "matrix.hpp"

Matrix::Matrix(const Matrix& other):size(other.size),data(nullptr)
{
    initialize();
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            data[i][j] = other.data[i][j];
}

bool Matrix::operator==(const Matrix& other) const
{
    if(size != other.size) return false;
    return (0 == memcmp(data,other.data,size));
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if(this == &other) return *this;
    
    this->dispose();
    size = other.size;
    initialize();
    for(int i = 0; i < size; i++)
        memcpy(data[i], other.data[i], other.size);
    return *this;
}

void Matrix::initialize()
{
    if(nullptr != data) return;
    if(size <= 0) return;

    data=new int32_t*[size];
    for(int i = 0; i < size; i++)
    {
        data[i] = new int32_t[size];
        memset(data[i], 0, size * sizeof(int32_t));
    }
        
}

void Matrix::dispose()
{
    if(nullptr == data) return;
    for(int i = 0; i < size; i++)
        delete [] data[i];
    delete [] data;
    data = nullptr;
}

void Matrix::clear()
{
    for(int i = 0; i < size; i++)
        memset(data[i], 0, size * sizeof(int32_t));
}

void Matrix::resize(uint32_t size)
{
    this->dispose();
    this->size = size;
    this->initialize();
}

bool Matrix::load_data_text(const char* filename)
{
    if(nullptr != data) return false;

    ifstream fin;
    fin.open(filename);
    fin >> size;
    initialize();
    for(int i = 0; i < size;i++)
        for(int j = 0; j < size; j++)
            fin >> data[i][j];
    fin.close();
    return true;
}

bool Matrix::load_data_binary(const char* filename)
{
    if(nullptr != data) return false;

    ifstream fin;
    fin.open(filename);
    fin.read((char*)&size,sizeof(uint32_t));
    initialize();
    for(int i = 0; i < size;i++)
        for(int j = 0; j < size; j++)
            fin.read((char*)&data[i][j],sizeof(int32_t));
    fin.close();
    return true;
}