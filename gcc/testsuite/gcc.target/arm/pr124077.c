/* { dg-do compile } */
/* { dg-options "-Os" } */
/* { dg-require-effective-target arm_arch_v8m_base_ok } */
/* { dg-add-options arm_arch_v8m_base } */

extern int y;
extern void bar(void);
extern volatile int *p;

#define FILL p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 4; p[4] = 5; p[5] = 6; p[6] = 7;

void foo(int x, int z)
{
  y = x & z;
  if (y)
  {
    FILL FILL FILL FILL FILL;
  }
  else
  {
    bar();
  }
}

/* { dg-final { scan-assembler-not "cmp\tr\[0-9\], #0" } } */
