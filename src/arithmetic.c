/*
 * arithmetic.c — Implementation of basic integer arithmetic routines
 */

#include "arithmetic.h"

#include <stdio.h>   /* printf (for error messages) */

/* ------------------------------------------------------------------ */
/* GCD — Euclidean algorithm                                            */
/* ------------------------------------------------------------------ */

/*
 * The Euclidean algorithm is based on the identity:
 *   gcd(a, b) = gcd(b, a mod b)
 *
 * We reduce a and b until b reaches 0, at which point a = gcd.
 *
 * We take absolute values first to handle negative inputs.
 */
long gcd(long a, long b)
{
    long t;

    /* Make both values non-negative */
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    /* Euclidean iteration */
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Extended Euclidean algorithm                                         */
/* ------------------------------------------------------------------ */

/*
 * We maintain two "tracks" of Bézout coefficients in parallel:
 *
 *   At each step, we have:
 *     old_r = old_s * a + old_t * b
 *     r     =     s * a +     t * b
 *
 * and we perform the same division step on both sides.
 *
 * This is the standard iterative formulation (avoids recursion overhead).
 */
long extended_gcd(long a, long b, long *x, long *y)
{
    long old_r = a, r = b;
    long old_s = 1, s = 0;
    long old_t = 0, t = 1;
    long quotient, tmp;

    while (r != 0) {
        quotient = old_r / r;

        /* update remainder */
        tmp   = r;
        r     = old_r - quotient * r;
        old_r = tmp;

        /* update s coefficient */
        tmp   = s;
        s     = old_s - quotient * s;
        old_s = tmp;

        /* update t coefficient */
        tmp   = t;
        t     = old_t - quotient * t;
        old_t = tmp;
    }

    *x = old_s;   /* Bézout coefficient for a */
    *y = old_t;   /* Bézout coefficient for b */
    return old_r; /* gcd(a, b) */
}

/* ------------------------------------------------------------------ */
/* Fast modular exponentiation ("square and multiply")                  */
/* ------------------------------------------------------------------ */

/*
 * The key observation: any exponent can be written in binary.
 * For example, 13 = 1101 in binary, so:
 *   base^13 = base^8 * base^4 * base^1
 *
 * Algorithm:
 *   result = 1
 *   while exp > 0:
 *     if exp is odd:  result = result * base (mod m)
 *     base = base * base (mod m)
 *     exp  = exp >> 1
 *
 * This uses O(log exp) multiplications instead of O(exp).
 */
long mod_pow(long base, long exp, long m)
{
    long result = 1;

    if (m == 1) return 0;  /* everything is 0 mod 1 */

    base = base % m;       /* reduce base before starting */

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % m;
        }
        base = (base * base) % m;
        exp  = exp / 2;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Modular inverse                                                      */
/* ------------------------------------------------------------------ */

/*
 * By Bézout's theorem: if gcd(a, m) = 1, there exist x, y such that
 *   a*x + m*y = 1
 * Taking this equation mod m:
 *   a*x ≡ 1 (mod m)
 * So x is the modular inverse of a.
 *
 * We use extended_gcd to find x, then reduce it to [0, m-1].
 */
long mod_inv(long a, long m)
{
    long x, y;
    long g = extended_gcd(a, m, &x, &y);

    if (g != 1) {
        /* gcd != 1: inverse does not exist */
        fprintf(stderr, "mod_inv: %ld has no inverse mod %ld (gcd=%ld)\n",
                a, m, g);
        return -1;
    }

    /* x might be negative; reduce to [0, m-1] */
    return (x % m + m) % m;
}