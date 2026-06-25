/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gcbv_zicond -mabi=lp64d" { target { rv64 } } } */
/* { dg-options "-O2 -march=rv32gcbv_zicond -mabi=ilp32" { target { rv32 } } } */

#define TEST(TYPE,NAME) _Bool f1_##NAME(TYPE a) { TYPE b = a << 4; return b == -128; }

TEST (unsigned char, uc);
TEST (signed char, sc);

TEST (unsigned short, us);
TEST (signed short, ss);

TEST (unsigned int, ui);
TEST (signed int, si);

TEST (unsigned long, ul);
TEST (signed long, sl);

/* The unsigned char and unsigned short cases collapse to a trivial li.  */
/* { dg-final { scan-assembler-times {\tli} 2 } } */

/* The signed char case will have an andi.  It's the only one.  */
/* { dg-final { scan-assembler-times {\tandi} 1 } } */
/* { dg-final { scan-assembler-not {\tsext.b} } } */

/* Other than the degenerate unsigned char/short cases, every test has
   an sll, addi and seqz.  */
/* { dg-final { scan-assembler-times {\taddi} 6 } } */
/* { dg-final { scan-assembler-times {\tseqz} 6 } } */

/* The signed char, and degenerate unsigned tests have slli[w].  */
/* { dg-final { scan-assembler-times {\tslli} 5 } } */

/* The signed short will need an sext.  */
/* { dg-final { scan-assembler-times {\tsext.h} 1 } } */


