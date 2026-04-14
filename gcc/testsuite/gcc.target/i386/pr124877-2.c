/* { dg-do compile } */
/* { dg-options "-O -masm=att" } */

typedef union { unsigned char x; signed char y; }
__attribute__((__transparent_union__)) T;
void bar(int, T x);
void foo(signed char x)
{
  bar(0, x);
}

/* PROMOTE_PROTOTYPES should make us pass x as int, but zero extended
   according to the type of the first union member.  */
/* { dg-final { scan-assembler "movzbl" } } */
