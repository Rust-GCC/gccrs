/* { dg-do run } */

int
main ()
{
  __label__ set, neg, check;
  void *ops[] = { &&set, &&neg, &&check };
  int i = 0;
  double foo = 0.0;

next:
  goto *ops[i++];

set:
  foo = 1.0;
  goto next;

neg:
  foo = -foo;
  goto next;

check:
  if (foo != -1.0)
    __builtin_abort ();
  return 0;
}
