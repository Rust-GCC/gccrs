/* { dg-do run } */
/* { dg-options "-fsanitize=alignment,bounds -fsanitize-trap" } */


struct sp {
  unsigned int c;
  int *a __attribute__ ((counted_by (c)));
};

int
main ()
{
  struct sp y;
  y.a = __builtin_malloc (30 * sizeof (int));
  y.c = 30;
  y.a[0] = 4;
  y.a[29] = 77;
  (void) &y;

  return 0;
}

