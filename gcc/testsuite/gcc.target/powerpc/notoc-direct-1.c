/* { dg-do compile } */
/* { dg-options "-mdejagnu-cpu=future -O2" } */
/* { dg-require-effective-target powerpc_elfv2 } */

/* Test that calls generated from PC-relative code are
   annotated with @notoc.  */

extern int yy0 (int);
extern void yy1 (int);

int zz0 (void) __attribute__((noinline));
void zz1 (int) __attribute__((noinline));

int xx (void)
{
  yy1 (7);
  return yy0 (5);
}

int zz0 ()
{
  asm ("");
  return 16;
};

void zz1 (int a __attribute__((__unused__)))
{
  asm ("");
};

int ww (void)
{
  zz1 (zz0 ());
  return 4;
}

/* { dg-final { scan-assembler {yy1@notoc} } } */
/* { dg-final { scan-assembler {yy0@notoc} } } */
/* { dg-final { scan-assembler {zz1@notoc} } } */
/* { dg-final { scan-assembler {zz0@notoc} } } */

