/* { dg-do compile } */
/* { dg-options "-fchecking" } */
/* { dg-require-effective-target nonlocal_goto } */

struct S { int i; };
void baz(struct S *p)
{
  __builtin_setjmp(p--);
}
