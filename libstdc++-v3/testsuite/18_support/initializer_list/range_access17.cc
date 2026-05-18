// { dg-do compile { target c++17 } }

#include <initializer_list>
#include <iterator>

void
test01()
{
  static constexpr std::initializer_list<int> il{1};
  static_assert( std::data(il) == il.begin() );
  static_assert( std::size(il) == il.size() );
  static_assert( !std::empty(il) );
  static_assert( noexcept(std::data(il)) );
  static_assert( noexcept(std::size(il)) );
  static_assert( noexcept(std::empty(il)) );
}
