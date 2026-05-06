// PR c++/124944
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

namespace D {
  struct E {};
}
constexpr auto y = ^^D;

struct F : [: y :]::E {
  F () : typename [: y :]::E () {}	// { dg-error "keyword 'typename' not allowed in this context \\\(a qualified member initializer is implicitly a type\\\)" }
};
