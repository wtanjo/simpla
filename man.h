#include <stdio.h>

#define                                                                 \
    MAN_TYPE                                                            \
    "MAT_TYPE is a user-defined macro used for indicating which data type the entried of the matrix (sm) should be.\n"

#define                                                                 \
    MAN_SM                                                              \
    "sm is the data type for the matrices themselves, standing for \"simple matrix\".\n"

#define                                                                 \
    MAN_MATI                                                            \
    "MATI is the macro for indexing the matrices. MATI(mat, i, j) gives the entry at the i_th row and the j_th column.\n"

#define                                                                 \
    MAN_SLICE                                                           \
    "SLICE is the macro for slicing out submatrices. SLICE(mat, r1, r2, c1, c2) gives the submatrix with index ranges: rows[r1, r2] and columns[c1, c2] (close intervals). This slice share the same memory with its parental matrix.\n"

#define                                                                 \
    MAN_PRINT                                                           \
    "MAT_PRINT is the macro that prints out matrices with only its entries. Usage: MAT_PRINT(mat).\n_mat_print is the macro that prints out matrices with its variable name and its entries, working as an advanced version of MAT_PRINT. Usage: _mat_print(mat).\n"



#define                                         \
    MAN_ERR                                     \
    "There's no manual for %s.\n"
