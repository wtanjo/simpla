#include <stdio.h>

#define \
    MAN_MAN \
    "You can use this function to get a manual for this library without any external dependencies like mandoc, just with C code.\nThe acceptable arguments are: \"man\", \"MAT_TYPE\", \"sm\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\", \"\"\n"

#define                                                                 \
    MAN_TYPE                                                            \
    "MAT_TYPE is a user-defined macro used for indicating which data type the entried of the matrix (sm) should be.\n"

#define                                                                 \
    MAN_SM                                                              \
    "sm is the data type for the matrices themselves, standing for \"simple matrix\".\n"

#define                                                                 \
    MAN_MATI                                                            \
    "MATI is a macro for indexing the matrices. MATI(mat, i, j) gives the entry at the i_th row and the j_th column.\n"

#define                                                                 \
    MAN_SLICE                                                           \
    "SLICE is a macro for slicing out submatrices. SLICE(mat, r1, r2, c1, c2) gives the submatrix with index ranges: rows[r1, r2] and columns[c1, c2] (close intervals). This slice share the same memory with its parental matrix.\n"

#define                                                                 \
    MAN_PRINT                                                           \
    "MAT_PRINT is a macro that prints out matrices with only its entries. Usage: MAT_PRINT(mat).\n_mat_print is the macro that prints out matrices with its variable name and its entries, working as an advanced version of MAT_PRINT. Usage: _mat_print(mat).\n"

#define                                                                 \
    MAN_FROM                                                            \
    "mat_from is a function that accepts a pointer to the data and two integers that indicates the number of rows and columns, and returns a matrix (sm) according to the parameters. The pointer (data array) can be either on stack or on heap. You can also pass a compound literal(C99) (eg., (MAT_TYPE[]){0, 1}).\n"

#define                                                                 \
    MAN_ALLOC                                                           \
    "mat_alloc is a function that allocates memory for a matrix with rows and columns specified by the two parameters. It uses aligned_alloc(C11) to be cache-friendly and easy to optimize.\n"

#define                                                                 \
    MAN_FREE                                                            \
    "mat_free is a function that frees the memory of the given matrix.\n"

#define                                                                 \
    MAN_CLEAR                                                           \
    "mat_clear is a function that fills the given matrix with 0. The effect is the same as mat_fill(mat, 0), but mat_clear is generally faster using memset.\n"

#define                                                                 \
    MAN_FILL                                                            \
    "mat_fill is a function that fills the given matrix with the given value.\n"



#define                                         \
    MAN_ERR                                     \
    "There's no manual for %s.\n"
