/* { dg-do compile { target int128 } } */
/* { dg-options "-O2 -mno-avx512f -mavx2" } */

char c, d;
_BitInt(2048) b;

void
foo (__int128, _BitInt(1024) a)
{
  b *= 0;
  c %= (char)(a ?: d);
}
