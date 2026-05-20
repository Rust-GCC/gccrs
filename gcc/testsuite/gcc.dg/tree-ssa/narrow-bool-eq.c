/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

/* Verify the fix for PR112533 at -O2.
   (~a & 1) == (~b & 1) is lowered via inlining to
   (bool)(~a) == (bool)(~b), then existing forwprop rules cancel
   the NOTs giving (bool)(a) == (bool)(b), which this rule
   simplifies to (bool)(a ^ b) == 0.  */

typedef unsigned int u32;

static _Bool
is_even (u32 a)
{
  return a % 2 == 0;
}

_Bool
same_evenness (u32 a, u32 b)
{
  return is_even (a) == is_even (b);
}

_Bool
diff_evenness (u32 a, u32 b)
{
  return is_even (a) != is_even (b);
}

/* Verify XOR form is produced.  */
/* { dg-final { scan-tree-dump-times "\\^" 2 "optimized" } } */
