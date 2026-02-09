// PR c++/123984
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct A;
consteval
{
  define_aggregate (^^A, { data_member_spec (^^int, {.name{"_"}}) });
}

struct B : A {};

constexpr int
get (B &&d)
{
  constexpr auto ctx = std::meta::access_context::unchecked ();
  return static_cast <A*> (&d)->[: nonstatic_data_members_of (^^A, ctx)[0] :];
}

static_assert (get (B { 123 }) == 123);
