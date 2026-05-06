// { dg-do run { target c++26 } }
// { dg-options "-fexec-charset=UTF-8" }

#include <filesystem>
#include <format>
#include <testsuite_hooks.h>

using std::filesystem::path;

template<typename... Args>
bool
is_format_string_for(const char* str, Args&&... args)
{
  try {
    (void) std::vformat(str, std::make_format_args(args...));
    return true;
  } catch (const std::format_error&) {
    return false;
  }
}

template<typename... Args>
bool
is_format_string_for(const wchar_t* str, Args&&... args)
{
  try {
    (void) std::vformat(str, std::make_wformat_args(args...));
    return true;
  } catch (const std::format_error&) {
    return false;
  }
}

void
test_format_spec()
{
  // [fs.path.fmtr.funcs]
  // \nontermdef{path-format-spec}\br
  //     \opt{fill-and-align} \opt{width} \opt{\terminal{?}} \opt{\terminal{g}}
  path p;
  VERIFY( is_format_string_for("{}", p) );
  VERIFY( is_format_string_for("{:}", p) );
  VERIFY( is_format_string_for("{:?}", p) );
  VERIFY( is_format_string_for("{:g}", p) );
  VERIFY( is_format_string_for("{:?g}", p) );
  VERIFY( is_format_string_for("{:?g}", p) );
  VERIFY( is_format_string_for("{:F^32?g}", p) );
  VERIFY( is_format_string_for("{:G<{}?g}", p, 32) );
  VERIFY( is_format_string_for(L"{:G<{}?g}", p, 32) );

  VERIFY( ! is_format_string_for("{:g?}", p) );
}

#define WIDEN_(C, S) ::std::__format::_Widen<C>(S, L##S)
#define WFMT(S) WIDEN_(_FCharT, S)
#define WPATH(S) WIDEN_(_PCharT, S)

template<typename _FCharT, typename _PCharT>
void
test_format()
{
  std::basic_string<_FCharT> res;
  res = std::format(WFMT("{}"), path(WPATH("/usr/include")));
  VERIFY( res == WFMT("/usr/include") );
  res = std::format(WFMT("{:.<10}"), path(WPATH("foo/bar")));
  VERIFY( res == WFMT("foo/bar...") );
  res = std::format(WFMT("{}"), path(WPATH("foo///bar")));
  VERIFY( res == WFMT("foo///bar") );
  res = std::format(WFMT("{:g}"), path(WPATH("foo///bar")));
  VERIFY( res == WFMT("foo/bar") );
  res = std::format(WFMT("{}"), path(WPATH("/path/with/new\nline")));
  VERIFY( res == WFMT("/path/with/new\nline") );
  res = std::format(WFMT("{:?}"), path(WPATH("multi\nline")));
  VERIFY( res == WFMT("\"multi\\nline\"") );
  res = std::format(WFMT("{:?g}"), path(WPATH("mu///lti\nli///ne")));
  VERIFY( res == WFMT("\"mu/lti\\nli/ne\"") );
  res = std::format(WFMT("{}"),
           path(WPATH("\u0428\u0447\u0443\u0447\u044B\u043D\u0448\u0447\u044B\u043D\u0430")));
  VERIFY( res == WFMT("\u0428\u0447\u0443\u0447\u044B\u043D\u0448\u0447\u044B\u043D\u0430"));

  if constexpr (path::preferred_separator == L'\\')
  {
    res = std::format(WFMT("{}"), path(WPATH("C:\\foo\\bar")));
    VERIFY( res == WFMT("C:\\foo\\bar") );
    res = std::format(WFMT("{:g}"), path(WPATH("C:\\foo\\bar")));
    VERIFY( res == WFMT("C:/foo/bar") );
  }
}

void
test_format_invalid()
{
  if constexpr (std::is_same_v<path::value_type, char>)
  {
    std::wstring res;
    std::string_view seq = "\xf0\x9f\xa6\x84"; //  \U0001F984
    
    path p(seq.substr(1));
    res = std::format(L"{}", p);
    VERIFY( res == L"\uFFFD\uFFFD\uFFFD" );
    res = std::format(L"{:?}", p);
    VERIFY( res == LR"("\x{9f}\x{a6}\x{84}")" );
  }
  else
  {
    std::string res;

    path p(L"\xd800");
    res = std::format("{}", p);
    VERIFY( res == "\uFFFD" );
    res = std::format("{:?}", p);
    VERIFY( res == "\"\\x{d800}\"" );

    path p2(L"///\xd800");
    res = std::format("{}", p2);
    VERIFY( res == "///\uFFFD" );
    res = std::format("{:g}", p2);
    VERIFY( res == "/\uFFFD" );
    res = std::format("{:?}", p2);
    VERIFY( res == "\"///\\x{d800}\"" );
    res = std::format("{:?g}", p2);
    VERIFY( res == "\"/\\x{d800}\"" );
    res = std::format("{:C>14?g}", p2);
    VERIFY( res == "CCC\"/\\x{d800}\"" );
  }
}

int main()
{
  test_format_spec();
  test_format<char, char>();
  test_format<char, wchar_t>();
  test_format<wchar_t, char>();
  test_format<wchar_t, wchar_t>();
  test_format_invalid();
}
