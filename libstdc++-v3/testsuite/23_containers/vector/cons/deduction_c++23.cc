// { dg-do compile { target c++23 } }

#include <vector>
#include <testsuite_iterators.h>

using Range = __gnu_test::test_input_range<int>;

template<typename Alloc>
concept can_deduce_vector = requires(Range r, Alloc a) {
  std::vector(std::from_range, r, a);
};

// Deduction should fail because int does not qualify as an allocator.
static_assert( ! can_deduce_vector<int> );
