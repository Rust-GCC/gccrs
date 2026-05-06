// PR c++/123783
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

void
bar (int a)
{
}

void
foo (int a)
{
  auto b = &[: ^^a :];
  auto c = &[: variable_of (parameters_of (^^foo)[0]) :];
  auto d = &[: parameters_of (^^foo)[0] :];			// { dg-error "cannot use 'a' function parameter reflection in a splice expression" }
								// { dg-message "apply 'std::meta::variable_of' on it before splicing" "" { target *-*-* } .-1 }
  auto e = &[: parameters_of (^^bar)[0] :];			// { dg-error "cannot use 'a' function parameter reflection in a splice expression" }
  auto f = [] { auto g = &[: parameters_of (^^foo)[0] :]; };	// { dg-error "cannot use 'a' function parameter reflection in a splice expression" }
}
