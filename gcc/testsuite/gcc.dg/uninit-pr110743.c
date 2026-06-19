/* { dg-do compile } */
/* { dg-options "-O2 -Wall" } */

void foo (int *);

void bar()
{
  int x, tem;
  tem = x; /* { dg-bogus "used uninitialized" } */
  tem = tem | 5;
  x = tem;
  foo (&x);
}

void baz()
{
  int x, tem;
  tem = x; /* { dg-bogus "used uninitialized" } */
  tem = tem & 5;
  x = tem;
  foo (&x);
}
