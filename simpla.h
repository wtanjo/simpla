#ifndef SIMPLA_H_
#define SIMPLA_H_

#include <stddef.h>

#ifndef MAT_TYPE
#define MAT_TYPE float
#endif // MAT_TYPE

typedef struct {
    MAT_TYPE* p;
    size_t rows; // number of rows
    size_t cols; // number of columns
} sm;

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols);
sm mat_malloc(size_t rows, size_t cols);
void mat_free(sm mat);
void mat_print(sm mat);
void mat_fill(sm mat, MAT_TYPE e);
void mat_rand(sm mat, MAT_TYPE l, MAT_TYPE u);
void mat_eye(sm mat);
void mat_add(sm dst, sm mat1, sm mat2);
void mat_minus(sm dst, sm mat1, sm mat2);
void mat_dot(sm dst, sm mat1, sm mat2);

#endif // SIMPLA_H_
