// CWG3140 - Allowing expansion over non-constant std::array
// { dg-do compile { target c++14 } }
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
  struct B { constexpr B (int n) : b (n) {} int b; };
  constexpr A begin (const B &) { return A (0); }
  constexpr A end (const B &x) { return A (x.b); }	// { dg-error "is not usable in a constant expression" }
  struct C { };
  constexpr A begin (const C &) { return A (0); }
  constexpr A end (const C &x) { return A (6); }
}

int
foo ()
{
  int r = 0;
  template for (constexpr auto m : N::B (3))	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += m;
  return r;
}

int
bar ()
{
  int r = 0;
  template for (auto m : N::B (3))		// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += m;					// { dg-message "was not declared 'constexpr'" }
  return r;
}

int
baz ()
{
  int r = 0;
  template for (constexpr auto m : N::C ())	// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += m;
  return r;
}

int
qux ()
{
  int r = 0;
  template for (auto m : N::C ())		// { dg-warning "'template for' only available with" "" { target c++23_down } }
    r += m;
  return r;
}
