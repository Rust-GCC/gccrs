// { dg-do compile { target c++11 } }

#include <iterator>

void
test01()
{
  (void)std::begin({1, 2, 3}); // { dg-error "no matching function for call" "" { target c++26 } }
  (void)std::end({1, 2, 3});   // { dg-error "no matching function for call" "" { target c++26 } }
}

// { dg-prune-output "cannot bind non-const lvalue reference of type" }
// { dg-prune-output "which is of non-class type" }
