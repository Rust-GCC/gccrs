// PR c++/124493
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template<typename T>
constexpr T Y{};	// { dg-message "but .Y<T>. is a variable template" }
template<decltype(^^::) R>
constexpr auto f () -> [:R:]<0> { return {}; }	// { dg-error "expected a reflection of a type template" }
constexpr auto a = f<^^Y>(); // { dg-error "no matching function" }
