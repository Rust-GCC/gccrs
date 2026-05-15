// PR c++/125315
// { dg-do compile { target c++20 } }
// { dg-prune-output "used but never defined" }

struct S{~S(){}};
constexpr S& f(S& t);
struct W{[[no_unique_address]]S v;};
struct R:W{};
S s;
auto x=R{{f(s)}};
