// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^::);
constexpr auto null = info{};
constexpr auto q = [:null:];  // { dg-error "cannot splice a null reflection" }
[:null:] t1;		      // { dg-error "cannot splice a null reflection|expected" }
[:null:]<int> t2;	      // { dg-error "cannot splice a null reflection|expected" }
