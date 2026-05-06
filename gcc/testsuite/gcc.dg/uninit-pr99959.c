/* { dg-do compile } */
/* { dg-options "-O2 -Wuninitialized" } */

void g (void)
{
  int i;

  void f (int *t)
  {
    __builtin_printf ("%i", i); /* { dg-warning  "is used uninitialized" } */
  }

  int t __attribute__ ((cleanup (f))) = 0;
}
