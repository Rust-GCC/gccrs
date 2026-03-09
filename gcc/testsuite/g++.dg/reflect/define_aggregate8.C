// PR c++/124070
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct S;
consteval void bar (std::meta::info a = define_aggregate (^^S, {})) {}
consteval {
  bar ();
}
S s;
