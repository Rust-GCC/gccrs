// { dg-do compile { target c++26 } }

#include <utility>
#include <tuple>
#include <testsuite_hooks.h>

#if __cpp_lib_integer_sequence < 202511L
# error "Feature-test macro __cpp_lib_integer_sequence is incorrect"
#endif

constexpr auto
destructure_sum(auto seq)
{
  auto [...elems] = seq;
  return (0 + ... + elems);
}

using IS1 = std::make_index_sequence<10>;
static_assert( std::tuple_size_v<IS1> == 10 );
static_assert( std::is_same_v<std::tuple_element_t<3, IS1>, std::size_t> );
static_assert( std::get<7>(IS1{}) == 7 );
static_assert( destructure_sum(IS1{}) == 45 );
static_assert( noexcept(get<0>(IS1{})) );

using IS2 = std::integer_sequence<int, 42, 101, -13>;
static_assert( std::tuple_size_v<IS2> == 3 );
static_assert( std::is_same_v<std::tuple_element_t<1, IS2>, int> );
static_assert( std::get<2>(IS2{}) == -13 );
static_assert( destructure_sum(IS2{}) == 130 );
static_assert( noexcept(get<0>(IS2{})) );

using IS3 = std::integer_sequence<char>;
static_assert( std::tuple_size_v<IS3> == 0 );

using IS4 = std::integer_sequence<int, 1, 2>;
static_assert( !std::is_constructible_v<std::pair<int, int>, IS4> );
static_assert( !std::is_constructible_v<std::tuple<int, int>, IS4> );

template<typename = void>
constexpr bool
test_basic()
{
  {
    auto [...elems] = std::make_index_sequence<0>{};
    static_assert( sizeof...(elems) == 0 );
  }

  {
    auto [...elems] = std::integer_sequence<int, 3, 5, 7, 11>{};

    static_assert( sizeof...(elems) == 4 );

    VERIFY( elems...[0] == 3 );
    VERIFY( elems...[1] == 5 );
    VERIFY( elems...[2] == 7 );
    VERIFY( elems...[3] == 11 );
  }

  {
    static constexpr auto [...elems] = std::integer_sequence<short, 2, 4, 8, 16>{};

    static_assert( sizeof...(elems) == 4 );

    static_assert( elems...[0] == 2 );
    static_assert( elems...[1] == 4 );
    static_assert( elems...[2] == 8 );
    static_assert( elems...[3] == 16 );
  }

  {
    constexpr auto [...elems] = std::integer_sequence<int, 1, 8, 2, 11>{};

    static_assert( sizeof...(elems) == 4 );

    static_assert( elems...[0] == 1 );
    static_assert( elems...[1] == 8 );
    static_assert( elems...[2] == 2 );
    static_assert( elems...[3] == 11 );
  }

  {
    static constexpr auto&& [...elems] = std::integer_sequence<short, 2, 4, 8, 16>{};

    static_assert( sizeof...(elems) == 4 );

    static_assert( elems...[0] == 2 );
    static_assert( elems...[1] == 4 );
    static_assert( elems...[2] == 8 );
    static_assert( elems...[3] == 16 );
  }

  /* Unimplemented, see PR117784
  {
    constexpr auto&& [...elems] = std::integer_sequence<int, 1, 8, 2, 11>{};

    static_assert( sizeof...(elems) == 4 );

    static_assert( elems...[0] == 1 );
    static_assert( elems...[1] == 8 );
    static_assert( elems...[2] == 2 );
    static_assert( elems...[3] == 11 );
  }
  */

  {
    auto idx = 0;
    static constexpr auto seq = std::make_index_sequence<4>{};
    template for (constexpr auto elem : seq)
      {
	VERIFY( elem == idx );
	++idx;
      }
    VERIFY( idx == 4 );
  }


  /* Unimplemented, see PR117784
  {
    auto idx = 0;
    constexpr auto seq = std::make_index_sequence<4>{};
    template for (constexpr auto elem : seq)
      {
	VERIFY( elem == idx );
	++idx;
      }
    VERIFY( idx == 4 );
  }
   
  {
    auto idx = 0;
    template for (constexpr auto elem : std::make_index_sequence<4>{})
      {
	VERIFY( elem == idx );
	++idx;
      }
    VERIFY( idx == 4 );
  }
  */

  return true;
}

static_assert( test_basic() );
