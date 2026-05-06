// { dg-do compile { target c++26 } }

#include <memory>

void
test01(int* ptr)
{
  (void)std::is_sufficiently_aligned<5>(ptr); // { dg-error "here" }
  (void)std::is_sufficiently_aligned<7>(ptr); // { dg-error "here" }
}

// { dg-error "static assertion failed" "" { target *-*-* } 0 }
