// PR c++/125764
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <typename T>
struct S {};

static_assert(can_substitute(^^S, {^^int&()}));
