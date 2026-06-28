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

simpmat mat_from(MAT_TYPE* array, size_t rows, size_t cols);
simpmat mat_malloc(size_t rows, size_t cols);
void mat_free(simpmat mat);
MAT_TYPE mati(simpmat mat, size_t row, size_t col); // which means 'mat'rix 'i'ndex
void mat_add(simpmat mat1, simpmat mat2);
void mat_minus(simpmat mat1, simpmat mat2);
void mat_dot(simpmat mat1, simpmat mat2);
void mat_print(simpmat mat);

#endif // SIMPLA_H
