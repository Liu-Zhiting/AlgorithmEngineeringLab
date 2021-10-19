#include "utils.hpp"

void sta_mat_sub(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            c[i][j] = a[i][j] - b[i][j];
}

void sta_mat_add(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size)
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            c[i][j] = a[i][j] + b[i][j];
}

int32_t **sta_new_mat(const uint32_t size)
{
    int32_t **mat = new int32_t *[size];
    for (int i = 0; i < size; i++)
    {
        mat[i] = new int32_t[size];
        memset(mat[i], 0, size * sizeof(int32_t));
    }
    return mat;
}

int32_t **sta_delete_mat(int32_t **mat, const uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        delete[] mat[i];
    }
    delete[] mat;
    return nullptr;
}

void strassen(
    const int32_t *const *const a,
    const int32_t *const *const b,
    int32_t *const *const c,
    const uint32_t size)
{
    

    // end recursion
    if (size <= 64)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                for (int k = 0; k < size; k++)
                    c[i][j] += a[i][k] * b[k][j];

        return;
    }

    // init
    int n = size >> 1;
    const int32_t **split_a[2][2], **split_b[2][2];
    int32_t **split_c[2][2];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            split_a[i][j] = new const int32_t *[n];
            split_b[i][j] = new const int32_t *[n];
            split_c[i][j] = new int32_t *[n];
            for (int k = 0; k < n; k++)
            {
                split_a[i][j][k] = a[i * n + k] + j * n;
                split_b[i][j][k] = b[i * n + k] + j * n;
                split_c[i][j][k] = c[i * n + k] + j * n;
            }
        }
    }

    int32_t **m[7];
    for (int i = 0; i < 7; i++)
    {
        m[i] = sta_new_mat(n);
    }
    int32_t **s[10];
    for (int i = 0; i < 10; i++)
    {
        s[i] = sta_new_mat(n);
    }
    int32_t **tmp[2];
    tmp[0] = sta_new_mat(n);
    tmp[1] = sta_new_mat(n);

    // s1 = a11 + a22
    sta_mat_add(split_a[0][0], split_a[1][1], s[0], n);
    // s2 = b11 + b22
    sta_mat_add(split_b[0][0], split_b[1][1], s[1], n);
    // s3 = a21 + a22
    sta_mat_add(split_a[1][0], split_a[1][1], s[2], n);
    // s4 = b12 - b22
    sta_mat_sub(split_b[0][1], split_b[1][1], s[3], n);
    // s5 = b21 - b11
    sta_mat_sub(split_b[1][0], split_b[0][0], s[4], n);
    // s6 = a11 + a12
    sta_mat_add(split_a[0][0], split_a[0][1], s[5], n);
    // s7 = a21 - a11
    sta_mat_sub(split_a[1][0], split_a[0][0], s[6], n);
    // s8 = b11 + b12
    sta_mat_add(split_b[0][0], split_b[0][1], s[7], n);
    // s9 = a12 - a22
    sta_mat_sub(split_a[0][1], split_a[1][1], s[8], n);
    // s10 = b21 + b22
    sta_mat_add(split_b[1][0], split_b[1][1], s[9], n);

    // m1 = s1 * s2
    strassen(s[0], s[1], m[0], n);
    // m2 = s3 * b11
    strassen(s[2], split_b[0][0], m[1], n);
    // m3 = a11 * s4
    strassen(split_a[0][0], s[3], m[2], n);
    // m4 = a22 * s5
    strassen(split_a[1][1], s[4], m[3], n);
    // m5 = s6 * b22
    strassen(s[5], split_b[1][1], m[4], n);
    // m6 = s7 * s8
    strassen(s[6], s[7], m[5], n);
    // m7 = s9 * s10
    strassen(s[8], s[9], m[6], n);

    // c11 = m1 + m4 - m5 + m7
    sta_mat_add(m[0], m[3], tmp[0], n);
    sta_mat_sub(m[6], m[4], tmp[1], n);
    sta_mat_add(tmp[0], tmp[1], split_c[0][0], n);

    // c12 = m3 + m5
    sta_mat_add(m[2], m[4], split_c[0][1], n);

    // c21 = m2 + m4
    sta_mat_add(m[1], m[3], split_c[1][0], n);

    // c22 = m1 - m2 + m3 + m6
    sta_mat_sub(m[0], m[1], tmp[0], n);
    sta_mat_add(m[2], m[5], tmp[1], n);
    sta_mat_add(tmp[0], tmp[1], split_c[1][1], n);

    // dispose
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            delete[] split_a[i][j];
            delete[] split_b[i][j];
            delete[] split_c[i][j];
        }
    }
    for (int i = 0; i < 7; i++)
    {
        m[i] = sta_delete_mat(m[i], n);
    }
    for (int i = 0; i < 10; i++)
    {
        s[i] = sta_delete_mat(s[i], n);
    }
    tmp[0] = sta_delete_mat(tmp[0], n);
    tmp[1] = sta_delete_mat(tmp[1], n);
}