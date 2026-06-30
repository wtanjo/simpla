#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simpla.h"

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
        .stride = cols,
    };
}

sm mat_malloc(size_t rows, size_t cols) {
    MAT_TYPE* array = (MAT_TYPE*)aligned_alloc(64, sizeof(MAT_TYPE) * rows * cols);
    assert(array != NULL);
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
        .stride = cols,
    };
}

void mat_free(sm mat) {
    free(mat.p);
    return;
}

void mat_print(sm mat, const char* name) {
    MAT_TYPE* mp = mat.p;
    printf("%s = [\n", name);
    for (size_t i = 0; i < mat.rows; i++) {
        MAT_TYPE* mrp = mp + i * mat.stride;
        for (size_t j = 0; j < mat.cols; j++) {
            printf("%12.4f", mrp[j]);
        }
        printf("\n");
    }
    printf("]\n");
    return;
}

void mat_fill(sm mat, MAT_TYPE e) {
    if (mat.cols == mat.stride) {
        memset(mat.p, e, sizeof(MAT_TYPE) * mat.rows * mat.cols);
        return;
    } else {
        MAT_TYPE* mp = mat.p;
        for (size_t i = 0; i < mat.rows; i++) {
            MAT_TYPE* mrp = mp + i * mat.stride;
            for (size_t j = 0; j < mat.cols; j++) {
                memset(mrp, e, sizeof(MAT_TYPE) * mat.cols);
            }
        }
    }
    return;
}

void mat_srand(uint64_t seed) {
    rng_state = seed;
    return;
}

// randomize mat with (pseudo) random values between lower bound l and upper bound u
void mat_rand(sm mat, MAT_TYPE l, MAT_TYPE u) {
    assert(u > l);
    MAT_TYPE* mp = mat.p;
    
    for (size_t i = 0; i < mat.rows; i++) {
        MAT_TYPE* mrp = mp + i * mat.stride;
        for (size_t j = 0; j < mat.cols; j++) {
            mrp[j] = (MAT_TYPE)xorshift64() / (MAT_TYPE)UINT64_MAX * (u - l) + l;
        }
    }
    return;
}

void mat_eye(sm mat) {
    assert(mat.rows == mat.cols);
    mat_fill(mat, 0);
    MAT_TYPE* mp = mat.p;

    for (size_t i = 0; i < (mat.rows - 1) * mat.stride + mat.cols; i += mat.stride + 1) {
        mp[i] = 1;
    }
    return;
}

void mat_add(sm dst,sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict m1rp;
    const MAT_TYPE* restrict m2rp;

    for (size_t i = 0; i < dst.rows; i++) {
        drp = dp + i * dst.stride;
        m1rp = m1p + i * mat1.stride;
        m2rp = m2p + i * mat2.stride;
        for (size_t j = 0; j < dst.cols; j++) {
            drp[j] = m1rp[j] + m2rp[j];
        }
    }
    return;
}

void mat_minus(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict m1rp;
    const MAT_TYPE* restrict m2rp;

    for (size_t i = 0; i < dst.rows; i++) {
        drp = dp + i * dst.stride;
        m1rp = m1p + i * mat1.stride;
        m2rp = m2p + i * mat2.stride;
        for (size_t j = 0; j < dst.cols; j++) {
            drp[j] = m1rp[j] - m2rp[j];
        }
    }
    return;
}

MAT_TYPE vec_dot(sm mat1, sm mat2) {
    assert(mat1.rows == 1 && mat2.cols == 1 && mat1.cols == mat2.rows);
    MAT_TYPE prod = 0;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    
    for (size_t i = 0; i < mat1.cols; i++) {
        prod += m1p[i] * m2p[i];
    }
    return prod;
}

// plain matrix multiplication with moderate acceleration with openmp parallel
// try to avoid using macros in time-consuming part
void mat_dot(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat2.cols && mat1.cols == mat2.rows);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    memset(dp, 0, sizeof(MAT_TYPE) * dst.rows * dst.cols);

    #pragma omp parallel for
    for (size_t i = 0; i < dst.rows; i++) {
        MAT_TYPE* drp = dp + i * dst.stride;
        for (size_t k = 0; k < mat1.cols; k++) {
            const MAT_TYPE t = m1p[k + mat1.stride * i];
            const MAT_TYPE* m2rp = m2p + k * mat2.stride;
            #pragma omp simd
            for (size_t j = 0; j < dst.cols; j++) {
                drp[j] += t * m2rp[j];
            }
        }
    }
    return;
}

// large-scale matrix multiplication accelerated with blocking
#define BLK (64)

void mat_dot_blocked(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat2.cols && mat1.cols == mat2.rows);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    size_t M = dst.rows;
    size_t N = dst.cols;
    size_t K = mat1.cols;
    memset(dp, 0, sizeof(MAT_TYPE) * dst.rows * dst.cols);

    #pragma omp parallel for collapse(2)
    for (size_t rb = 0; rb < M; rb += BLK) {
        for (size_t cb = 0; cb < N; cb += BLK) {
            for (size_t kb = 0; kb < K; kb += BLK) {
                // i-k-j loop for this block
                size_t i_max = (rb + BLK > M) ? M : (rb + BLK);
                size_t k_max = (kb + BLK > K) ? K : (kb + BLK);
                size_t j_max = (cb + BLK > N) ? N : (cb + BLK);
                
                for (size_t i = rb; i < i_max; i++) {
                    MAT_TYPE* drp = dp + i * dst.stride;
                    for (size_t k = kb; k < k_max; k++) {
                        const MAT_TYPE t = m1p[k + mat1.stride * i];
                        const MAT_TYPE* m2rp = m2p + k * mat2.stride;
                        #pragma omp simd
                        for (size_t j = cb; j < j_max; j++) {
                            drp[j] += t * m2rp[j];
                        }
                    }
                }
            }
        }
    }
    return;
}

void mat_assign(sm dst, sm src) {
    assert(dst.rows == src.rows && dst.cols == src.cols);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict sp = src.p;
    
    for (size_t i = 0; i < dst.rows; i++) {
        MAT_TYPE* restrict drp = dp + i * dst.stride;
        const MAT_TYPE* restrict srp = sp + i * src.stride;
        for (size_t j = 0; j < dst.cols; j++) {
            drp[j] = srp[j];
        }
    }
    return;
}
