#include "simpla.h"
#include <assert.h>
#include <stdlib.h>

smf matf_from(float *array, size_t rows, size_t cols) {
    return (smf) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

smf matf_malloc(size_t rows, size_t cols) {
    float* array = (float*)malloc(rows * cols);
    return (smf) {
        .mat = array,
        .rows = rows,
        .cols = cols,
    };
}

float matf_index(smf mat, size_t row, size_t col) {
    return mat.mat[col + mat.cols * row];
}

void matf_free(smf mat) {
    free(mat.mat);
    return;
}
