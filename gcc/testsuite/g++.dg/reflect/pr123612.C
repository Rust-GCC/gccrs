// PR c++/123612
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

int a;
extern int b;
int foo (int x, int y, int z = 42);

consteval auto
bar ()
{
  extern int a;
  return ^^a;
}

consteval auto
baz ()
{
  extern int b;
  return ^^b;
}

consteval auto
qux ()
{
  static_assert (!has_default_argument (parameters_of (^^foo)[0]));
  static_assert (!has_default_argument (parameters_of (^^foo)[1]));
  static_assert (has_default_argument (parameters_of (^^foo)[2]));
  {
    extern int foo (int x, int y = 5, int z = 16);
    static_assert (!has_default_argument (parameters_of (^^foo)[0]));
    static_assert (has_default_argument (parameters_of (^^foo)[1]));
    static_assert (has_default_argument (parameters_of (^^foo)[2]));
    return ^^foo;
  }
}

static_assert (^^a == bar ());
static_assert (^^b == baz ());
static_assert (^^foo == qux ());
static_assert (!has_default_argument (parameters_of (^^foo)[0]));
static_assert (has_default_argument (parameters_of (^^foo)[1]));
static_assert (has_default_argument (parameters_of (^^foo)[2]));
