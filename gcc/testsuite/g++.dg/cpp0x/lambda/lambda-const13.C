// PR c++/123536
// { dg-do compile { target c++11 } }

int g;

template<class T>
void
fn1 ()
{
  T c = g;
  auto l = [] { c++; };
  l();
}

void
bar ()
{
  fn1<int&> ();
}
