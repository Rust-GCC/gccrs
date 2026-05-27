// PR c++/107124
// { dg-do compile { target c++11 } }

template<int>
struct A {};

template<const int &I>
constexpr int f (A<I>) { return 0; }

template<typename T>
constexpr int f (T) { return 1; }

const int a = 42;
static_assert (f (A<0>{}) == 1, "");
static_assert (f (A<a>{}) == 1, "");
