// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::current_class.

#include <meta>

using namespace std::meta;

consteval info
foo (info x = current_class ())
{
  try
    {
      current_class ();
    }
  catch (const std::meta::exception &)
    {
      return x;
    }
  throw 1;
}

static_assert (foo (^^std) == ^^std);

struct T
{
  static_assert (current_class () == ^^T);
  static_assert (foo () == ^^T);
  info t = current_class ();
};

struct U
{
  static_assert (current_class () == ^^U);
  static_assert (foo () == ^^U);
  consteval U () {}
  info u = current_class ();
};

struct V : U
{
  static_assert (current_class () == ^^V);
  static_assert (foo () == ^^V);
  using U::U;
  consteval {
    static_assert (current_class () == ^^V);
    static_assert (foo () == ^^V);
    consteval {
      static_assert (current_class () == ^^V);
      static_assert (foo () == ^^V);
    }
  }
  void baz ()
  {
    static_assert (current_class () == ^^V);
    static_assert (foo () == ^^V);
    static_assert (T {}.t == ^^V);
    static_assert (U {}.u == ^^U);
  }
};

void
bar ()
{
  auto l = [] () {
    int v = 0;
    static_assert (current_class () == parent_of (parent_of (^^v)));
    static_assert (parent_of (current_class ()) == ^^bar);
  };
}
