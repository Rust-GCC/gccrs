/* Test that jumps within the same section still use the short form (JAL).
   This ensures that the CROSSING_JUMP_P fix doesn't pessimize normal jumps.  */

/* { dg-do compile } */
/* { dg-options "-O2" } */

int global;

/* Simple function with a loop - jumps stay within .text section.  */
int
sum_to_n (int n)
{
  int sum = 0;
  for (int i = 1; i <= n; i++)
    sum += i;
  return sum;
}

/* Function with conditional - jumps stay within .text section.  */
int
abs_value (int x)
{
  if (x < 0)
    return -x;
  return x;
}

/* The backward jump in the loop should use "j" (JAL) not "jump" (AUIPC+JALR)
   since it doesn't cross section boundaries.  We verify that "jump" is NOT
   used for intra-section jumps.  */

/* { dg-final { scan-assembler-not "jump\\t\\.L\[0-9\]+,ra" } } */

