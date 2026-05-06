/* { dg-do compile { target rv64 } } */
/* { dg-additional-options "-march=rv64gcb_zicond -mabi=lp64d" } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og" "-Os" "-Oz" } } */


int foo (int t)
{
  return 3 * t - 1;
}

/* { dg-final { scan-assembler-not "slliw\t" } } */
/* { dg-final { scan-assembler-not "addw\t" } } */
/* { dg-final { scan-assembler-times "sh1add\t" 1 } } */
/* { dg-final { scan-assembler-times "addiw\t" 1 } } */
