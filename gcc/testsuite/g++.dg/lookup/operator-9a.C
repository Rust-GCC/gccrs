// PR c++/125378
// A version of operator-9.C where the operator<=> returns char instead of int.

// { dg-do compile { target c++20 } }

#include <compare>

struct A {
  bool operator==(int);
  char operator<=>(int);
};

template<class T>
void f() {
  A a;
  (void)(0 <=> a);
}

template void f<int>();
