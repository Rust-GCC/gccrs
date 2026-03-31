// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::current_function.

#include <meta>

using namespace std::meta;

consteval info
foo (info x = current_function ())
{
  static_assert (current_function () == ^^foo);
  return x;
}

static_assert (foo (^^std) == ^^std);

void
qux ()
{
  static_assert (foo () == ^^qux);
}

struct T
{
  info t = current_function ();
};

struct U
{
  consteval U () {}
  info u = current_function ();
};

struct V : U
{
  using U::U;
  info v = current_function ();
};

void
bar ()
{
  static_assert (current_function () == ^^bar);
  static_assert (foo (^^foo) == ^^foo);
  static_assert (foo () == ^^bar);
  static_assert (T {}.t == ^^bar);
  consteval {
    static_assert (current_function () == ^^bar);
    consteval {
      static_assert (current_function () == ^^bar);
    }
  }
  auto l = [] () {
    int v = 0;
    static_assert (current_function () == parent_of (^^v));
    static_assert (parent_of (parent_of (current_function ())) == ^^bar);
  };
  auto l2 = [] () -> int (&) [current_function () == ^^bar ? 2 : 3] {
    static int a[2];
    return a;
  };
}

consteval bool
baz ()
{
  constexpr U u;
  static_assert (is_constructor (u.u) && parent_of (u.u) == ^^U);
  constexpr V v;
  static_assert (is_constructor (v.u) && parent_of (v.u) == ^^U);
  static_assert (is_constructor (v.v) && parent_of (v.v) == ^^V);
  return true;
}

static_assert (baz ());
