// { dg-do run { target c++23 } }

#include <memory>
#include <stdlib.h>
#include <testsuite_hooks.h>

std::size_t gn = 0;
void* operator new(std::size_t n)
{
  gn = n;
  return ::malloc(n);
}

// Failing to define ops delete, too, would generate warnings.

void operator delete(void* p) noexcept
{ ::free(p); }

#if __cpp_sized_deallocation
void operator delete(void* p, std::size_t) noexcept
{ ::free(p); }
#endif

template <unsigned size, unsigned n>
void test_nm()
{
  struct A { char a[size]; };
  std::allocator<A> alloc;
  using alloc_traits = std::allocator_traits<std::allocator<A>>;
  auto [p, m] = alloc_traits::allocate_at_least(alloc, n);

#if __cpp_aligned_new
  unsigned mod = __STDCPP_DEFAULT_NEW_ALIGNMENT__;
  unsigned max = ((n * size) + mod - 1) & ~(mod - 1);
  unsigned count = max / sizeof(A);
#else
  unsigned count = n;
#endif
  VERIFY(m == count);
  VERIFY(gn == count * sizeof(A));
  VERIFY(p != nullptr);  // named it, use it.
}

void test()
{                  //  m  gn
  test_nm<1,3>();  // 16  16
  test_nm<2,3>();  //  8  16
  test_nm<3,3>();  //  5  15
  test_nm<4,3>();  //  4  16
  test_nm<5,3>();  //  3  15
  test_nm<6,3>();  //  5  30
  test_nm<7,3>();  //  4  28
  test_nm<8,3>();  //  4  32
  test_nm<9,3>();  //  3  27
  test_nm<10,3>(); //  3  30
  test_nm<11,3>(); //  4  44
  test_nm<12,3>(); //  4  48
  test_nm<13,3>(); //  3  39
  test_nm<14,3>(); //  3  42
  test_nm<15,3>(); //  3  45
}

int main()
{
  test();
}
