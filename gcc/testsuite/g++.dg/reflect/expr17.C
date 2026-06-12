// PR c++/125759
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

consteval auto foo(auto&&... xs) { return (^^xs, ...); }
constexpr auto r1 = foo (0);
constexpr auto r2 = foo (0, 1u);
