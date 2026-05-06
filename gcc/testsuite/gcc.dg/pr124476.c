/* PR rtl-optimization/124476 */
/* { dg-do run } */
/* { dg-require-effective-target lp64 } */
/* { dg-options "-O1 -favoid-store-forwarding" } */

/* Verify that overlapping stores are handled correctly.
   The avoid-store-forwarding pass must respect "last writer wins"
   semantics when stores have partially overlapping byte ranges.

   The expected value is little-endian specific: the memmove writes
   bytes 0-3 last (zeroing them), leaving bytes 4-7 as 0x05 from the
   earlier memset, so f = 0x0505050500000000 on little-endian.  */

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
int main () { return 0; }
#else

int y;

__attribute__((noipa)) void
foo (int a, long *ret)
{
  long f;
  char c;
  __builtin_memset (2 + (char *) &f, a, 6);
  char n = *(char *) __builtin_memset (&c, a, 1);
  __builtin_memmove (&f, &y, 4);
  long r = f + a + n;
  *ret = r;
}

int
main ()
{
  long x;
  foo (5, &x);
  if (x != 0x050505050000000aL)
    __builtin_abort ();
  return 0;
}

#endif
