/* PR target/124138 */
/* { dg-do run } */
/* { dg-options "-Wno-psabi" } */

typedef __attribute__((__vector_size__ (8 * sizeof (unsigned long long)))) unsigned long long V;

[[gnu::noipa]] V
foo (V x)
{
  return __builtin_shuffle (x, (V) { 0, 1, 2, 5, 4, 5, 6, 7 });
}

int
main ()
{
  V x = foo ((V) { 1, 2, 3, 4, 5, 6, 7, 8 });
  if (x[0] != 1 || x[1] != 2 || x[2] != 3 || x[3] != 6
      || x[4] != 5 || x[5] != 6 || x[6] != 7 || x[7] != 8)
    __builtin_abort ();
}
