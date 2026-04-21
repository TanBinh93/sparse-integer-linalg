/*
 * csr.c — Implementation of CSR sparse matrix operations
 */

#include "csr.h"

#include <stdlib.h>   /* malloc, free */
#include <stdio.h>    /* printf */

/* ------------------------------------------------------------------ */
/* Memory management                                                    */
/* ------------------------------------------------------------------ */

CSRMatrix *csr_alloc(size_t nrows, size_t ncols, size_t nnz)
{
    CSRMatrix *A = malloc(sizeof(CSRMatrix));
    if (!A) return NULL;

    A->values      = malloc(nnz    * sizeof(long));
    A->col_indices = malloc(nnz    * sizeof(size_t));
    A->row_ptr     = malloc((nrows + 1) * sizeof(size_t));

    /* If any allocation failed, clean up and return NULL */
    if (!A->values || !A->col_indices || !A->row_ptr) {
        free(A->values);
        free(A->col_indices);
        free(A->row_ptr);
        free(A);
        return NULL;
    }

    A->nrows = nrows;
    A->ncols = ncols;
    A->nnz   = nnz;

    return A;
}

void csr_free(CSRMatrix *A)
{
    if (!A) return;
    free(A->values);
    free(A->col_indices);
    free(A->row_ptr);
    free(A);
}

/* ------------------------------------------------------------------ */
/* Core operation: matrix-vector product y = A * x                     */
/* ------------------------------------------------------------------ */

/*
 * The key idea: for each row i, we iterate only over the non-zero entries.
 * row_ptr[i] to row_ptr[i+1]-1 gives the range of non-zeros in row i.
 *
 * This is O(nnz) instead of O(nrows * ncols) for a dense matrix.
 * For a matrix with 1% non-zeros, this is ~100x faster.
 */
void csr_matvec(const CSRMatrix *A, const long *x, long *y)
{
    size_t i, k;

    for (i = 0; i < A->nrows; i++) {
        y[i] = 0;
        /* iterate over all non-zero entries in row i */
        for (k = A->row_ptr[i]; k < A->row_ptr[i + 1]; k++) {
            y[i] += A->values[k] * x[A->col_indices[k]];
        }
    }
}

/* ------------------------------------------------------------------ */
/* Debug utility                                                        */
/* ------------------------------------------------------------------ */

/*
 * Print the matrix in dense form, showing zeros explicitly.
 * Only suitable for small matrices — used to verify correctness.
 */
void csr_print(const CSRMatrix *A)
{
    size_t i, j, k;

    printf("CSRMatrix (%zu x %zu, nnz=%zu):\n", A->nrows, A->ncols, A->nnz);

    for (i = 0; i < A->nrows; i++) {
        printf("  [");
        for (j = 0; j < A->ncols; j++) {
            long val = 0;
            /* Search for entry (i, j) among non-zeros of row i */
            for (k = A->row_ptr[i]; k < A->row_ptr[i + 1]; k++) {
                if (A->col_indices[k] == j) {
                    val = A->values[k];
                    break;
                }
            }
            printf(" %4ld", val);
        }
        printf(" ]\n");
    }
}