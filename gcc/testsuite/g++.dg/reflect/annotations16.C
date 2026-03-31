// CWG3145 - Uniqueness of annotations
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

template <int>
int x [[=1]];
static_assert (annotations_of (^^x<0>) != annotations_of (^^x<1>));
static_assert (annotations_of (^^x<42>) == annotations_of (^^x<42>));
