#include "simpla.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// which means 'mat'rix 'i'ndex
#define MATI(mat, row, col) (mat).p[(col) + (mat).cols * (row)]

MAT_TYPE rand_MAT_TYPE() {
    return (MAT_TYPE)rand() / (MAT_TYPE)RAND_MAX;
}

sm mat_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
    };
}

sm mat_malloc(size_t rows, size_t cols) {
    MAT_TYPE* array = (MAT_TYPE*)malloc(sizeof(MAT_TYPE) * rows * cols);
    assert(array != NULL);
    return (sm) {
        .p = array,
        .rows = rows,
        .cols = cols,
    };
}

void mat_free(sm mat) {
    free(mat.p);
    return;
}

void mat_fill(sm mat, MAT_TYPE e) {
    for (size_t r = 0; r < mat.rows; r++) {
        for (size_t c = 0; c < mat.cols; c++) {
            MATI(mat, r, c) = e;
        }
    }
    return;
}

// randomize mat with (pseudo) random values between lower bound l and upper bound u
void mat_rand(sm mat, MAT_TYPE l, MAT_TYPE u) {
    assert(u > l);
    for (size_t r = 0; r < mat.rows; r++) {
        for (size_t c = 0; c < mat.cols; c++) {
            MATI(mat, r, c) = rand_MAT_TYPE() * (u - l) + l;
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
    for (size_t r = 0; r < dst.rows; r++) {
        for (size_t c = 0; c < dst.cols; c++) {
            MATI(dst, r, c) = MATI(mat1, r, c) + MATI(mat2, r, c);
        }
    }
    return;
}

void mat_print(sm mat) {
    for (size_t r = 0; r < mat.rows; r++) {
        for (size_t c = 0; c < mat.cols; c++) {
            printf("%f ", MATI(mat, r, c));
        }
        printf("\n");
    }
    return;
}
