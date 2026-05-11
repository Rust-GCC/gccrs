// PR c++/50479
// { dg-do compile }

void fn1 (int i = sizeof (i)) {}
void fn2 (int i, int = sizeof (i)) {}

void
g ()
{
  void fn4 (int i = sizeof (i));
  void fn5 (int i, int = sizeof (i));
  fn1 ();
  fn2 (42);
  fn4 ();
  fn5 (42);
}
