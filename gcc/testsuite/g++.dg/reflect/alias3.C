// PR c++/125208
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using A = int;
using B = int;
using C = A;
static_assert (^^A == ^^A);
static_assert (^^const A == ^^const A);
static_assert (^^A != ^^const A);
static_assert (^^B == ^^B);
static_assert (^^C == ^^C);
static_assert (^^A != ^^B);
static_assert (^^A != ^^C);
static_assert (^^B != ^^C);
