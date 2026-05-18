// { dg-do compile { target c++26 } }

#include <initializer_list>
#include <iterator>

#ifndef __cpp_lib_initializer_list
# error "Feature-test macro for text_encoding missing in <initializer_list>"
#elif __cpp_lib_initializer_list != 202511L
# error "Feature-test macro for text_encoding has wrong value in <initializer_list>"
#endif

void
test02()
{
  static constexpr std::initializer_list<int> il{1};
  static_assert( il.data() == il.begin() );
  static_assert( il.empty() == false );
  static_assert( noexcept(il.data()) );
  static_assert( noexcept(il.empty()) );
}

