// { dg-do compile { target c++23 } }

#include <generator>
#include <vector>

std::generator<std::span<int>> f() {
  std::vector<int> v;
  co_yield v;
}

std::generator<std::span<int>> g() {
  std::vector<std::vector<int>> v;
  co_yield std::ranges::elements_of(v);
}
