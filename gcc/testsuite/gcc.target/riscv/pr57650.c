/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gc_zicond -mabi=lp64d -mbranch-cost=4" { target rv64 } } */
/* { dg-additional-options "-march=rv32gc_zicond -mabi=ilp32 -mbranch-cost=4" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Os" "-Og" "-Oz" } } */

int baz (int);

int
bar (char *x, int y, int z)
{
  if (x && y > z)
    return baz (1);
  return 0;
}

/* { dg-final { scan-assembler-not "snez\t" } } */
/* { dg-final { scan-assembler-not "seq\t" } } */
/* { dg-final { scan-assembler-times "czero" 1 } } */
