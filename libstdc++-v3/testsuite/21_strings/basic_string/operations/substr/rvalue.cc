// { dg-do run { target c++26 } }

#include <string_view>
#include <testsuite_hooks.h>
#include <testsuite_allocator.h>

// When selection is short enough to fit into SSO, the rhs
// is left unchanged.
template<typename CharT, typename Allocator>
constexpr void
do_test_short_in(const CharT* cstr, Allocator alloc)
{
  using StringView = std::basic_string_view<CharT>;
  using String = std::basic_string<CharT, std::char_traits<CharT>, Allocator>;

  const Allocator defAlloc;
  String src, res;

  // substr(0)
  src = String(cstr, alloc);
  res = std::move(src).substr(0);
  VERIFY( res == StringView(cstr).substr(0) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res1(std::move(src), 0);
  VERIFY( res1 == StringView(cstr).substr(0) );
  VERIFY( res1.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res2(std::move(src), 0, alloc);
  VERIFY( res2 == StringView(cstr).substr(0) );
  VERIFY( res2.get_allocator() == alloc );
  VERIFY( src == cstr );

  // substr(1)
  src = String(cstr, alloc);
  res = std::move(src).substr(1);
  VERIFY( res == StringView(cstr).substr(1) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res3(std::move(src), 1);
  VERIFY( res3 == StringView(cstr).substr(1) );
  VERIFY( res3.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res4(std::move(src), 1, alloc);
  VERIFY( res4 == StringView(cstr).substr(1) );
  VERIFY( res4.get_allocator() == alloc );
  VERIFY( src == cstr );

  // substr(1, 1000)
  src = String(cstr, alloc);
  res = std::move(src).substr(1, 1000);
  VERIFY( res == StringView(cstr).substr(1, 1000) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res5(std::move(src), 1, 1000);
  VERIFY( res5 == StringView(cstr).substr(1, 1000) );
  VERIFY( res5.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res6(std::move(src), 1, 1000, alloc);
  VERIFY( res6 == StringView(cstr).substr(1, 1000) );
  VERIFY( res6.get_allocator() == alloc );
  VERIFY( src == cstr );
}

template<typename CharT, typename Allocator>
constexpr void
do_test_short_sel(const CharT* cstr, Allocator alloc)
{
  using StringView = std::basic_string_view<CharT>;
  using String = std::basic_string<CharT, std::char_traits<CharT>, Allocator>;

  const Allocator defAlloc;
  String src, res;

  // substr(0, 2)
  src = String(cstr, alloc);
  res = std::move(src).substr(0, 2);
  VERIFY( res == StringView(cstr).substr(0, 2) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res1(std::move(src), 0, 2);
  VERIFY( res1 == StringView(cstr).substr(0, 2) );
  VERIFY( res1.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res2(std::move(src), 0, 2, alloc);
  VERIFY( res2 == StringView(cstr).substr(0, 2) );
  VERIFY( res2.get_allocator() == alloc );
  VERIFY( src == cstr );

  // substr(1, 2)
  src = String(cstr, alloc);
  res = std::move(src).substr(1, 2);
  VERIFY( res == StringView(cstr).substr(1, 2) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res3(std::move(src), 1, 2);
  VERIFY( res3 == StringView(cstr).substr(1, 2) );
  VERIFY( res3.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res4(std::move(src), 1, 2, alloc);
  VERIFY( res4 == StringView(cstr).substr(1, 2) );
  VERIFY( res4.get_allocator() == alloc );
  VERIFY( src == cstr );

  // libstdc++ specific
  constexpr int max_short = (sizeof(CharT) == 1) ? 15 : 3;
  // substr(0, max_short)
  src = String(cstr, alloc);
  res = std::move(src).substr(0, max_short);
  VERIFY( res == StringView(cstr).substr(0, max_short) );
  VERIFY( res.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res5(std::move(src), 0, max_short);
  VERIFY( res5 == StringView(cstr).substr(0, max_short) );
  VERIFY( res5.get_allocator() == defAlloc );
  VERIFY( src == cstr );

  src = String(cstr, alloc);
  String res6(std::move(src), 0, max_short, alloc);
  VERIFY( res6 == StringView(cstr).substr(0, max_short) );
  VERIFY( res6.get_allocator() == alloc );
  VERIFY( src == cstr );
}

// If the selection is long enough to do not fit into SSO,
// the memory is moved if allocators are compatible.
template<typename CharT, typename Allocator>
constexpr void
do_test_long(const CharT* cstr, Allocator alloc)
{
  using StringView = std::basic_string_view<CharT>;
  using String = std::basic_string<CharT, std::char_traits<CharT>, Allocator>;

  const Allocator defAlloc;
  String src, res;
  const CharT* data;

  auto verifyMove = [&](const String& str)
  {
    // Only SSO string provide rvalue overloads of
    // substr and corresponding constructor.
#if _GLIBCXX_USE_CXX11_ABI || !_GLIBCXX_USE_DUAL_ABI
    if (str.get_allocator() == alloc) {
      VERIFY( str.data() == data );
      VERIFY( src.empty() );
    } else
#endif
      VERIFY( src == cstr );
  };

  // substr(0)
  src = String(cstr, alloc);
  data = src.data();
  res = std::move(src).substr(0);
  VERIFY( res == StringView(cstr).substr(0) );
  VERIFY( res.get_allocator() == defAlloc );
  verifyMove(res);

  src = String(cstr, alloc);
  data = src.data();
  String res1(std::move(src), 0);
  VERIFY( res1 == StringView(cstr).substr(0) );
  VERIFY( res1.get_allocator() == defAlloc );
  verifyMove(res1);

  src = String(cstr, alloc);
  data = src.data();
  String res2(std::move(src), 0, alloc);
  VERIFY( res2 == StringView(cstr).substr(0) );
  VERIFY( res2.get_allocator() == alloc );
  verifyMove(res2);

  // substr(5)
  src = String(cstr, alloc);
  data = src.data();
  res = std::move(src).substr(5);
  VERIFY( res == StringView(cstr).substr(5) );
  VERIFY( res.get_allocator() == defAlloc );
  verifyMove(res);

  src = String(cstr, alloc);
  data = src.data();
  String res3(std::move(src), 5);
  VERIFY( res3 == StringView(cstr).substr(5) );
  VERIFY( res3.get_allocator() == defAlloc );
  verifyMove(res3);

  src = String(cstr, alloc);
  data = src.data();
  String res4(std::move(src), 5, alloc);
  VERIFY( res4 == StringView(cstr).substr(5) );
  verifyMove(res4);

  // substr(0, 50)
  src = String(cstr, alloc);
  data = src.data();
  res = std::move(src).substr(0, 50);
  VERIFY( res == StringView(cstr).substr(0, 50) );
  VERIFY( res.get_allocator() == defAlloc );
  verifyMove(res);

  src = String(cstr, alloc);
  data = src.data();
  String res5(std::move(src), 0, 50);
  VERIFY( res5 == StringView(cstr).substr(0, 50) );
  VERIFY( res5.get_allocator() == defAlloc );
  verifyMove(res5);

  src = String(cstr, alloc);
  data = src.data();
  String res6(std::move(src), 0, 50, alloc);
  VERIFY( res6 == StringView(cstr).substr(0, 50) );
  VERIFY( res6.get_allocator() == alloc );
  verifyMove(res6);

  // substr(5, 50)
  src = String(cstr, alloc);
  data = src.data();
  res = std::move(src).substr(5, 50);
  VERIFY( res == StringView(cstr).substr(5, 50) );
  VERIFY( res.get_allocator() == defAlloc );
  verifyMove(res);

  src = String(cstr, alloc);
  data = src.data();
  String res7(std::move(src), 5, 50);
  VERIFY( res7 == StringView(cstr).substr(5, 50) );
  VERIFY( res7.get_allocator() == defAlloc );
  verifyMove(res7);

  src = String(cstr, alloc);
  data = src.data();
  String res8(std::move(src), 5, 50, alloc);
  VERIFY( res8 == StringView(cstr).substr(5, 50) );
  VERIFY( res8.get_allocator() == alloc );
  verifyMove(res8);

  // substr(5, 100)
  src = String(cstr, alloc);
  data = src.data();
  res = std::move(src).substr(5, 1000);
  VERIFY( res == StringView(cstr).substr(5, 1000) );
  VERIFY( res.get_allocator() == defAlloc );
  verifyMove(res);

  src = String(cstr, alloc);
  data = src.data();
  String res9(std::move(src), 5, 1000);
  VERIFY( res9 == StringView(cstr).substr(5, 1000) );
  VERIFY( res9.get_allocator() == defAlloc );
  verifyMove(res9);

  src = String(cstr, alloc);
  data = src.data();
  String res10(std::move(src), 5, 1000, alloc);
  VERIFY( res10 == StringView(cstr).substr(5, 1000) );
  VERIFY( res10.get_allocator() == alloc );
  verifyMove(res10);
}

template<typename CharT, typename Allocator>
constexpr void
do_test_alloc(const CharT* sht, const CharT* lng, Allocator alloc)
{
  do_test_short_in(sht, alloc);
  do_test_short_sel(sht, alloc);
  do_test_short_sel(lng, alloc);
  do_test_long(lng, alloc);
}

template<typename CharT>
constexpr void
do_test_bounds(const CharT* cstr)
{
  using String = std::basic_string<CharT>;
  try
  {
    auto res = String(cstr).substr(30);
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }

  try
  {
    auto res = String(cstr).substr(30, 20);
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }

  try
  {
    String res(String(cstr), 30);
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }

  try
  {
    String res(String(cstr), 30, 20);
    VERIFY(false);
  } catch (const std::out_of_range&) {
    VERIFY(true);
  }
}

template<typename CharT>
constexpr void
do_test(const CharT* sht, const CharT* lng)
{
  do_test_alloc(sht, lng, std::allocator<CharT>());
  if ! consteval {
    do_test_alloc(sht, lng, __gnu_test::uneq_allocator<CharT>());
    do_test_alloc(sht, lng, __gnu_test::uneq_allocator<CharT>(42));
    do_test_bounds(sht);
  }
}

constexpr bool
test_all()
{
  do_test(
    "abcdefghijklmop",
    "some very very long string, that will not use SSO, and have  at least fifty characters");
  do_test(
    L"abc", 
    L"some very very long string, that will not use SSO, and have  at least fifty characters");

  return true;
}

int main()
{ 
  test_all();
}
