// { dg-do compile }
// { dg-require-effective-target vect_int }
// { dg-additional-options "-std=gnu++20" }

#include <algorithm>
#include <array>
#include <ranges>

std::array<int, 16> foo(std::array<int, 16> u, std::array<int, 16> const &v)
{
  std::ranges::transform(u, v, u.begin(), std::plus<int>());
  return u;
}

// { dg-final { scan-tree-dump "optimized: loop vectorized" "vect" } }
// { dg-final { scan-tree-dump-not "Loop being analyzed as uncounted" "vect" } }
