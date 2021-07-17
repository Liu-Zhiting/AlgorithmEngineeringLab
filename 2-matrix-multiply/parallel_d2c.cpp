#include "utils.hpp"
#include "matrix.hpp"

string parallel_d2c(const MatArray& left, const MatArray& right, MatArray& result)
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