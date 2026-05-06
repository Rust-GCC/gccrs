// { dg-do compile { target c++26 } }

#include <utility>

using empty = std::integer_sequence<int>;
static_assert( std::is_same_v<std::tuple_element_t<0, empty>, int> ); // { dg-error "here" }
static_assert( std::is_same_v<std::tuple_element_t<0, const empty>, int> ); // { dg-error "here" }

using size4 = std::integer_sequence<int, 1, 9, 7, 15>;
static_assert( std::is_same_v<std::tuple_element_t<4, size4>, int> ); // { dg-error "here" }
static_assert( std::is_same_v<std::tuple_element_t<4, const size4>, int> ); // { dg-error "here" }

void
test()
{
  (void)std::get<0>(empty{}); // { dg-error "here" }
  (void)std::get<4>(size4{}); // { dg-error "here" }
}

// { dg-error "static assertion failed" "" { target *-*-* } 0 }
// { dg-error "cannot index an empty pack" "" { target *-*-* } 0 }
// { dg-error "pack index '.' is out of range for pack of length" "" { target *-*-* } 0 }
