#include "utils.hpp"
#include "matrix.hpp"

string strassen(const Matrix& left, const Matrix& right, Matrix& result)
{
    string func_name = __FUNCTION__;

    if(left.size != right.size)
    {
        throw std::exception();
        return func_name;
    }

    // Code here


    return func_name;
}