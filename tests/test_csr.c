/*
 * test_csr.c — Tests for CSR sparse matrix operations
 *
 * Each test is self-contained: it builds a small matrix by hand,
 * runs the operation, and checks the result with assert().
 * If all assertions pass, the program exits with code 0.
 */

#include "csr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helper: build the example matrix from the README                     */
/*                                                                      */
/*   A = | 5  0  0  2 |                                                 */
/*       | 0  3  0  0 |                                                 */
/*       | 0  1  4  0 |                                                 */
/* ------------------------------------------------------------------ */
static CSRMatrix *build_example_matrix(void)
{
    /*
     * Non-zero entries, listed row by row:
     *   row 0: (0,0)=5, (0,3)=2
     *   row 1: (1,1)=3
     *   row 2: (2,1)=1, (2,2)=4
     */
    CSRMatrix *A = csr_alloc(3, 4, 5);
    assert(A != NULL);

    /* values and column indices, in row-major order */
    A->values[0] = 5;  A->col_indices[0] = 0;
    A->values[1] = 2;  A->col_indices[1] = 3;
    A->values[2] = 3;  A->col_indices[2] = 1;
    A->values[3] = 1;  A->col_indices[3] = 1;
    A->values[4] = 4;  A->col_indices[4] = 2;

    /* row_ptr[i] = index of first non-zero in row i */
    A->row_ptr[0] = 0;  /* row 0 starts at index 0 */
    A->row_ptr[1] = 2;  /* row 1 starts at index 2 */
    A->row_ptr[2] = 3;  /* row 2 starts at index 3 */
    A->row_ptr[3] = 5;  /* sentinel: total number of non-zeros */

    return A;
}

/* ------------------------------------------------------------------ */
/* Test 1: basic matvec                                                 */
/* ------------------------------------------------------------------ */
static void test_matvec_basic(void)
{
    /*
     * x = [1, 2, 3, 4]^T
     *
     * Expected: y = A * x
     *   y[0] = 5*1 + 2*4 = 5 + 8  = 13
     *   y[1] = 3*2       = 6
     *   y[2] = 1*2 + 4*3 = 2 + 12 = 14
     */
    CSRMatrix *A = build_example_matrix();

    long x[4] = {1, 2, 3, 4};
    long y[3];

    csr_matvec(A, x, y);

    assert(y[0] == 13);
    assert(y[1] == 6);
    assert(y[2] == 14);

    printf("  test_matvec_basic        PASSED  (y = [%ld, %ld, %ld])\n",
           y[0], y[1], y[2]);

    csr_free(A);
}

/* ------------------------------------------------------------------ */
/* Test 2: matvec with zero vector → result must be all zeros           */
/* ------------------------------------------------------------------ */
static void test_matvec_zero_vector(void)
{
    CSRMatrix *A = build_example_matrix();
    long x[4] = {0, 0, 0, 0};
    long y[3] = {99, 99, 99};  /* pre-fill with garbage to catch bugs */

    csr_matvec(A, x, y);

    assert(y[0] == 0);
    assert(y[1] == 0);
    assert(y[2] == 0);

    printf("  test_matvec_zero_vector  PASSED\n");
    csr_free(A);
}

/* ------------------------------------------------------------------ */
/* Test 3: identity matrix                                              */
/*                                                                      */
/*   I = | 1  0  0 |                                                    */
/*       | 0  1  0 |                                                    */
/*       | 0  0  1 |                                                    */
/*                                                                      */
/*   I * x = x  for any x                                               */
/* ------------------------------------------------------------------ */
static void test_matvec_identity(void)
{
    CSRMatrix *I = csr_alloc(3, 3, 3);
    assert(I != NULL);

    I->values[0] = 1; I->col_indices[0] = 0;
    I->values[1] = 1; I->col_indices[1] = 1;
    I->values[2] = 1; I->col_indices[2] = 2;

    I->row_ptr[0] = 0;
    I->row_ptr[1] = 1;
    I->row_ptr[2] = 2;
    I->row_ptr[3] = 3;

    long x[3] = {7, -3, 42};
    long y[3];

    csr_matvec(I, x, y);

    assert(y[0] == 7);
    assert(y[1] == -3);
    assert(y[2] == 42);

    printf("  test_matvec_identity     PASSED\n");
    csr_free(I);
}

/* ------------------------------------------------------------------ */
/* Test 4: negative values and large integers                           */
/* ------------------------------------------------------------------ */
static void test_matvec_negative_values(void)
{
    /*
     * A = | -3   0 |
     *     |  0   7 |
     *
     * x = [2, -5]^T
     * y = [-6, -35]
     */
    CSRMatrix *A = csr_alloc(2, 2, 2);
    assert(A != NULL);

    A->values[0] = -3; A->col_indices[0] = 0;
    A->values[1] =  7; A->col_indices[1] = 1;
    A->row_ptr[0] = 0;
    A->row_ptr[1] = 1;
    A->row_ptr[2] = 2;

    long x[2] = {2, -5};
    long y[2];
    csr_matvec(A, x, y);

    assert(y[0] == -6);
    assert(y[1] == -35);

    printf("  test_matvec_negative     PASSED\n");
    csr_free(A);
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== CSR matrix tests ===\n");
    test_matvec_basic();
    test_matvec_zero_vector();
    test_matvec_identity();
    test_matvec_negative_values();
    printf("All CSR tests passed.\n\n");
    return 0;
}
