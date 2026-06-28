#include <stdio.h>
#include "simpla.h"

#define MAT_TYPE float

int main(void) {
    simpmat av = mat_from(NULL, 2, 2);
    av.mat = (float[]){2,1,2,4};
    mat_print(av);

    return 0;
}
