// PR c++/123613
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <typename T, auto ... Vs>
constexpr auto construct_from = T{Vs...};

struct Inner {};
struct Outer { Inner m; };

constexpr auto r = substitute(^^construct_from,
                             { ^^Outer, ^^construct_from<Inner> });
