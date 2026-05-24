/* PR target/54412 */
/* { dg-do run { target { avx && { ! ia32 } } } } */
/* { dg-skip-if "PR target/54412 exercises the Win64 ABI" { ! x86_64-*-mingw* } } */
/* { dg-options "-O2 -mavx -std=gnu99" } */

#include "avx-check.h"
#include <immintrin.h>

typedef double v4d __attribute__ ((vector_size (32), aligned (32)));
typedef char byte_vec __attribute__ ((vector_size (32)));

struct avx_wrapper
{
  __m256 value;
};

#define PERM_LEFT_BVEC \
  (byte_vec) { 63, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, \
               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, \
               29, 30 }

__attribute__ ((noinline, noclone)) static v4d
roundtrip_v4d (v4d x)
{
  return x;
}

__attribute__ ((noinline)) static __m256
pass_m256 (__m256 x)
{
  return x;
}

__attribute__ ((noinline)) static struct avx_wrapper
pass_wrapper (struct avx_wrapper x)
{
  return x;
}

__attribute__ ((noinline)) static byte_vec
permute (byte_vec x, byte_vec y)
{
  return __builtin_shuffle (x, y, PERM_LEFT_BVEC);
}

static void
test_v4d_roundtrip (void)
{
  v4d x = (v4d) { 1.0, 2.0, 3.0, 4.0 };
  v4d r = roundtrip_v4d (x);
  v4d expected = (v4d) { 1.0, 2.0, 3.0, 4.0 };

  if (__builtin_memcmp (&r, &expected, sizeof (r)) != 0)
    __builtin_abort ();
}

static void
test_m256_and_wrapper_by_value (void)
{
  __m256 m = _mm256_set_ps (8.0f, 7.0f, 6.0f, 5.0f,
                            4.0f, 3.0f, 2.0f, 1.0f);
  __m256 r = pass_m256 (m);
  struct avx_wrapper x;
  struct avx_wrapper y;

  x.value = _mm256_set_ps (16.0f, 15.0f, 14.0f, 13.0f,
                           12.0f, 11.0f, 10.0f, 9.0f);
  y = pass_wrapper (x);

  if (__builtin_memcmp (&r, &m, sizeof (r)) != 0)
    __builtin_abort ();

  if (__builtin_memcmp (&y, &x, sizeof (y)) != 0)
    __builtin_abort ();
}

static void
test_two_arg_bytevec_shuffle (void)
{
  byte_vec x = { 1, 2, 3, 4, 5, 6, 7, 8,
                 9, 10, 11, 12, 13, 14, 15, 16,
                 17, 18, 19, 20, 21, 22, 23, 24,
                 25, 26, 27, 28, 29, 30, 31, 32 };
  byte_vec y = { 11, 12, 13, 14, 15, 16, 17, 18,
                 19, 110, 111, 112, 113, 114, 115, 116,
                 117, 118, 119, 120, 121, 122, 123, 124,
                 125, 126, 127, 88, 89, 90, 91, 92 };
  byte_vec expected = { 92, 1, 2, 3, 4, 5, 6, 7,
                        8, 9, 10, 11, 12, 13, 14, 15,
                        16, 17, 18, 19, 20, 21, 22, 23,
                        24, 25, 26, 27, 28, 29, 30, 31 };
  byte_vec z = permute (x, y);

  if (__builtin_memcmp (&z, &expected, sizeof (z)) != 0)
    __builtin_abort ();
}

static void
avx_test (void)
{
  test_v4d_roundtrip ();
  test_m256_and_wrapper_by_value ();
  test_two_arg_bytevec_shuffle ();
}
