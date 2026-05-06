// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::current_namespace.

#include <meta>

using namespace std::meta;

static_assert (current_namespace () == ^^::);

consteval info
foo (info x = current_namespace ())
{
  static_assert (current_namespace () == ^^::);
  return x;
}

static_assert (foo (^^std) == ^^std);
static_assert (foo () == ^^::);

namespace N
{
  static_assert (foo () == ^^N);
  static_assert (current_namespace () == ^^N);
  namespace M
  {
    static_assert (foo () == ^^M);
    static_assert (current_namespace () == ^^M);
    consteval {
      static_assert (current_namespace () == ^^M);
      consteval {
	static_assert (current_namespace () == ^^M);
      }
    }
  }
}

struct T
{
  static_assert (current_namespace () == ^^::);
  static_assert (foo () == ^^::);
  info t = current_namespace ();
};

namespace O
{
  struct T
  {
    static_assert (current_namespace () == ^^O);
    static_assert (foo () == ^^O);
    info t = current_namespace ();
  };
}

void
bar ()
{
  static_assert (current_namespace () == ^^::);
  static_assert (T {}.t == ^^::);
  static_assert (O::T {}.t == ^^::);
  static_assert (foo (^^foo) == ^^foo);
  static_assert (foo () == ^^::);
}

namespace P
{
  void
  bar ()
  {
    static_assert (current_namespace () == ^^P);
    static_assert (T {}.t == ^^P);
    static_assert (O::T {}.t == ^^P);
    static_assert (foo (^^foo) == ^^foo);
    static_assert (foo () == ^^P);
  }
}

consteval {
  static_assert (current_namespace () == ^^::);
  consteval {
    static_assert (current_namespace () == ^^::);
  }
}

namespace Q
{
  int a[2];
  auto
  foo () -> int (&) [current_namespace () == ^^Q ? 2 : 3]
  {
    return a;
  }

  int bar ();
  int baz ();
}

typename [: current_namespace () == ^^:: ? ^^int : ^^:: :]
Q::bar ()
{
  return 0;
}

auto
Q::baz () -> typename [: current_namespace () == ^^Q ? ^^int : ^^:: :]
{
  return 0;
}
