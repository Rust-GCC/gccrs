// PR c++/124842
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>

inline constexpr int x = 1;
template <auto r>
typename[: std::meta::type_of(r) :] f() {
  return 1;
}

int
f1 ()
{
  return f<^^x>();
// { dg-final { scan-assembler "_Z1fITnDaLDmvr1xEEDScl7type_ofT_EEv" } }
}

int
f2 ()
{
  return f<std::meta::reflect_constant(1)>();
// { dg-final { scan-assembler "_Z1fITnDaLDmvlLi1EEEDScl7type_ofT_EEv" } }
}
