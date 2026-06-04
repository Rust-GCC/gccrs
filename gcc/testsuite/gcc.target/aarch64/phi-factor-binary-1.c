/* { dg-do compile } */
/* { dg-options "-O2" } */

/* tree-optimization/125557: factor_out_binary_common_operand computes a binary
   operation shared between a PHI's two arms once, behind a single select,
   instead of once per arm -- and when several join PHIs share the differing
   operand it collapses them to one select, which also removes the
   data-dependent branch.  These are the asm wins of patch 1 on its own.  */

/* f1: one multiply behind a select, not one per arm.  */
int
f1 (int cond, int a, int b, int c)
{
  int x;
  if (cond)
    x = a * c;
  else
    x = b * c;
  return x;
}

/* f3: one shift, not one per arm (snappy's address-arithmetic shape).  */
unsigned long
f3 (int cond, unsigned long a, unsigned long b, int s)
{
  unsigned long x;
  if (cond)
    x = a << s;
  else
    x = b << s;
  return x;
}

/* f2: two results share the differing operand (a/b): one select feeds both, so
   only one multiply and one add remain and the diamond is branchless.  */
void
f2 (int cond, int a, int b, int c, int d, int *p, int *q)
{
  int x, y;
  if (cond)
    {
      x = a * c;
      y = a + d;
    }
  else
    {
      x = b * c;
      y = b + d;
    }
  *p = x;
  *q = y;
}

/* f1 and f2 each keep a single multiply (two without the factoring).  */
/* { dg-final { scan-assembler-times {\tmul\t} 2 } } */
/* f3 keeps a single shift (two without the factoring).  */
/* { dg-final { scan-assembler-times {\tlsl\t} 1 } } */
/* Each diamond becomes a branchless select; no data-dependent branch remains.  */
/* { dg-final { scan-assembler-not {\tcbn?z\t} } } */
/* { dg-final { scan-assembler-not {\tb\.[a-z]} } } */
