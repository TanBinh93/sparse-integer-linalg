/*
 * arithmetic.h — Basic integer arithmetic routines
 *
 * These are fundamental building blocks in algorithmic number theory
 *
 * All functions operate on exact integers (long), with no floating-point.
 */

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <stddef.h>

/*
 * gcd — compute the Greatest Common Divisor of a and b.
 *
 * Uses the Euclidean algorithm: gcd(a, b) = gcd(b, a mod b).
 * Works correctly for negative inputs (returns a non-negative result).
 *
 * Examples:
 *   gcd(12, 8)  = 4
 *   gcd(0, 5)   = 5
 *   gcd(-6, 9)  = 3
 */
long gcd(long a, long b);

/*
 * extended_gcd — compute gcd(a, b) and Bézout coefficients x, y.
 *
 * Finds x, y such that:  a*x + b*y = gcd(a, b)
 *
 * This is the Extended Euclidean Algorithm, essential for:
 *   - computing modular inverses
 *   - solving linear Diophantine equations
 *   - lattice basis reduction (used in class group algorithms)
 *
 * The result gcd is stored in *x and *y (Bézout coefficients).
 * Returns gcd(a, b).
 *
 * Example:
 *   extended_gcd(12, 8, &x, &y) returns 4, with x=1, y=-1
 *   because 12*1 + 8*(-1) = 4
 */
long extended_gcd(long a, long b, long *x, long *y);

/*
 * mod_pow — compute base^exp mod m using fast exponentiation.
 *
 * Uses the "square and multiply" method: O(log exp) multiplications.
 * This is far more efficient than naive repeated multiplication.
 *
 * Requires: m > 0, exp >= 0
 * Returns 0 if m == 1 (since everything is 0 mod 1).
 *
 * Example:
 *   mod_pow(2, 10, 1000) = 1024 mod 1000 = 24
 */
long mod_pow(long base, long exp, long m);

/*
 * mod_inv — compute the modular inverse of a modulo m.
 *
 * Finds x such that: a * x ≡ 1 (mod m)
 * Uses extended_gcd internally.
 *
 * Requires: gcd(a, m) == 1  (a and m must be coprime)
 * Returns -1 if the inverse does not exist (i.e., gcd(a,m) != 1).
 *
 * Example:
 *   mod_inv(3, 7) = 5   because 3 * 5 = 15 ≡ 1 (mod 7)
 */
long mod_inv(long a, long m);

#endif /* ARITHMETIC_H */