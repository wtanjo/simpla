#include <stdio.h>
#include <simpla.h>

#define MAT_TYPE float

#define N ((int)1e4)

int main(void) {
    sm bigm1 = mat_malloc(N, N);
    sm bigm2 = mat_malloc(N, N);
    sm prod = mat_malloc(N, N);
    mat_rand(bigm1, 0, 1);
    mat_rand(bigm2, 0, 1);
    mat_dot(prod, bigm1, bigm2);

    mat_free(bigm1);
    mat_free(bigm2);
    mat_free(prod);
 
    return 0;
}
