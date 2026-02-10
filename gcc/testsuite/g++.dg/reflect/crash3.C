// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template <auto V> constexpr int e = [:V:];  // { dg-error "expected a reflection of an expression" }
// { dg-message "but .int. is a type" "" { target *-*-* } .-1 }
template <auto V> constexpr int e2 = [:V:]; // { dg-error "expected a reflection of an expression" }
// { dg-message "but .int. is a type" "" { target *-*-* } .-1 }
constexpr auto h = ^^int;
constexpr auto i = e<([:^^h:])>;
constexpr auto j = e2<^^int>;
