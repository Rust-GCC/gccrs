// PR tree-optimization/124754
// { dg-do compile }
// { dg-options "-O2 -Wno-stringop-overflow -Wno-stringop-overread" }

char a[3], *c, *d;

void
foo ()
{
  try
    {
      __builtin_memcpy (&a, "ABCDEFGHIJKLMNOPQRSTUVWX", 24);
      c = new char ('@');
      d = new char ('@');
      __builtin_memcpy (&a, &a + 8, 1);
    }
  catch (...)
    {
      __builtin_abort ();
    }
}
