// { dg-options "-O1 -fdump-tree-optimized" }
// { dg-do compile { target c++17 } }
// { dg-final { scan-tree-dump "memcpy" "optimized" } }

// PR libstdc++/121789
// std::uninitialized_move_n() and friends don't optimize to memcpy

#include <memory>

struct T { int x; };

void f(T* src, T* dst, unsigned n)
{
  std::uninitialized_move(src, src + n, dst);
}

void g(T* src, T* dst, unsigned n)
{
  std::uninitialized_move_n(src, n, dst);
}
