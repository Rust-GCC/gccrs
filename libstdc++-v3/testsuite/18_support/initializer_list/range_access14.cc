// { dg-do compile { target c++14 } }

#include <initializer_list>
#include <iterator>

void
test01()
{
  static constexpr std::initializer_list<int> il{1, 2};
  static_assert( std::cbegin(il) == il.begin() );
  static_assert( std::cend(il) == il.end() );
#if __cplusplus >= 201703L
  static_assert( std::rbegin(il).base() == il.end() );
  static_assert( std::rend(il).base() == il.begin() );
  static_assert( std::rbegin(il).base() == il.end() );
  static_assert( std::rend(il).base() == il.begin() );
#endif 
  static_assert( noexcept(std::cbegin(il)) );
  static_assert( noexcept(std::cend(il)) );
  static_assert( noexcept(std::rbegin(il)) );
  static_assert( noexcept(std::rend(il)) );
  static_assert( noexcept(std::crbegin(il)) );
  static_assert( noexcept(std::crend(il)) );
}
