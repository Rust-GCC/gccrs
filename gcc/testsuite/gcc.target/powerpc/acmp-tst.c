/* { dg-do compile } */
/* { dg-options "-O2" } */

#define TESTS \
  X(signed char, qi) \
  X(unsigned char, uqi) \
  X(short, hi) \
  X(signed short, shi) \
  X(unsigned short, uhi) \
  X(int, si) \
  X(signed int, ssi) \
  X(unsigned int, usi) \
  X(long, di) \
  X(signed long, sdi) \
  X(unsigned long, udi) \
  X(vector signed __int128, sti) \
  X(vector unsigned __int128, uti)

#define X(T, name) \
bool word_exchange_##name (T *ptr, T *expected, T * desired) \
{ \
  return __builtin_ppc_atomic_cas_local (ptr, expected, desired, 0, \
					 __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE); \
}

TESTS

/* { dg-final { scan-assembler-times {\mlbarx +[0-9]+,[0-9]+,[0-9]+,1} 2 } } */
/* { dg-final { scan-assembler-times {\mlharx +[0-9]+,[0-9]+,[0-9]+,1} 3 } } */
/* { dg-final { scan-assembler-times {\mlwarx +[0-9]+,[0-9]+,[0-9]+,1} 3 } } */
/* { dg-final { scan-assembler-times {\mldarx +[0-9]+,[0-9]+,[0-9]+,1} 3 } } */
/* { dg-final { scan-assembler-times {\mlqarx +[0-9]+,[0-9]+,[0-9]+,1} 2 } } */
