#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    sm av = mat_from(NULL, 5, 5);
    av.p = (float[100]){0};
    mat_fill(av, 2);
    mat_print(av);
    printf("\n");
    mat_eye(av);
    mat_print(av);

    return 0;
}
