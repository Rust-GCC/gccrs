// PR c++/124792
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct S { const char *name = nullptr; };

constexpr auto a = std::define_static_array (std::vector { S { std::define_static_string ("foo") } });
constexpr auto b = std::define_static_array (std::vector { S { std::define_static_string ("foo") },
							   S { std::define_static_string ("bar") } });
