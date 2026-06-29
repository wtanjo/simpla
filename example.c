#include <stdio.h>
#include <simpla.h>

#define MAT_TYPE float

#define N ((int)2e3)

int main(void) {
    sm A = mat_from((float[]){2,3,1,5,4,5,7,3,6}, 3, 3);
    mat_print(A);
    mat_eye(SLICE(A, 1, 2, 1, 2));
    mat_print(A);
    return 0;
}
