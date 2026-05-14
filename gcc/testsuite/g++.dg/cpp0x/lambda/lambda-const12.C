// PR c++/123536
// { dg-do compile { target c++11 } }

int g;

struct S {
  static int static_data_member;
};

template<int = 1>
void
fn1 ()
{
  int &c = g;
  auto l = [] { c++; };
  l();
}

template<int N = 1>
void
fn2 ()
{
  const int &c = N;
  // TODO This will be valid once we implement P2686.
  auto l = [] { int i = c; (void) i; }; // { dg-error ".c. is not captured" }
  l();
}

void
fn3 ()
{
  int &c = g;
  auto l = [] { c++; };
  l();
}

void
fn4 ()
{
  int n = 42;
  const int &c = n;
  auto l = [] { int i = c; (void) i; }; // { dg-error ".c. is not captured" }
  l();
}

template<class T>
void
fn5 ()
{
  int& c = T::static_data_member;
  auto l = [] { c++; };
  l();
}

template<class T>
void
fn6 ()
{
  int& c = T::static_data_member;
  [&c] {
    [] { c++; }();
  };
}

void
bar ()
{
  fn1 ();
  fn2 ();
  fn3 ();
  fn4 ();
  fn5<S>();
  fn6<S>();
}
