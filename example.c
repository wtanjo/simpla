#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

// #define N ((int))

int main(void) {
    sm A = mat_from((float[9]){0}, 3, 3);
    mat_fill(A, 1);
    mat_clear(A);
    _mat_print(A);

    man("man");

    return 0;
}
