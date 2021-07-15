#include "utils.hpp"
#include <cstdlib>


int cmp_int32(const void * a,const void * b)
{
    return *((int32_t*)a) - *((int32_t*)b);
}

string ref(SortArray& result)
{
    string func_name = __FUNCTION__;

    qsort(result.data,result.size,sizeof(int32_t),cmp_int32);
    
    return func_name;
}