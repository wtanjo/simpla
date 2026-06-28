#ifndef SIMPLA_H
#define SIMPLA_H

#include <stddef.h>

typedef struct {
    float* mat;
    size_t rows; // number of rows
    size_t cols; // number of columns
} smf;

smf matf_from(float* array, size_t rows, size_t cols);
smf matf_malloc(size_t rows, size_t cols);
void matf_free(smf mat);
float matf_index(smf mat, size_t row, size_t col);
void matf_add(smf mat1, smf mat2);
void matf_minus(smf mat1, smf mat2);
void matf_dot(smf mat1, smf mat2);

#endif // SIMPLA_H
