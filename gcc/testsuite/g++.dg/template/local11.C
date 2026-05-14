// PR c++/123536

int g;

template<class T>
void
fn1 ()
{
  T c = g;
  struct A { static void f() { c++; } };
  A::f();
}

void
bar ()
{
  fn1<int&> ();
}
