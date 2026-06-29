#include <stdio.h>
#include <simpla.h>

#define MAT_TYPE float

#define N ((int)2e3)

int main(void) {
    sm A = mat_from((float[]){2,3,1,5,10,5,7,3,6}, 3, 3);
    MAT_PRINT(A);
    mat_eye(SLICE(A, 1, 2, 1, 2));
    MAT_PRINT(A);
    return 0;
}
