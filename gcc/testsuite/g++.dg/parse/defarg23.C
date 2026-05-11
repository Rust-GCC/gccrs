// PR c++/50479
// { dg-do compile { target c++14 } }

inline void
fn1 ()
{
  int f (int i = []{ return 1; }());
}

auto l = [](int i = sizeof (i)) { };
void fn2 (int i = []{ return 1; }()) {}
void fn3 (int x = []{ decltype(x) y{}; return y; }()) {}
void fn6 (int i = sizeof (i), decltype (i) = sizeof (i)) {}

struct S {
  int mfn1 (int i = sizeof (i)) { return i; }
  int mfn2 (int i, int = sizeof (i)) { return i; }
  int lm = [](int i6 = sizeof (i6)) { return i6; }();
  int mfn3 (int i = []{ return 1; }()) { return i; }
  int i = mfn1 () + mfn2 (42) + mfn3 () + lm;
};

void
g ()
{
  auto ll = [](int i5 = sizeof (i5)) { };
  void fn4 (int i = []{ return 1; }());
  void fn5 (int x = []{ decltype(x) y{}; return y; }());
  fn1 ();
  fn2 ();
  fn3 ();
  fn4 ();
  fn5 ();
  fn6 ();
  l ();
}
