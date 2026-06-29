/* { dg-do compile } */
/* { dg-options "-mabi=32 -march=mips32r2 -mno-abicalls -fno-pic -fdump-rtl-final" } */

extern void use (char *);

int
f (int n)
{
  char * p = (char *) __builtin_alloca (n);

  use (p);
  return p[0];
}

/* { dg-final { scan-rtl-dump-times "UNSPEC_BLOCKAGE" 3 "final" } } */
