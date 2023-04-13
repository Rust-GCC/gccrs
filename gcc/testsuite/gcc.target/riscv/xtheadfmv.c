/* { dg-do compile } */
/* { dg-options "-march=rv32gc_xtheadfmv" { target { rv32 } } } */
/* { dg-options "-march=rv64gc_xtheadfmv" { target { rv64 } } } */

#ifndef __riscv_xtheadfmv
#error Feature macro not defined
#endif

int
foo (int a)
{
  return a;
}

