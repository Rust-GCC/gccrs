// PR c++/124493
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

struct Y { };
template<decltype(^^::) R>
constexpr auto f (typename [:R:]<0> x) { return x; }  // { dg-error "expected a reflection of a type template" }
constexpr auto a = f<^^Y>(Y{}); // { dg-error "no matching function for call" }
