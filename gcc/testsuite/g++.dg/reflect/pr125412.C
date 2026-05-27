// PR c++/125412
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

void
foo ()
{
}

auto bar ();

template <typename T>
auto a = foo ();	// { dg-error "variable or field 'a' declared void" }
template <typename T>
auto b = bar ();	// { dg-error "use of 'auto bar\\\(\\\)' before deduction of 'auto'" }

consteval {
  auto c = substitute (^^a, { ^^int });
  std::meta::extract <int> (c);
  auto d = substitute (^^b, { ^^int });
  std::meta::extract <int> (d);
}
