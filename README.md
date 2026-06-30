# simpla

A simple linear alagebra library written in C. Basically for learning and my personal use.

## Inspiration

Openblas is an awesome library, but check this function which computes the product of two matrices:
```C
cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
            N, N, N,
            1.0f,
            matrix1, N,
            matrix2, N,
            0.0f,
            prod, N);
```
What does all the parameters even mean? Let's just keep it simpler.

## Build

Clone this repo and do `make` to make the libraries. Do `(sudo) make install` if you wish. You can modified [Makefile](./Makefile) according to your specific needs.

## A Simple Tutorial

Here's a simple example with this library:

```C
#include "simpla.h" // if the header file is in this folder

#define MAT_TYPE (float) // by default it is float

int main(void) {
    
    return 0;
}
```
