// { dg-do run { target c++26 } }

#include <format>
#include <testsuite_hooks.h>

#ifdef __glibcxx_constexpr_format
# define CONSTEXPR constexpr
#else
# define CONSTEXPR
#endif

CONSTEXPR void
test_char()
{
  std::string fmt = "{}";
  auto s = std::format(std::dynamic_format(fmt), 123);
  VERIFY( s == "123" );
}

CONSTEXPR void
test_wchar()
{
  std::wstring fmt = L"{:#o}";
  auto s = std::format(std::dynamic_format(fmt), 456);
  VERIFY( s == L"0710" );
}

CONSTEXPR void
test_internal_api()
{
  // Using _Dynamic_format_string directly works even in C++20 mode.
  // This can be used internally by libstdc++.
  std::string fmt = "{:#x}";
  auto s = std::format(std::__format::_Dynamic_format_string<char>(fmt), 789);
  VERIFY( s == "0x315" );
}

static_assert( noexcept(std::format_string<>(std::dynamic_format(""))) );
static_assert( noexcept(std::wformat_string<>(std::dynamic_format(L""))) );
static_assert( noexcept(std::format_string<int>(std::dynamic_format(""))) );
static_assert( noexcept(std::wformat_string<char>(std::dynamic_format(L""))) );
// A format string can be constructed from the result of std::dynamic_format
// using copy elision, but cannot be constructed from an xvalue.
static_assert( !std::is_constructible_v<std::format_string<>,
					decltype(std::dynamic_format(""))&&> );
static_assert( !std::is_constructible_v<std::wformat_string<>,
					decltype(std::dynamic_format(L""))&&> );

CONSTEXPR bool
test_all()
{
  test_char();
  test_wchar();
  test_internal_api();
  return true;
}

#ifdef __glibcxx_constexpr_format
static_assert(test_all());
#endif

int main()
{
  test_all();
}
