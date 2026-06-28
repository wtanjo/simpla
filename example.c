#include <stdio.h>
#include "simpla.h"

int main(void) {
    smf av = matf_from(NULL, 2, 2);
    av.mat = (float[]){2,1,2,3};
    printf("%f", matf_index(av,1,1));

    return 0;
}
