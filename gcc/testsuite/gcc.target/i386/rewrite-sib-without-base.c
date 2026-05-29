/* { dg-do compile } */
/* { dg-options "-O2 -masm=att" } */
/* { dg-final { scan-assembler-not "\\(,%" } } */

_Complex a;
_Complex int b;
void c(void) { a = *(_Complex char *)__builtin_memset(&a, 1, 2) / b; }
