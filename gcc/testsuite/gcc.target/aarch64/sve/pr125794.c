/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-additional-options "-O2" } */

/* PR target/125794: a 128-bit Advanced SIMD integer constant whose high 64
   bits are zero (here { 0, 1, 0, 0 }) must be materialised with a 64-bit MOVI
   that zeroes the upper half of the register.  It was wrongly emitted as a
   replicating SVE "mov zN.d, #imm", which set the high 64 bits to a non-zero
   value and produced wrong code.  */

typedef unsigned V __attribute__((__vector_size__ (32)));

V x;

int
main ()
{
  x[5] = 1;
  for (unsigned i = 0; i < 8; i++)
    if (x[i] != (i == 5))
      __builtin_abort ();
  return 0;
}
