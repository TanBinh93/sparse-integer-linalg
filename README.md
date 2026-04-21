# sparse-integer-linalg

A small C library implementing two fundamental building blocks for
algorithmic number theory and exact computation:

1. **Sparse matrix-vector product** (CSR format, integer arithmetic)
2. **Basic integer arithmetic** (GCD, extended GCD, modular exponentiation, modular inverse)

---

## Motivation

This project was written as a code sample to illustrate my programming
style in C, in the context of applying for a software engineer position
on the [PARI/GP](https://pari.math.u-bordeaux.fr/) project at Inria Bordeaux.

The two modules reflect skills directly relevant to the position:

- **CSR sparse matrices over integers** — a data structure used in the
  linear algebra step of class group computations (a key bottleneck in
  algorithms for imaginary quadratic fields, relevant to post-quantum cryptography).

- **Integer arithmetic routines** — the Euclidean algorithm and modular
  arithmetic are foundational in PARI/GP and in algorithms for number
  fields, lattice reduction, and cryptographic primitives.

A key design choice: all computations are done with **exact integer arithmetic**
(`long`), with no floating-point. This reflects the philosophy of symbolic
computation systems like PARI/GP, where results must be exact.

---

## Project structure

```
sparse-integer-linalg/
├── include/
│   ├── csr.h          # CSR sparse matrix: struct and function declarations
│   └── arithmetic.h   # GCD, modular arithmetic: declarations
├── src/
│   ├── csr.c          # sparse matrix-vector product, alloc/free, print
│   └── arithmetic.c   # gcd, extended_gcd, mod_pow, mod_inv
├── tests/
│   ├── test_csr.c     # tests for CSR operations
│   └── test_arithmetic.c  # tests for arithmetic routines
└── Makefile
```

---

## CSR format

The Compressed Sparse Row (CSR) format stores only non-zero entries:

```
A = | 5  0  0  2 |
    | 0  3  0  0 |
    | 0  1  4  0 |

values      = { 5, 2, 3, 1, 4 }     ← non-zero values, row by row
col_indices = { 0, 3, 1, 1, 2 }     ← column of each value
row_ptr     = { 0, 2, 3, 5 }        ← row_ptr[i]: start of row i in values[]
```

The matrix-vector product `y = A * x` runs in `O(nnz)` time,
where `nnz` is the number of non-zeros — much faster than the
`O(nrows * ncols)` cost for a dense matrix when the matrix is sparse.

---

## Build and test

Requirements: `gcc`, `make` (standard on any Linux system).

```bash
# Build and run all tests
make test

# Build only
make

# Clean
make clean
```

Expected output:

```
--- Running CSR tests ---
=== CSR matrix tests ===
  test_matvec_basic        PASSED  (y = [13, 6, 14])
  test_matvec_zero_vector  PASSED
  test_matvec_identity     PASSED
  test_matvec_negative     PASSED
All CSR tests passed.

--- Running arithmetic tests ---
=== Integer arithmetic tests ===
  test_gcd                 PASSED
  test_extended_gcd        PASSED
  test_mod_pow             PASSED
  test_mod_inv             PASSED
All arithmetic tests passed.
```

---

## Notes

- All functions use `long` integers (exact arithmetic, no rounding).
- Memory allocation failures are handled explicitly (no silent crashes).
- The extended GCD implementation follows the standard iterative formulation,
  maintaining Bézout coefficients at each step.
- `mod_pow` uses the "square and multiply" method: O(log exp) multiplications.
