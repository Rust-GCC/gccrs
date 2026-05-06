// PR c++/124070
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct S;
template <std::meta::info I = define_aggregate (^^S, {})>	// { dg-error "'define_aggregate' not evaluated from 'consteval' block" }
constexpr void foo () {}

consteval {
  foo <> ();	// { dg-error "no matching function for call to 'foo<>\\\(\\\)'" }
}
