#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "simpla.h"

MAT_TYPE rand_MAT_TYPE() {
    return (MAT_TYPE)rand() / (MAT_TYPE)RAND_MAX;
}

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
        .stride = cols,
    };
}

sm mat_malloc(size_t rows, size_t cols) {
    MAT_TYPE* array = (MAT_TYPE*)malloc(sizeof(MAT_TYPE) * rows * cols);
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

void mat_print(sm mat) {
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            printf("%f ", MATI(mat, i, j));
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void mat_fill(sm mat, MAT_TYPE e) {
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            MATI(mat, i, j) = e;
        }
    }
    return;
}

// randomize mat with (pseudo) random values between lower bound l and upper bound u
void mat_rand(sm mat, MAT_TYPE l, MAT_TYPE u) {
    assert(u > l);
    for (size_t i = 0; i < mat.rows; i++) {
        for (size_t j = 0; j < mat.cols; j++) {
            MATI(mat, i, j) = rand_MAT_TYPE() * (u - l) + l;
        }
    }
    return;
}

void mat_eye(sm mat) {
    assert(mat.rows == mat.cols);
    mat_fill(mat, 0);
    for (size_t i = 0; i < mat.rows; i++) {
        MATI(mat, i, i) = 1;
    }
    return;
}

void mat_add(sm dst,sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            MATI(dst, i, j) = MATI(mat1, i, j) + MATI(mat2, i, j);
        }
    }
    return;
}

void mat_minus(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat1.cols);
    assert(dst.rows == mat2.rows && dst.cols == mat2.cols);
    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            MATI(dst, i, j) = MATI(mat1, i, j) - MATI(mat2, i, j);
        }
    }
    return;
}

MAT_TYPE vec_dot(sm mat1, sm mat2) {
    assert(mat1.rows == 1 && mat2.cols == 1 && mat1.cols == mat2.rows);
    MAT_TYPE prod = 0;
    for (size_t i = 0; i < mat1.cols; i++) {
        prod += MATI(mat1, 0, i) * MATI(mat2, i, 0);
    }
    return prod;
}

void mat_dot(sm dst, sm mat1, sm mat2) {
    assert(dst.rows == mat1.rows && dst.cols == mat2.cols && mat1.cols == mat2.rows);
    mat_fill(dst, 0);
    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t k = 0; k < mat1.cols; k++) {
            MAT_TYPE t = MATI(mat1, i, k);
            for (size_t j = 0; j < dst.cols; j++) {
                MATI(dst, i, j) += t * MATI(mat2, k, j);
            }
        }
    }
    return;
}

// basically 'clone' (deep copy), which is a relatively time-consuming step
void mat_assign(sm dst, sm src) {
    assert(dst.rows == src.rows && dst.cols == src.cols);
    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            MATI(dst, i, j) = MATI(src, i, j);
        }
    }
    return;
}
