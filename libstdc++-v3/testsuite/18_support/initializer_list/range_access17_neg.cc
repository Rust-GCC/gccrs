// { dg-do compile { target c++17 } }

#include <iterator>

void
test01()
{
  (void)std::data({1, 2, 3});  // { dg-error "no matching function for call" "" { target c++26 } }
  (void)std::size({1, 2, 3});  // uses array overload
  (void)std::empty({1, 2, 3}); // uses array overload
}

// { dg-prune-output "cannot bind non-const lvalue reference of type" }
// { dg-prune-output "which is of non-class type" }
