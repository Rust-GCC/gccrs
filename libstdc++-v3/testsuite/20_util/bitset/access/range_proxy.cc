// { dg-do run { target c++20 } }

#include <bitset>
#include <concepts>
#include <iterator>
#include <version>
#include <testsuite_hooks.h>

void
test_swap()
{
  std::bitset<2> v("01");
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

template<size_t N>
struct bitset_iterator
{
  using value_type = bool;
  using reference = std::bitset<2>::reference;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  bitset_iterator() = default;
  bitset_iterator(std::bitset<N>& c) noexcept : cont(&c) {}

  reference operator*() const noexcept
  { return (*cont)[pos]; }

  bitset_iterator& operator++() {
   ++pos;
   return *this;
  }

  bitset_iterator operator++(int) {
    auto tmp = *this;
    ++pos;
    return *this;
  };

  friend bool operator==(bitset_iterator, bitset_iterator) = default;

private:
  std::bitset<N>* cont = nullptr;
  size_t pos = 0;
};

void
test_iter_swap()
{
  std::bitset<2> v("01");
  auto it1 = bitset_iterator<2>(v);
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
static_assert( std::indirectly_writable<bitset_iterator<2>, bool> );
static_assert( std::indirectly_writable<bitset_iterator<2>,
					std::bitset<2>::reference> );

#else
static_assert( !std::indirectly_writable<bitset_iterator<2>, bool> );
static_assert( !std::indirectly_writable<bitset_iterator<2>,
					 std::bitset<2>::reference> );
#endif

int
main()
{
  test_swap();
  test_iter_swap();
}
