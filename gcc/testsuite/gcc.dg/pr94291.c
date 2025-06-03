/* PR rtl-optimization/94291 */
/* { dg-do compile } */
/* { dg-options "-Og" } */

unsigned a;

unsigned
foo (void)
{
  unsigned x
    = (__builtin_sub_overflow ((long long) a, 0, &x)
       ? 1 : (__INTPTR_TYPE__) __builtin_memmove (&x, foo, 1)); /* { dg-warning "accessing data memory with program memory address.*" "" { target avr-*-* } } */

  return a;
}
