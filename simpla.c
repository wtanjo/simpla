#include "simpla.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

simpmat mat_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (simpmat) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

simpmat mat_malloc(size_t rows, size_t cols) {
    MAT_TYPE* array = (MAT_TYPE*)malloc(sizeof(MAT_TYPE) * rows * cols);
    assert(array != NULL);
    return (simpmat) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

MAT_TYPE mati(simpmat mat, size_t row, size_t col) {
    return mat.mat[col + mat.cols * row];
}

void mat_free(simpmat mat) {
    free(mat.mat);
    return;
}

void mat_add(simpmat mat1, simpmat mat2) {
    
    return;
}

void mat_print(simpmat mat) {
    for (size_t r = 0; r < mat.rows; r++) {
        for (size_t c = 0; c < mat.cols; c++) {
            printf("%f ", mati(mat, r, c));
        }
        printf("\n");
    }
    return;
}
