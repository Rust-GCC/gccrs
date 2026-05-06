/* { dg-do compile } */
/* { dg-options "-O2 --param=avoid-fma-max-bits=8 -march=rv64gv -mabi=lp64d -fdump-tree-widening_mul" } */

typedef __attribute__((__vector_size__(1))) unsigned char U;
typedef __attribute__((__vector_size__(1))) signed char V;

U u;
signed char c;

U
foo(V v)
{
  v *= c;
  return u + v;
}

/* { dg-final { scan-tree-dump "FMA" "widening_mul" } } */
