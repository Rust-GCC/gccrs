// { dg-do compile { target c++26 } }

#include <format>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <testsuite_hooks.h>

#if _GLIBCXX_USE_CXX11_ABI

#ifndef __glibcxx_constexpr_format
# error "Feature test macro for constexpr std::format is missing in <format>"
#elif __glibcxx_constexpr_format < 202511L
# error "Feature test macro for constexpr std::format has wrong value in <format>"
#endif

// Slightly more general from __format::_Widen, works with character literals.
template<typename CharT>
consteval auto widen(auto narrow, auto wide)
{
  if constexpr (std::is_same_v<CharT, wchar_t>)
    return wide;
  else
    return narrow;
}

#define WIDEN_(C, S) widen<C>(S, L##S)
#define WIDEN(S) WIDEN_(CharT, S)

template<typename CharT>
constexpr void
test_format()
{
  using namespace std;

  basic_string<CharT> res;

  res = format(WIDEN("{}"), WIDEN('c'));
  VERIFY( res == WIDEN("c") );
  res = format(WIDEN("{1} {0} {0}"), WIDEN('a'), WIDEN('b'));
  VERIFY( res == WIDEN("b a a") );
  res = format(WIDEN("{:?}"), WIDEN('\n'));
  VERIFY( res == WIDEN("'\\n'") );
  res = format(WIDEN("{:.^10}"), WIDEN("hello"));
  VERIFY( res == WIDEN("..hello...") );
  res = format(WIDEN("{:.>{}}"), WIDEN("world"), 8);
  VERIFY( res == WIDEN("...world") );
  res = format(WIDEN("{:+#06X}"), 0xa);
  VERIFY( res == WIDEN("+0X00A") );
  res = format(WIDEN("{:p}"), nullptr);
  VERIFY( res == WIDEN("0x0") );
  res = format(WIDEN("{:07P}"), nullptr);
  VERIFY( res == WIDEN("0X00000") );
  res = format(WIDEN("{} {}"), true, false);
  VERIFY( res == WIDEN("true false") );
  res = format(WIDEN("{:+#06b}"), true);
  VERIFY( res == WIDEN("+0b001") );
  res = format(WIDEN("{} {} {}"), WIDEN("abc"), basic_string_view<CharT>(WIDEN("def")), basic_string<CharT>(WIDEN("ghi")));
  VERIFY( res == WIDEN("abc def ghi") );
  res = format(WIDEN("{:?}"), WIDEN("hello\nworld"));
  VERIFY( res == WIDEN("\"hello\\nworld\"") );
  res = format(WIDEN("{}"), tuple(1, true));
  VERIFY( res == WIDEN("(1, true)") );
  res = format(WIDEN("{:t<12m}"), tuple(WIDEN('a'), WIDEN("bc")));
  VERIFY( res == WIDEN("'a': \"bc\"ttt") );
  res = format(WIDEN("{:n}"), tuple(nullptr, -1, 1));
  VERIFY( res == WIDEN("0x0, -1, 1") );
  res = format(WIDEN("{}"), vector{1, 2, 3, 4});
  VERIFY( res == WIDEN("[1, 2, 3, 4]") );
  res = format(WIDEN("{:?s}"), vector{WIDEN('a'), WIDEN('\n'), WIDEN('b')});
  VERIFY( res == WIDEN("\"a\\nb\"") );
  res = format(WIDEN("{:n:+}"), vector{1, 2, 3});
  VERIFY( res == WIDEN("+1, +2, +3") );
}

template<typename CharT>
constexpr void
test_format_to()
{
  using namespace std;

  CharT buf[100];
  CharT* out;

  out = format_to(buf, WIDEN("{:.^5}"), WIDEN("foo"));
  VERIFY( basic_string_view<CharT>(buf, out) == WIDEN(".foo.") );
  out = format_to(buf, WIDEN("{} {}"), nullptr, true);
  VERIFY( basic_string_view<CharT>(buf, out) == WIDEN("0x0 true") );
}

template<typename CharT>
constexpr void
test_vformat()
{
  using namespace std;
  using context = __format::__format_context<CharT>;

  basic_string<CharT> res;

  int arg1 = 1;
  CharT arg2 = WIDEN('a');
  bool arg3 = true;
  res = vformat(WIDEN("{} {:?} {}"), make_format_args<context>(arg1, arg2, arg3));
  VERIFY( res == WIDEN("1 'a' true") );
}

template<typename CharT>
constexpr void
test_vformat_to()
{
  using namespace std;
  using context = __format::__format_context<CharT>;

  CharT buf[100];
  CharT* out;

  nullptr_t arg1 = nullptr;
  basic_string<CharT> arg2 = WIDEN("foo");
  tuple<int, int> arg3{-3, 5};
  out = vformat_to(buf, WIDEN("{} {:?} {}"), make_format_args<context>(arg1, arg2, arg3));
  VERIFY( basic_string_view<CharT>(buf, out) == WIDEN("0x0 \"foo\" (-3, 5)") );
}

template<typename CharT>
constexpr void
test_format_to_n()
{
  using namespace std;

  CharT buf[100];
  format_to_n_result<CharT*> out;
  int n;

  n = 100;
  out = format_to_n(buf, n, WIDEN("{:+} {:?} {}"), 1, WIDEN("\n\n"), vector{1, 2, 3});
  VERIFY( out.size <= n );
  VERIFY( out.out - buf == out.size );
  VERIFY( basic_string_view<CharT>(buf, out.size) == WIDEN("+1 \"\\n\\n\" [1, 2, 3]") );
  n = 12;
  out = format_to_n(buf, n, WIDEN("{} {} {}"), true, nullptr, WIDEN("long string"));
  VERIFY( out.size > n );
  VERIFY( out.out - buf == n );
  VERIFY( basic_string_view<CharT>(buf, out.out) == WIDEN("true 0x0 lon") );
}

constexpr bool
all_tests()
{
  test_format<char>();
  test_format<wchar_t>();
  test_format_to<char>();
  test_format_to<wchar_t>();
  test_vformat<char>();
  test_vformat<wchar_t>();
  test_vformat_to<char>();
  test_vformat_to<wchar_t>();
  test_format_to_n<char>();
  test_format_to_n<wchar_t>();

  return true;
}

static_assert(all_tests());

#endif // _GLIBCXX_USE_CXX11_ABI
