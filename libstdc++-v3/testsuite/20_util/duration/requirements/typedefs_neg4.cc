// { dg-do compile { target c++11 } }

#include <chrono>

const int i = 10;
// Check if rep is a cv-unqualifed object type
std::chrono::duration<const int> d1(i); // { dg-error "required from here" }
std::chrono::duration<volatile int> d2(i); // { dg-error "required from here" }
std::chrono::duration<const int&> d3(i); // { dg-error "required from here" }

// { dg-error "rep should be cv-unqualified object type" "" { target *-*-* } 0 }
