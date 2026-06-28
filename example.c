#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    simpmat av = matf_from(NULL, 2, 2);
    av.mat = (float[]){2,1,2,4};
    printf("%f", matf_index(av,1,1));

    return 0;
}
