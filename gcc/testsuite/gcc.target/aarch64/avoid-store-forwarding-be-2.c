/* PR rtl-optimization/123754 */
/* { dg-do run } */
/* { dg-require-effective-target aarch64_big_endian } */
/* { dg-require-effective-target int128 } */
/* { dg-options "-O2 -favoid-store-forwarding" } */

typedef unsigned __int128 u128;

union u {
  u128 v;
  unsigned char b[16];
};

u128 __attribute__ ((noinline))
foo (unsigned char x, unsigned char y, u128 init)
{
  union u u;
  u.v = init;
  u.b[0] = x;
  u.b[15] = y;
  return u.v;
}

int
main (void)
{
  u128 init = ((u128) 0xfedcba9876543210ULL << 64) | 0x0123456789abcdefULL;
  u128 r = foo (0xAA, 0xBB, init);

  unsigned char *p = (unsigned char *) &r;
  unsigned char *q = (unsigned char *) &init;

  if (p[0] != 0xAA || p[15] != 0xBB)
    __builtin_abort ();

  for (int i = 1; i < 15; i++)
    if (p[i] != q[i])
      __builtin_abort ();

  return 0;
}
