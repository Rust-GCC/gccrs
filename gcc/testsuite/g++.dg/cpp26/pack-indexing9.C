// P2662R3 - Pack Indexing
// PR c++/113798
// { dg-do compile { target c++26 } }
// From <https://github.com/itanium-cxx-abi/cxx-abi/issues/175>.

template <class... T> struct tuple {
  template <unsigned I> T...[I] get();
};

int
g ()
{
  tuple<int> t;
  // { dg-final { scan-assembler "_ZN5tupleIJiEE3getILj0EEEDyJiET_v" } }
  return t.get<0>();
}

template<typename T, typename U> concept C = true;
template<typename T, auto U> concept D = true;
template<typename ...T> struct A {
  template<int I, typename ...U> void f(T...[I], U...[I]) requires C<T...[I], U...[I]>;
  template<int I, auto ...U> void h(T...[I], decltype(U...[I])) requires D<T...[I], U...[I]>;
};

void
h ()
{
  A<char, int, double> a;
  // { dg-final { scan-assembler "_ZN1AIJcidEE1fILi1EJiicEEEvDyJcidET_DyT0_T_Q1CIDyT_TL0__DyTL0_0_TL0__E" } }
  a.f<1, int, int, char>(1, 2);
  // { dg-final { scan-assembler "_ZN1AIJcidEE1hILi1EJLi2ELl3ELj4EEEEvDyJcidET_DtsyT0_T_EQ1DIDyT_TL0__XsyTL0_0_TL0__EE" } }
  a.h<1, 2, 3L, 4U>(1, 3L);
}

template <auto... U> struct E {
  template <unsigned I> decltype (U...[I]) get();
};

int
i ()
{
  E<1, 2L, 3U, 4LL> t;
  // { dg-final { scan-assembler "_ZN1EIJLi1ELl2ELj3ELx4EEE3getILj2EEEDtsyJLi1ELl2ELj3ELx4EET_Ev" } }
  return t.get<2>();
}
