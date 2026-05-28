/* { dg-do compile } */
/* { dg-options "-O2 -msse2" } */

typedef int v4ssi __attribute__ ((vector_size(16)));
typedef unsigned int v4usi __attribute__ ((vector_size(16)));

v4ssi si,so;
v4usi ui,uo;

void foo()
{
  so = si + 1;
}

void bar()
{
  uo = ui + 1;
}

/* { dg-final { scan-assembler-times "\[ \t\]psubd\[ \t\]" 2 } } */
