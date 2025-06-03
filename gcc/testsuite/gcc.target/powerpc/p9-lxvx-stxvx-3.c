/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx -O3 -mfloat128" } */
/* { dg-require-effective-target ppc_float128_sw } */
/* { dg-require-effective-target powerpc_vsx } */
/* { dg-final { scan-assembler "lxvx" } } */
/* { dg-final { scan-assembler "stxvx" } } */
/* { dg-final { scan-assembler-not "lxvd2x" } } */
/* { dg-final { scan-assembler-not "stxvd2x" } } */
/* { dg-final { scan-assembler-not "xxpermdi" } } */

/* Verify P9 vector loads and stores are used rather than the
   load-swap/swap-store workarounds for P8.  */
#define SIZE (16384/sizeof(__float128))

static __float128 x[SIZE] __attribute__ ((aligned (16)));
static __float128 y[SIZE] __attribute__ ((aligned (16)));
static __float128 a;

void obfuscate(void *a, ...);

void __attribute__((noinline)) do_one(void)
{
  unsigned long i;

  obfuscate(x, y, &a);

  for (i = 0; i < SIZE; i++)
    y[i] = a * x[i];

  obfuscate(x, y, &a);
}
