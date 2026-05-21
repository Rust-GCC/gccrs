// PR c++/125422
// { dg-do compile }
// { dg-options "-Wduplicated-branches" }

// new (nothrow) T[n] with a runtime variable must not trigger
// -Wduplicated-branches (false positive from compiler-generated COND_EXPR).

#include <new>

void
test_nothrow (int sz)
{
  char *p = new (std::nothrow) char[sz];
  delete[] p;
}
