// { dg-do compile { target c++17 } }

#include <iterator>

void
test01()
{
  (void)std::cbegin({1, 2, 3});  // { dg-error "no matching function for call" }
  (void)std::cend({1, 2, 3});    // { dg-error "no matching function for call" }
  (void)std::rbegin({1, 2, 3});  // initializer_list overload not removed
  (void)std::rend({1, 2, 3});    // initializer_list overload not removed
  (void)std::crbegin({1, 2, 3}); // { dg-error "no matching function for call" }
  (void)std::crend({1, 2, 3});   // { dg-error "no matching function for call" }

}

// { dg-prune-output "cannot bind non-const lvalue reference of type" }
// { dg-prune-output "which is of non-class type" }
