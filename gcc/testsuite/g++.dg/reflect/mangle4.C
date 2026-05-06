// PR c++/124771
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <mdspan>
#include <meta>

namespace std {
template<random_access_iterator I>
  struct iterator_accessor {
    using element_type     = typename [: 
      [] { return ^^iter_value_t<I>; }() 
    :];

    constexpr iterator_accessor() noexcept = default;
    constexpr operator default_accessor<const element_type>() noexcept
      requires contiguous_iterator<I> && (!is_const_v<element_type>)
        { return {}; }
  };
}

int main() {
  // ??? It's not clear which of these should error.
  std::default_accessor<int>       _ = std::iterator_accessor<int*>();
  std::default_accessor<const int> _ = std::iterator_accessor<int*>(); // { dg-error "conversion" }
}
