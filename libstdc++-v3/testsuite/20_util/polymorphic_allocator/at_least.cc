// { dg-do run { target c++23 } }

#include <memory_resource>
#include <testsuite_hooks.h>

void
alloc_at_least()
{
  struct A { char a; };
  using alloc_A = std::pmr::polymorphic_allocator<A>;
  using traits_A = std::allocator_traits<alloc_A>;
  alloc_A alloc_a;
  // This just forwards to alloc_a::allocate:
  std::allocation_result result = traits_A::allocate_at_least(alloc_a, 1);
  VERIFY(result.count == 1);
  VERIFY(result.ptr != nullptr);
  auto [pointer, count] = result;
  traits_A::deallocate(alloc_a, pointer, count);
}

int main()
{
  alloc_at_least();
}
