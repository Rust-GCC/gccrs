// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

namespace N { }

template <auto V> constexpr int e = [:V:];  // { dg-error "expected a reflection of an expression" }
// { dg-message "but .N. is a namespace" "" { target *-*-* } 4 }
template <auto V> constexpr int e2 = [:V:]; // { dg-error "expected a reflection of an expression" }
constexpr auto h = ^^N;
constexpr auto i = e<([:^^h:])>;
constexpr auto j = e2<^^N>;
