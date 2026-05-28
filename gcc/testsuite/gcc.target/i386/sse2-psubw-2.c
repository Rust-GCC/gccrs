/* { dg-do compile } */
/* { dg-options "-O2 -msse2" } */

typedef short v8shi __attribute__ ((vector_size(16)));
typedef unsigned short v8uhi __attribute__ ((vector_size(16)));

v8shi si,so;
v8uhi ui,uo;

void foo()
{
  so = si + 1;
}

void bar()
{
  uo = ui + 1;
}

/* { dg-final { scan-assembler-times "\[ \t\]psubw\[ \t\]" 2 } } */
