#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simpla.h"

#define BLK (128)

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
        .stride = cols,
    };
}

sm mat_alloc(size_t rows, size_t cols) {
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
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    const size_t S = mat.stride;
    MAT_TYPE* mp = mat.p;
    
    printf("%s = [\n", name);
    for (size_t i = 0; i < M; i++) {
        MAT_TYPE* mrp = mp + i * S;
        for (size_t j = 0; j < N; j++) {
            printf("%12.4f", mrp[j]);
        }
        printf("\n");
    }
    printf("]\n");
    return;
}

void mat_clear(sm mat) {
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    const size_t S = mat.stride;
    MAT_TYPE* mp = mat.p;
    
    if (N == S) {
        memset(mp, 0, sizeof(MAT_TYPE) * M * N);
        return;
    } else {
        for (size_t i = 0; i < M; i++) {
            MAT_TYPE* mrp = mp + i * S;
            for (size_t j = 0; j < N; j++) {
                memset(mrp, 0, sizeof(MAT_TYPE) * N);
            }
        }
    }
    return;
}

void mat_fill(sm mat, MAT_TYPE e) {
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    const size_t S = mat.stride;
    MAT_TYPE* mp = mat.p;

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        MAT_TYPE* mrp = mp + i * S;
        for (size_t j = 0; j < N; j++) {
            mrp[j] = e;
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
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    const size_t S = mat.stride;
    MAT_TYPE* mp = mat.p;
    
    for (size_t i = 0; i < M; i++) {
        MAT_TYPE* mrp = mp + i * S;
        for (size_t j = 0; j < N; j++) {
            mrp[j] = (MAT_TYPE)xorshift64() / (MAT_TYPE)UINT64_MAX * (u - l) + l;
        }
    }
    return;
}

void mat_eye(sm mat) {
    assert(mat.rows == mat.cols);
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    const size_t S = mat.stride;
    MAT_TYPE* mp = mat.p;
    mat_clear(mat);

    #pragma omp parallel for
    for (size_t i = 0; i < (M - 1) * S + N; i += S + 1) {
        mp[i] = 1;
    }
    return;
}

void mat_transpose(sm dst, sm mat) {
    assert(dst.rows == mat.cols && dst.cols == mat.rows);
    assert(dst.p != mat.p);
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict mp = mat.p;
    
    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        const MAT_TYPE* mrp = mp + i * mat.stride;
        for (size_t j = 0; j < N; j++) {
            dp[i + j * dst.stride] = mrp[j];
        }
    }
    return;
}

void mat_transpose_blocked(sm dst, sm mat) {
    assert(dst.rows == mat.cols && dst.cols == mat.rows);
    assert(dst.p != mat.p);
    const size_t M = mat.rows;
    const size_t N = mat.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict mp = mat.p;
    
    #pragma omp parallel for collapse(2)
    for (size_t ib = 0; ib < M; ib += BLK) {
        for (size_t jb = 0; jb < N; jb += BLK) {
            size_t i_max = (ib + BLK > M) ? M : (ib + BLK);
            size_t j_max = (jb + BLK > N) ? N : (jb + BLK);

            for (size_t i = ib; i < i_max; i++) {
                const MAT_TYPE* mrp = mp + i * mat.stride;
                for (size_t j = jb; j < j_max; j++) {
                    dp[i + j * dst.stride] = mrp[j];
                }
            }
        }
    }
    return;
}

void mat_add(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    assert(dst.p != mat1.p && dst.p != mat2.p);
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict m1rp;
    const MAT_TYPE* restrict m2rp;

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        drp = dp + i * dst.stride;
        m1rp = m1p + i * mat1.stride;
        m2rp = m2p + i * mat2.stride;
        for (size_t j = 0; j < N; j++) {
            drp[j] = m1rp[j] + m2rp[j];
        }
    }
    return;
}

void mat_minus(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    assert(dst.p != mat1.p && dst.p != mat2.p);
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict m1rp;
    const MAT_TYPE* restrict m2rp;

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        drp = dp + i * dst.stride;
        m1rp = m1p + i * mat1.stride;
        m2rp = m2p + i * mat2.stride;
        for (size_t j = 0; j < N; j++) {
            drp[j] = m1rp[j] - m2rp[j];
        }
    }
    return;
}

void mat_addn(sm dst, sm mat, MAT_TYPE a) {
    assert(dst.rows == mat.rows && dst.cols == mat.cols);
    assert(dst.p != mat.p);
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict mp = mat.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict mrp;

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        drp = dp + i * dst.stride;
        mrp = mp + i * mat.stride;
        for (size_t j = 0; j < N; j++) {
            drp[j] = mrp[j] + a;
        }
    }
    return;
}

void mat_dotn(sm dst, sm mat, MAT_TYPE a) {
    assert(dst.rows == mat.rows && dst.cols == mat.cols);
    assert(dst.p != mat.p);
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict mp = mat.p;
    MAT_TYPE* restrict drp;
    const MAT_TYPE* restrict mrp;

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        drp = dp + i * dst.stride;
        mrp = mp + i * mat.stride;
        for (size_t j = 0; j < N; j++) {
            drp[j] = mrp[j] * a;
        }
    }
    return;
}

MAT_TYPE vec_dot(sm mat1, sm mat2) {
    assert(mat1.rows == 1 && mat2.cols == 1 && mat1.cols == mat2.rows);
    const size_t N = mat1.cols;
    MAT_TYPE prod = 0;
    const MAT_TYPE* m1p = mat1.p;
    const MAT_TYPE* m2p = mat2.p;
    
    #pragma omp parallel for reduction(+:prod)
    for (size_t i = 0; i < N; i++) {
        prod += m1p[i] * m2p[i];
    }
    return prod;
}

