// { dg-do run { target c++26 } }

#include <inplace_vector>

#include <ranges>
#include <span>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>
#include <testsuite_allocator.h>

template<typename T, typename V, size_t N>
constexpr bool
eq(const std::inplace_vector<V, N>& l, std::span<const T> r) {
  if (l.size() != r.size())
    return false;
  for (auto i = 0u; i < l.size(); ++i)
    if (l[i] != r[i])
      return false;
  return true;
};

template<typename T, template<class TT> class ItType>
constexpr void
do_test_it()
{
  // The vector's value_type.
  using V = int;

  T a[]{1,2,3,4,5,6,7,8,9};
  using It = ItType<T>;

  auto bounds = typename It::ContainerType(a, a+9);
  std::inplace_vector<V, 0> e0(It(a, &bounds), It(a, &bounds));
  VERIFY( e0.empty() );

  bounds = typename It::ContainerType(a, a+9);
  std::inplace_vector<V, 10> v0(It(a, &bounds), It(a, &bounds));
  VERIFY( v0.empty() );

  bounds = typename It::ContainerType(a, a+9);
  std::inplace_vector<V, 10> v4(It(a, &bounds), It(a+4, &bounds));
  VERIFY( eq<T>(v4, {a, 4}) );

#ifdef __cpp_exceptions
#ifndef __cpp_lib_constexpr_exceptions
  if consteval {
    return;
  }
#endif

  bounds = typename It::ContainerType(a, a+9);
  try
  {
    std::inplace_vector<int, 5> v9(It(a, &bounds), It(a+9, &bounds));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }

  bounds = typename It::ContainerType(a, a+9);
  try
  {
    std::inplace_vector<int, 0> v2(It(a, &bounds), It(a+2, &bounds));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }
#endif
}

constexpr bool
test_iterators()
{
  using namespace __gnu_test;

  do_test_it<int, input_iterator_wrapper>();
  do_test_it<int, forward_iterator_wrapper>();
  do_test_it<int, random_access_iterator_wrapper>();

  do_test_it<short, forward_iterator_wrapper>();
  return true;
}

template<typename Range>
constexpr void
do_test_r()
{
  // The vector's value_type.
  using V = int;

  // The range's value_type.
  using T = std::ranges::range_value_t<Range>;
  T a[]{1,2,3,4,5,6,7,8,9};

  std::inplace_vector<V, 0> e0(std::from_range, Range(a, a+0));
  VERIFY( e0.empty() );

  std::inplace_vector<V, 10> v0(std::from_range, Range(a, a+0));
  VERIFY( v0.empty() );

  std::inplace_vector<V, 10> v4(std::from_range, Range(a, a+4));
  VERIFY( eq<T>(v4, {a, 4}) );

#ifdef __cpp_exceptions
#ifndef __cpp_lib_constexpr_exceptions
  if consteval {
    return;
  }
#endif

  try
  {
    std::inplace_vector<V, 5> v9(std::from_range, Range(a, a+9));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }

  try
  {
    std::inplace_vector<V, 0> v3(std::from_range, Range(a, a+3));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }
#endif
}

constexpr bool
test_ranges()
{
  using namespace __gnu_test;

  do_test_r<test_forward_range<int>>();
  do_test_r<test_sized_range_sized_sent<int, forward_iterator_wrapper>>();

  do_test_r<test_input_range<int>>();
  do_test_r<test_input_sized_range<int>>();
  do_test_r<test_sized_range_sized_sent<int, input_iterator_wrapper>>();

  do_test_r<test_range<int, input_iterator_wrapper_nocopy>>();
  do_test_r<test_sized_range<int, input_iterator_wrapper_nocopy>>();
  do_test_r<test_sized_range_sized_sent<int, input_iterator_wrapper_nocopy>>();

  do_test_r<test_forward_range<short>>();
  do_test_r<test_input_range<short>>();

  // Not lvalue-convertible to int
  struct C {
    constexpr C(int v) : val(v) { }
    constexpr operator int() && { return val; }
    constexpr bool operator==(int b) const { return b == val; }
    int val;
  };
  using rvalue_input_range = test_range<C, input_iterator_wrapper_rval>;
  do_test_r<rvalue_input_range>();

  return true;
}

template<typename T>
constexpr void
test_iota()
{
  T a[]{1,2,3,4,5,6,7,8,9,10};

  std::inplace_vector<T, 10> v1(
    std::from_range, std::views::iota(T(1), T(11)));
  VERIFY( eq<T>(v1, {a, a+10}) );

  std::inplace_vector<T, 10> v2(
    std::from_range,
    std::views::iota(T(1))
    | std::views::as_input
    | std::views::take_while([](T t) { return t <= 10; }));
  VERIFY( eq<T>(v2, {a, a+10}) );

#ifdef __cpp_exceptions
#ifndef __cpp_lib_constexpr_exceptions
  if consteval {
    return;
  }
#endif
  constexpr T max = std::numeric_limits<T>::max();
  try
  {
    std::inplace_vector<T, 10> v(
      std::from_range, std::views::iota(T(0), max));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }

  try
  {
    std::inplace_vector<T, 10> v(
      std::from_range,
      std::views::iota(T(0))
      | std::views::as_input
      | std::views::take_while([](T t) { return t < 15; }));
    VERIFY(false);
  }
  catch (std::bad_alloc const&)
  {
  }
#endif
}

int main()
{
  auto test_all = [] {
    test_iterators();
    test_ranges();

    test_iota<long long>();
#ifdef __SIZEOF_INT128__
    test_iota<__int128>();
#endif
    return true;
  };

  test_all();
  static_assert( test_all() );
}

