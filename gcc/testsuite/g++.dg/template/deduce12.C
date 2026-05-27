// PR c++/107124
// { dg-do compile { target c++11 } }

template<int>
struct A {};

template<const int &I>
constexpr int f (A<I>) { return 0; }

constexpr int i = f (A<0>{}); // { dg-error "no matching function for call" }
// { dg-message "(candidate|does not bind directly|could not convert)" "candidate note" { target c++17 } .-1 }
// { dg-message "(candidate|not a valid template argument)" "candidate note" { target c++14_down } .-2 }
