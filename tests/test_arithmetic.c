/*
 * test_arithmetic.c — Tests for integer arithmetic routines
 *
 * We test gcd, extended_gcd, mod_pow, and mod_inv
 * with concrete values that can be verified by hand.
 */

#include "arithmetic.h"

#include <assert.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/* GCD tests                                                            */
/* ------------------------------------------------------------------ */
static void test_gcd(void)
{
    /* standard cases */
    assert(gcd(12, 8)  == 4);
    assert(gcd(8,  12) == 4);   /* commutative */
    assert(gcd(17, 13) == 1);   /* coprime */
    assert(gcd(100, 75) == 25);

    /* edge cases */
    assert(gcd(0, 5)  == 5);    /* gcd(0, n) = n */
    assert(gcd(5, 0)  == 5);
    assert(gcd(0, 0)  == 0);    /* gcd(0, 0) = 0 by convention */

    /* negative inputs */
    assert(gcd(-6, 9)  == 3);
    assert(gcd(6,  -9) == 3);
    assert(gcd(-6, -9) == 3);

    printf("  test_gcd                 PASSED\n");
}

/* ------------------------------------------------------------------ */
/* Extended GCD tests                                                   */
/* ------------------------------------------------------------------ */
static void test_extended_gcd(void)
{
    long x, y, g;

    /* 12*x + 8*y = 4:  one solution is x=1, y=-1 */
    g = extended_gcd(12, 8, &x, &y);
    assert(g == 4);
    assert(12 * x + 8 * y == g);   /* verify Bézout identity */

    /* 17*x + 13*y = 1 */
    g = extended_gcd(17, 13, &x, &y);
    assert(g == 1);
    assert(17 * x + 13 * y == 1);

    /* 35*x + 15*y = 5 */
    g = extended_gcd(35, 15, &x, &y);
    assert(g == 5);
    assert(35 * x + 15 * y == 5);

    /* gcd(0, 7) = 7, with 0*x + 7*y = 7 */
    g = extended_gcd(0, 7, &x, &y);
    assert(g == 7);
    assert(0 * x + 7 * y == 7);

    printf("  test_extended_gcd        PASSED\n");
}

/* ------------------------------------------------------------------ */
/* Modular exponentiation tests                                         */
/* ------------------------------------------------------------------ */
static void test_mod_pow(void)
{
    /* 2^10 = 1024; 1024 mod 1000 = 24 */
    assert(mod_pow(2, 10, 1000) == 24);

    /* 3^0 = 1 (mod anything > 1) */
    assert(mod_pow(3, 0, 7) == 1);

    /* x^1 = x mod m */
    assert(mod_pow(5, 1, 13) == 5);

    /* Fermat's little theorem: a^(p-1) ≡ 1 (mod p) for prime p, gcd(a,p)=1 */
    /* 2^6 mod 7 = 64 mod 7 = 1 */
    assert(mod_pow(2, 6, 7) == 1);

    /* anything mod 1 = 0 */
    assert(mod_pow(123, 45, 1) == 0);

    /* larger example: 7^100 mod 13 */
    /* By Fermat: 7^12 ≡ 1 (mod 13), so 7^100 = 7^(12*8+4) ≡ 7^4 = 2401 ≡ 2401 mod 13 */
    /* 2401 = 184*13 + 9, so 7^100 mod 13 = 9 */
    assert(mod_pow(7, 100, 13) == 9);

    printf("  test_mod_pow             PASSED\n");
}

/* ------------------------------------------------------------------ */
/* Modular inverse tests                                                */
/* ------------------------------------------------------------------ */
static void test_mod_inv(void)
{
    long inv;

    /* 3 * 5 = 15 ≡ 1 (mod 7) */
    inv = mod_inv(3, 7);
    assert(inv == 5);
    assert((3 * inv) % 7 == 1);

    /* 2 * 6 = 12 ≡ 1 (mod 11) */
    inv = mod_inv(2, 11);
    assert(inv == 6);
    assert((2 * inv) % 11 == 1);

    /* self-inverse: 1 is always its own inverse */
    inv = mod_inv(1, 7);
    assert(inv == 1);

    /* inverse does not exist when gcd(a, m) != 1 */
    inv = mod_inv(6, 9);   /* gcd(6,9) = 3 */
    assert(inv == -1);

    printf("  test_mod_inv             PASSED\n");
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== Integer arithmetic tests ===\n");
    test_gcd();
    test_extended_gcd();
    test_mod_pow();
    test_mod_inv();
    printf("All arithmetic tests passed.\n\n");
    return 0;
}
