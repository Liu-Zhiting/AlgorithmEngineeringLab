#include "utils.hpp"
#include "matrix.hpp"

void ref(const Matrix& left, const Matrix& right, Matrix& result)
{
    parallel_for (int i = 0; i < left.size; i++)
        parallel_for (int k = 0; k < result.size; k++)
            for(int j = 0; j < right.size; j++)
                result.data[i][j] += left.data[i][k] * right.data[k][j];
}