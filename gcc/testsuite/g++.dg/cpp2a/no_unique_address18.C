// PR c++/125336
// { dg-do compile { target c++20 } }
// { dg-prune-output "used but never defined" }

struct S { int i; };
constexpr S& f (S&);
union W {
  [[no_unique_address]] S v;
};
struct R { W w; };
S s;
auto x = R{{ f(s) }};
