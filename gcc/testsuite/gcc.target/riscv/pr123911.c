/* { dg-do compile */
/* { dg-options "-march=rv64gv -mabi=lp64d" { target { rv64 } } } */
/* { dg-options "-march=rv32gv -mabi=ilp32" { target { rv32 } } } */

typedef __attribute__((__vector_size__(8))) char W;
typedef __attribute__((__vector_size__(64))) short V;

V
foo(V v, W w)
{
  __builtin_memmove(30 + (char *)&v, &w, 1);
  __builtin_memmove(&v, &w, 8);
  return v;
}

