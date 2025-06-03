/* { dg-do compile { target { powerpc64*-*-* && lp64 } } } */
/* { dg-options "-mdejagnu-cpu=power9 -mvsx -O2" } */
/* { dg-require-effective-target powerpc_vsx } */

static unsigned short x[(16384/sizeof(unsigned short))] __attribute__ ((aligned (16)));
static unsigned short y[(16384/sizeof(unsigned short))] __attribute__ ((aligned (16)));
static unsigned short a;

void obfuscate(void *a, ...);

static void __attribute__((noinline)) do_one(void)
{
 unsigned long i;

 obfuscate(x, y, &a);

 for (i = 0; i < (16384/sizeof(unsigned short)); i++)
  y[i] = a * x[i];

 obfuscate(x, y, &a);
}

int main(void)
{
 unsigned long i;

 for (i = 0; i < 1000000; i++)
  do_one();

 return 0;
}
