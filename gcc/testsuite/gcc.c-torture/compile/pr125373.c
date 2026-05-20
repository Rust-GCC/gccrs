/* PR target/125373 */

int foo (int);

void
bar (int *v, int w)
{
  for (int i = 0; i < (__INTPTR_TYPE__) (v - 1); i++)
    if (v)
      v[i] = v[w];
}

int
main ()
{
  int a[] = { 0, 0, 0, 0 };
  bar (a, 4);
  for (int i = 0; i < 4; i++)
    foo (a[i]);
}
