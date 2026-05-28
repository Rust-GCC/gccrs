// { dg-do run { target c++26 } }

#include <filesystem>
#include <testsuite_fs.h>
#include <testsuite_hooks.h>

using std::filesystem::path;

#define WIDEN(S) ::std::__format::_Widen<CharT>(S, L##S) 

template<typename CharT>
void
test_display_and_native()
{
  std::string res;
  path p;
  auto verify_both = [&res](const path& p, std::string_view expected)
  {
    res = p.generic_display_string();
    VERIFY( res == expected );
    res = p.generic_native_encoded_string();
    VERIFY( res == expected );
  };

  verify_both(WIDEN("/usr/include"), "/usr/include");
  verify_both(WIDEN("foo///bar"), "foo/bar");
  for (path p : __gnu_test::test_paths)
    verify_both(p, p.generic_string()); // { dg-warning "deprecated" }

  verify_both(
    WIDEN("\u0428\u0447\u0443\u0447\u044B\u043D\u0448\u0447\u044B\u043D\u0430"),
    "\u0428\u0447\u0443\u0447\u044B\u043D\u0448\u0447\u044B\u043D\u0430" );

  if constexpr (path::preferred_separator == L'\\')
    verify_both(WIDEN("C:\\foo\\bar"), "C:/foo/bar");
}

void
test_display_invalid()
{
  std::string res;
  auto verify_native_error = [&res](const path& p, std::string_view expected)
  {
    res = p.generic_display_string();
    VERIFY( res == expected );
    try { p.generic_native_encoded_string(); }
    catch (const std::filesystem::filesystem_error&) {}
  };

#ifdef _GLIBCXX_USE_WCHAR_T
  if constexpr (std::is_same_v<path::value_type, wchar_t>)
  {
    verify_native_error(L"\xd800", "\uFFFD");
    verify_native_error(L"///\xd800", "/\uFFFD");
  }
#endif
}

int main()
{
  test_display_and_native<char>();
#ifdef _GLIBCXX_USE_WCHAR_T
  test_display_and_native<wchar_t>();
  test_display_invalid();
#endif
}
