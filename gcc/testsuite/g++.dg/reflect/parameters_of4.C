// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test std::meta::parameters_of.

#include <meta>

template<auto R> void foo () {}
void
g ()
{
  foo<std::meta::parameters_of(^^g)[0]>();	// { dg-error "no matching function for call to" }
}
// { dg-error "call to non-'constexpr' function" "" { target *-*-* } 0 }
