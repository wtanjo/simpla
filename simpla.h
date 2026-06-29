#ifndef SIMPLA_H_
#define SIMPLA_H_

#include <stddef.h>
#include <stdint.h>

#ifndef MAT_TYPE
#define MAT_TYPE float
#endif // MAT_TYPE

static uint64_t rng_state = 0x9e3779b97f4a7c15ULL;
static inline uint64_t xorshift64(void) {
    uint64_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state = x;
    return rng_state;
}

typedef struct {
    MAT_TYPE* p;
    size_t rows; // number of rows
    size_t cols; // number of columns
    size_t stride; // specifically for slicing
} sm;

// MATI means 'mat'rix 'i'ndex
#define MATI(mat, row, col) ((mat).p[(col) + (mat).stride * (row)])
#define SLICE(mat, rowb, rowe, colb, cole) ((sm){.p = &(MATI((mat), (rowb), (colb))), \
                                                 .rows = (rowe) - (rowb) + 1, \
                                                 .cols = (cole) - (colb) + 1, \
                                                 .stride = (mat).cols})
#define mat_fill(mat, e) memset((mat).p, (e), sizeof(MAT_TYPE) * (mat).rows * (mat).cols)

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols);
sm mat_malloc(size_t rows, size_t cols);
void mat_free(sm mat);
void mat_print(sm mat);
void mat_rand(sm mat, MAT_TYPE l, MAT_TYPE u);
void mat_eye(sm mat);
void mat_add(sm dst, sm mat1, sm mat2);
void mat_minus(sm dst, sm mat1, sm mat2);
MAT_TYPE vec_dot(sm mat1, sm mat2);
void mat_dot(sm dst, sm mat1, sm mat2);
void mat_assign(sm dst, sm src);

#endif // SIMPLA_H_
