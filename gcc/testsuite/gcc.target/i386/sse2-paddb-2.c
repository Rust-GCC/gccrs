/* { dg-do compile } */
/* { dg-options "-O2 -msse2" } */

typedef char v16sqi __attribute__ ((vector_size(16)));
typedef unsigned char v16uqi __attribute__ ((vector_size(16)));

v16sqi si,so;
v16uqi ui,uo;

void foo()
{
  so = si - 1;
}

void bar()
{
  uo = ui - 1;
}

/* { dg-final { scan-assembler-times "\[ \t\]v?paddb\[ \t\]" 2 } } */
