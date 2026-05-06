// PR c++/124706
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

using info = decltype(^^::);

template <class T> struct X;
template <> struct X<int> {
    constexpr X(int) {}
};
X(int) -> X<int>;

static constexpr auto template_info = ^^X;

[:template_info:] y{ 13 };

template<info R>
void
g ()
{
  typename [:R:] q{ 42 };
}

void
f ()
{
  g<template_info>();
}
