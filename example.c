#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    sm A = mat_from((float[]) {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9},
                    3, 3);
    mat_print(A);
    
    sm B = mat_from((float[]) {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9},
                    3, 3);
    mat_print(A);
    
    sm P = mat_from((float[9]){0}, 3, 3);
    mat_dot(P, A, B);
    mat_print(P);
    return 0;
}
