/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-phiopt1-details" } */

/* tree-optimization/125557.  factor_out_binary_common_operand handles more than
   one operation kind and, when several join PHIs share the differing operand,
   factors each of them -- collapsing the arms to a single select.  */

/* Two PHIs share the differing operand a/b: the multiply and the add are each
   factored out, so both arms reduce to one select of a/b.  */
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

/* A shift is factored just like the arithmetic ops.  */
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

/* f2 factors twice (the * and the +), f3 once (the <<).  */
/* { dg-final { scan-tree-dump-times " changed to factor operation out from COND_EXP" 3 "phiopt1" } } */
