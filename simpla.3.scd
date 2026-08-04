simpla(3)

# NAME

simpla - A simple linear algebra library written in C. 

# SYNOPSIS

```
#include <simpla/simpla.h>

#define MAT_TYPE float

int main(void) {
    sm A = mat_from((float[9]){1, 2, 3,
                               4, 5, 6,
                               7, 8, 9},
                    3, 3);
    sm B = mat_alloc(3, 3);
    sm C = mat_alloc(3, 3);

    mat_rand(B, 0, 1);
    mat_dot(C, A, B);
    mat_free(B);
    mat_free(C);
    return 0;
}
```

# DESCRIPTION

*simpla* is a simple linear algebra library written in C. It provides simple function interfaces and concise implementation of linear algebra operations with quite a small sacrifice of performance.

The core data structure is *sm*, which stands for NOT for Sadism & Masochism BUT for *s*imple *m*atrix:

```
typedef
struct
{
    MAT_TYPE* p;
    size_t rows;   // number of rows
    size_t cols;   // number of columns
    size_t stride; // specifically for slicing
} sm;
```

where MAT_TYPE is a user-defined macro used for indicating the data type the matrix should be using, with the default value float.
