// PR c++/123612
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
using namespace std::meta;

int a;
extern int b;
int foo (int x, int y, int z = 42);

consteval auto
bar ()
{
  extern int a;
  return ^^a;	// { dg-error "cannot take the reflection of a block-scope extern .a." }
}

consteval auto
baz ()
{
  extern int b;
  return ^^b;	// { dg-error "cannot take the reflection of a block-scope extern .b." }
}

consteval auto
qux ()
{
  static_assert (!has_default_argument (parameters_of (^^foo)[0]));
  static_assert (!has_default_argument (parameters_of (^^foo)[1]));
  static_assert (has_default_argument (parameters_of (^^foo)[2]));
  {
    extern int foo (int x, int y = 5, int z = 16);
    static_assert (!has_default_argument (parameters_of (^^foo)[0]));  // { dg-error "cannot take the reflection of a block-scope extern .foo." }
    static_assert (has_default_argument (parameters_of (^^foo)[1]));  // { dg-error "cannot take the reflection of a block-scope extern .foo." }
    static_assert (has_default_argument (parameters_of (^^foo)[2]));  // { dg-error "cannot take the reflection of a block-scope extern .foo." }
    return ^^foo;  // { dg-error "cannot take the reflection of a block-scope extern .foo." }
  }
}

static_assert (^^a == bar ());
static_assert (^^b == baz ());
static_assert (^^foo == qux ());
static_assert (!has_default_argument (parameters_of (^^foo)[0]));
static_assert (has_default_argument (parameters_of (^^foo)[1]));
static_assert (has_default_argument (parameters_of (^^foo)[2]));
