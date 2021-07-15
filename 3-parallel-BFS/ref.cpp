#include "utils.hpp"

string ref(const MatArray& left, const MatArray& right, MatArray& result)
{
    string func_name = __FUNCTION__;

    if(left.size != right.size)
    {
        throw std::exception();
        return func_name;
    }

    parallel_for (int i = 0; i < left.size; i++)
        parallel_for (int k = 0; k < result.size; k++)
            for(int j = 0; j < right.size; j++)
            {
                result.data[i * result.size + j] 
                    += left.data[i * left.size + k] + right.data[k * right.size + j];
            }

    return func_name;
}