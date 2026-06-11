// PR c++/125492
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct S {
  void memfn ();
};

constexpr auto a = std::meta::size_of(^^void()); // { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'reflection of function type in size_of'" }
constexpr auto b = std::meta::size_of(type_of(^^S::memfn)); // { dg-error "uncaught exception of type 'std::meta::exception'; 'what\\\(\\\)': 'reflection of function type in size_of'" }
