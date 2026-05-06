// PR target/124133
// { dg-options "-mdejagnu-cpu=power8 -mvsx" }
// { dg-require-effective-target powerpc_vsx }

typedef __INTPTR_TYPE__ intptr_t;

template <int>
void
foo (int x, short *y, intptr_t z)
{
  __builtin_vec_vsx_ld ((x + z) * 2, y);
}
