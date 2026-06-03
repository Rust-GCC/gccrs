/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-O3 -ftree-vectorize -fdump-tree-slp1-details" } */

typedef short i16;

void
foo (i16 *__restrict out, i16 *__restrict in, i16 x, unsigned n)
{
  unsigned k = 0;
  i16 tmp = x + 3;

  while (k < n)
    {
      tmp = tmp ^ 0x3f;

      out[k + 0]  = in[k + 0]  + tmp;
      out[k + 1]  = in[k + 1]  + tmp;
      out[k + 2]  = in[k + 2]  + tmp;
      out[k + 3]  = in[k + 3]  + tmp;
      out[k + 4]  = in[k + 4]  + tmp;
      out[k + 5]  = in[k + 5]  + tmp;
      out[k + 6]  = in[k + 6]  + tmp;
      out[k + 7]  = in[k + 7]  + tmp;
      out[k + 8]  = in[k + 8]  + tmp;
      out[k + 9]  = in[k + 9]  + tmp;
      out[k + 10] = in[k + 10] + tmp;
      out[k + 11] = in[k + 11] + tmp;
      out[k + 12] = in[k + 12] + tmp;
      out[k + 13] = in[k + 13] + tmp;
      out[k + 14] = in[k + 14] + tmp;
      out[k + 15] = in[k + 15] + tmp;

      k += 16;
    }
}

/* { dg-final { scan-tree-dump "Re-trying with swapped operands of stmts" "slp1" } } */
/* { dg-final { scan-tree-dump "Using a splat of the uniform operand" "slp1" } } */
/* { dg-final { scan-tree-dump "optimized: basic block part vectorized" "slp1" } } */
