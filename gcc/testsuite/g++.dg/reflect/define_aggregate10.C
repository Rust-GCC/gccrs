// CWG3162 - Evaluation context of manifestly constant-evaluated expressions
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

struct S;

consteval std::size_t
foo (int p)
{
  constexpr std::size_t r = /* Q */ std::meta::is_complete_type (^^S) ? 1 : 2; // #1
  if (!std::meta::is_complete_type (^^S))  // #2
    std::meta::define_aggregate (^^S, {});
  return p > 0 ? foo (p - 1) : r;
}

consteval {
  if (foo (1) != 2)
    throw;  // OK, not evaluated
}
