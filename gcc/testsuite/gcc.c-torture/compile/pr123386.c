/* PR middle-end/123386 */

char *a;
int b;

void
foo (char *)
{
  if (b)
    __builtin_abort ();
}

void
bar (char *c)
{
  __asm__ goto ("" : : : : d);
  foo (a);
  __asm__ goto ("" : : : : d);
  if (c)
    bar (a);
  __asm__ goto ("" : : : : d);
d:
  if (b)
    __builtin_abort ();
}
