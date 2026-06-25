// { dg-do run { target c++20 } }

#include <vector>
#include <concepts>
#include <iterator>
#include <version>
#include <testsuite_hooks.h>

void
test_swap()
{
  std::vector<bool> v{true, false};
  std::ranges::swap(v[0], v[1]);
  VERIFY( v[0] == false );
  VERIFY( v[1] == true );

  bool b = true;
  std::ranges::swap(v[0], b);
  VERIFY( v[0] == true );
  VERIFY( b == false );

  std::ranges::swap(b, v[1]);
  VERIFY( v[1] == false );
  VERIFY( b == true );
}

void
test_iter_swap()
{
  std::vector<bool> v{true, false};
  auto it1 = v.begin();
  auto it2 = std::ranges::next(it1);
  std::ranges::iter_swap(it1, it2);
  VERIFY( v[0] == false );
  VERIFY( v[1] == true );

  bool b = true, *p = &b;
  std::ranges::iter_swap(it1, p);
  VERIFY( v[0] == true );
  VERIFY( b == false );

  std::ranges::iter_swap(p, it2);
  VERIFY( v[1] == false );
  VERIFY( b == true );
}

#if __cpp_lib_ranges_zip
static_assert( std::indirectly_writable<std::vector<bool>::iterator, bool> );
static_assert( std::indirectly_writable<std::vector<bool>::iterator,
					std::vector<bool>::reference> );

#else
static_assert( !std::indirectly_writable<std::vector<bool>::iterator, bool> );
static_assert( !std::indirectly_writable<std::vector<bool>::iterator,
					 std::vector<bool>::reference> );
#endif

int
main()
{
  test_swap();
  test_iter_swap();
}
