/* { dg-do compile } */
/* { dg-additional-options "-march=rv64gcb_zicond -mabi=lp64d -mbranch-cost=4" { target rv64 } } */
/* { dg-additional-options "-march=rv32gcb_zicond -mabi=ilp32 -mbranch-cost=4" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Os" "-Og" "-Oz" } } */

#define TEST(TYPE, NAME) TYPE f##NAME(TYPE a, TYPE b, TYPE c) { a = -1; if (c < 10) a = b; return a; }

TEST(signed char, sc)
TEST(signed short, ss)
TEST(signed int, si)
TEST(signed long, sl)

/* { dg-final { scan-assembler-times "slti\t" 4 } } */
/* { dg-final { scan-assembler-times "addi\t" 4 } } */
/* { dg-final { scan-assembler-times "or\t" 4 } } */
/* { dg-final { scan-assembler-not "czero" } } */
