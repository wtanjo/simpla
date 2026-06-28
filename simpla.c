#include "simpla.h"
#include <assert.h>
#include <stdlib.h>

simpmat matf_from(MAT_TYPE* array, size_t rows, size_t cols) {
    return (simpmat) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

simpmat matf_malloc(size_t rows, size_t cols) {
    MAT_TYPE* array = (MAT_TYPE*)malloc(sizeof(MAT_TYPE) * rows * cols);
    return (simpmat) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

MAT_TYPE matf_index(simpmat mat, size_t row, size_t col) {
    return mat.mat[col + mat.cols * row];
}

void matf_free(simpmat mat) {
    free(mat.mat);
    return;
}
