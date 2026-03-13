// PR c++/124493
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

template<typename T> void Y(T);	// { dg-message "but .Y. is a function template" }
template<decltype(^^::) R>
constexpr auto f () -> [:R:]<0> { return {}; }	// { dg-error "expected a reflection of a type template" }
constexpr auto a = f<^^Y>(); // { dg-error "no matching function" }
