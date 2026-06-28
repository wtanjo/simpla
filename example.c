#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    sm a = mat_from(NULL, 5, 5);
    a.p = (float[25]){0};
    mat_fill(a, 1);
    sm b = mat_malloc(5, 5);
    mat_fill(b, 1);
    sm sum = mat_from(NULL, 5, 5);
    sum.p = (float[25]){0};
    mat_add(sum, a, b);
    mat_print(a);
    mat_print(sum);
    mat_eye(a);
    mat_print(a);

    mat_free(b);
    return 0;
}
