#ifndef SIMPLA_H
#define SIMPLA_H

#include <stddef.h>

#ifndef MAT_TYPE
#define MAT_TYPE float
#endif

typedef struct {
    MAT_TYPE* mat;
    size_t rows; // number of rows
    size_t cols; // number of columns
} simpmat;

simpmat matf_from(MAT_TYPE* array, size_t rows, size_t cols);
simpmat matf_malloc(size_t rows, size_t cols);
void matf_free(simpmat mat);
MAT_TYPE matf_index(simpmat mat, size_t row, size_t col);
void matf_add(simpmat mat1, simpmat mat2);
void matf_minus(simpmat mat1, simpmat mat2);
void matf_dot(simpmat mat1, simpmat mat2);

#endif // SIMPLA_H
