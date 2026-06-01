// PR c++/125539
// { dg-do compile { target c++14 } }

struct A {};
struct B { int i; };
constexpr void foo () {}

template<typename T>
constexpr static A a1 = (foo (), A {});
template<typename T>
constexpr static A a2 = (A{}, A{});
template<typename T>
constexpr static A a3 = A{};
template<typename T>
constexpr static A a4 = {};
template<typename T>
constexpr static A a5{};
template<typename T>
constexpr static A a6 = (A{}, A{}, A{}, A{}, A{}, A{}, A{});
template<int N>
constexpr static B b1 = B{N};
template<int N>
constexpr static B b2 = (B{N}, B{N});

template const A a1<int>;
template const A a2<int>;
template const A a3<int>;
template const A a4<int>;
template const A a5<int>;
template const A a6<int>;
template const B b1<42>;
template const B b2<42>;
