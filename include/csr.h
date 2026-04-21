/*
 * csr.h — Sparse matrix in CSR (Compressed Sparse Row) format
 *
 * CSR is a standard format for storing sparse matrices efficiently.
 * Instead of storing all n*m entries, we only store the non-zero values.
 *
 * For a matrix A with 'nnz' non-zero entries and 'nrows' rows:
 *
 *   values[k]      = the k-th non-zero value
 *   col_indices[k] = the column of the k-th non-zero value
 *   row_ptr[i]     = index in 'values' where row i begins
 *   row_ptr[nrows] = nnz  (sentinel value, marks the end)
 *
 * Example:
 *   A = | 5  0  0  2 |
 *       | 0  3  0  0 |
 *       | 0  1  4  0 |
 *
 *   values      = { 5, 2, 3, 1, 4 }
 *   col_indices = { 0, 3, 1, 1, 2 }
 *   row_ptr     = { 0, 2, 3, 5 }
 *
 * This format makes matrix-vector product (matvec) very efficient:
 * to compute row i of A*x, just iterate over values[row_ptr[i]..row_ptr[i+1]-1].
 */

#ifndef CSR_H
#define CSR_H

#include <stddef.h>  /* for size_t */

/*
 * CSRMatrix — a sparse matrix stored in CSR format.
 * All values are long integers (suitable for exact integer arithmetic).
 */
typedef struct {
    long   *values;       /* non-zero values, length = nnz          */
    size_t *col_indices;  /* column index of each value, length = nnz */
    size_t *row_ptr;      /* row_ptr[i]: start of row i, length = nrows+1 */
    size_t  nrows;        /* number of rows                          */
    size_t  ncols;        /* number of columns                       */
    size_t  nnz;          /* number of non-zero entries              */
} CSRMatrix;

/*
 * csr_alloc — allocate a CSRMatrix with given dimensions.
 *
 * Returns NULL if allocation fails.
 * The caller is responsible for filling values, col_indices, row_ptr.
 */
CSRMatrix *csr_alloc(size_t nrows, size_t ncols, size_t nnz);

/*
 * csr_free — free all memory owned by a CSRMatrix.
 */
void csr_free(CSRMatrix *A);

/*
 * csr_matvec — compute y = A * x (matrix-vector product).
 *
 * x must have length A->ncols.
 * y must be a pre-allocated array of length A->nrows.
 * Results are exact integer arithmetic (no rounding).
 */
void csr_matvec(const CSRMatrix *A, const long *x, long *y);

/*
 * csr_print — print the matrix in dense form (for debugging).
 * Useful to verify small examples by hand.
 */
void csr_print(const CSRMatrix *A);

#endif /* CSR_H */