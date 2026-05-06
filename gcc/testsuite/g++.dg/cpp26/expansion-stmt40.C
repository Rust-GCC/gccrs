// PR c++/124120
// { dg-do compile { target c++14 } }
// { dg-require-weak "" }
// { dg-options "" }

struct A
{
  int x;
  constexpr explicit A (int v) : x(v) {}
  constexpr A &operator ++ () { ++x; return *this; }
  constexpr int operator * () const { return x; }
  constexpr bool operator != (const A &o) const { return x != o.x; }
  constexpr A operator + (int o) const { A r (x + o); return r; }
  constexpr int operator - (const A &o) const { return x - o.x; }
};

namespace N
{
  struct B { constexpr B () {} };
  constexpr A begin (const B &) { return A (0); }
  constexpr A end (const B &) { return A (6); }
}

int
bar ()
{
  int r = 0;
  template for (constexpr auto i : N::B {})			// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += i;
  return r;
}

extern void plugh ();

[[gnu::weakref ("plugh")]] static void xyzzy ();

void
plugh ()
{
}