// plain matrix multiplication with moderate acceleration with openmp parallel
// try to avoid using macros in time-consuming part
void mat_dot(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat2.cols && mat1.cols == mat2.rows);
    assert(dst.p != mat1.p && dst.p != mat2.p);
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    const size_t K = mat1.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    mat_clear(dst);

    #pragma omp parallel for
    for (size_t i = 0; i < M; i++) {
        MAT_TYPE* drp = dp + i * dst.stride;
        for (size_t k = 0; k < K; k++) {
            const MAT_TYPE t = m1p[k + mat1.stride * i];
            const MAT_TYPE* m2rp = m2p + k * mat2.stride;
            #pragma omp simd
            for (size_t j = 0; j < N; j++) {
                drp[j] += t * m2rp[j];
            }
        }
    }
    return;
}

// large-scale matrix multiplication accelerated with blocking and loop expansion
void mat_dot_blocked(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat2.cols && mat1.cols == mat2.rows);
    assert(dst.p != mat1.p && dst.p != mat2.p);
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict m1p = mat1.p;
    const MAT_TYPE* restrict m2p = mat2.p;
    size_t M = dst.rows;
    size_t N = dst.cols;
    size_t K = mat1.cols;
    mat_clear(dst);

    #pragma omp parallel for collapse(2) schedule(dynamic, 1)
    for (size_t ib = 0; ib < M; ib += BLK) {
        for (size_t jb = 0; jb < N; jb += BLK) {
            for (size_t kb = 0; kb < K; kb += BLK) {
                // i-k-j loop for the current two blocks
                size_t i_max = (ib + BLK > M) ? M : (ib + BLK);
                size_t k_max = (kb + BLK > K) ? K : (kb + BLK);
                size_t j_max = (jb + BLK > N) ? N : (jb + BLK);

                size_t i = ib;
                for (; i + 8 - 1 < i_max; i += 8) {
                    MAT_TYPE* drp0 = dp + (i + 0) * dst.stride;
                    MAT_TYPE* drp1 = dp + (i + 1) * dst.stride;
                    MAT_TYPE* drp2 = dp + (i + 2) * dst.stride;
                    MAT_TYPE* drp3 = dp + (i + 3) * dst.stride;
                    MAT_TYPE* drp4 = dp + (i + 4) * dst.stride;
                    MAT_TYPE* drp5 = dp + (i + 5) * dst.stride;
                    MAT_TYPE* drp6 = dp + (i + 6) * dst.stride;
                    MAT_TYPE* drp7 = dp + (i + 7) * dst.stride;
                    
                    for (size_t k = kb; k < k_max; k++) {
                        const MAT_TYPE t0 = m1p[k + mat1.stride * (i + 0)];
                        const MAT_TYPE t1 = m1p[k + mat1.stride * (i + 1)];
                        const MAT_TYPE t2 = m1p[k + mat1.stride * (i + 2)];
                        const MAT_TYPE t3 = m1p[k + mat1.stride * (i + 3)];
                        const MAT_TYPE t4 = m1p[k + mat1.stride * (i + 4)];
                        const MAT_TYPE t5 = m1p[k + mat1.stride * (i + 5)];
                        const MAT_TYPE t6 = m1p[k + mat1.stride * (i + 6)];
                        const MAT_TYPE t7 = m1p[k + mat1.stride * (i + 7)];
                        const MAT_TYPE* m2rp = m2p + k * mat2.stride;
                        
                        #pragma omp simd
                        for (size_t j = jb; j < j_max; j++) {
                            const MAT_TYPE m2rpj = m2rp[j];
                            drp0[j] += t0 * m2rpj;
                            drp1[j] += t1 * m2rpj;
                            drp2[j] += t2 * m2rpj;
                            drp3[j] += t3 * m2rpj;
                            drp4[j] += t4 * m2rpj;
                            drp5[j] += t5 * m2rpj;
                            drp6[j] += t6 * m2rpj;
                            drp7[j] += t7 * m2rpj;
                        }
                    }
                }

                for (; i < i_max; i++) {
                    MAT_TYPE* drp = dp + i * dst.stride;
                    for (size_t k = kb; k < k_max; k++) {
                        const MAT_TYPE t = m1p[k + mat1.stride * i];
                        const MAT_TYPE* m2rp = m2p + k * mat2.stride;
                        
                        #pragma omp simd
                        for (size_t j = jb; j < j_max; j++) {
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
    if (dst.p == src.p) {
        return;
    }
    
    const size_t M = dst.rows;
    const size_t N = dst.cols;
    MAT_TYPE* restrict dp = dst.p;
    const MAT_TYPE* restrict sp = src.p;
    
    for (size_t i = 0; i < M; i++) {
        MAT_TYPE* restrict drp = dp + i * dst.stride;
        const MAT_TYPE* restrict srp = sp + i * src.stride;
        for (size_t j = 0; j < N; j++) {
            drp[j] = srp[j];
        }
    }
    return;
}

MAT_TYPE mat_det(sm mat) {
    assert(mat.rows == mat.cols);
    // LU decomposition with pivoting
    
    return 0;
}

void linsys_lu(sm x, sm A, sm b) {
    assert(x.cols == 1 && b.cols == 1 && A.cols == b.rows && A.rows == x.rows);
    
    return;
}

void linsys_qr(sm x, sm A, sm b) {
    assert(x.cols == 1 && b.cols == 1 && A.cols == b.rows && A.rows == x.rows);
    
    return;
}
