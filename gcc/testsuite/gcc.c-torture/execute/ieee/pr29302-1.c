/* { dg-do run }
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } } */
extern void abort (void);

int main (void)
{
  int n;
  long double x;

  x = 1/0.0;

  n = (x == 1/0.0);

  if (n == 1)
    return 0;
  else
    abort ();
}
