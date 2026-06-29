#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    sm A = mat_from((float[]) {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9},
                    3, 3);
    sm s1 = SLICE(A, 1, 2, 1, 2);
    mat_print(A);
    
    sm B = mat_from((float[]) {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9},
                    3, 3);
    sm s2 = SLICE(B, 0, 1, 0, 1);
    mat_print(s2);
    
    sm P = mat_from((float[9]){0}, 2, 2);
    mat_dot(P, s1, s2);
    mat_print(P);
    return 0;
}
