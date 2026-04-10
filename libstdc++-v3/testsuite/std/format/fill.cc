// { dg-options "-fexec-charset=UTF-8 -DUNICODE_ENC" }
// { dg-do run { target c++20 } }

#include <format>
#include <testsuite_hooks.h>

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

template<typename T>
void
test_fill(T t)
{
  constexpr bool accept_utf8_nonascii
#ifdef UNICODE_ENC
   = true;
#else
   = false;
#endif

   VERIFY(is_format_string_for("{: <}", t));
   VERIFY(is_format_string_for("{:Å<}", t));
   // U+0119 Latin Small Letter E with Ogonek
   VERIFY(is_format_string_for("{:\xC4\x99<}", t) == accept_utf8_nonascii);
   // U+2705 White Heavy Check Mark
   VERIFY(is_format_string_for("{:\xE2\x9C\x85<}", t) == accept_utf8_nonascii);
   // U+1F602 Face with Tears of Joy
   VERIFY(is_format_string_for("{:\xF0\x9F\x98\x82<}", t) == accept_utf8_nonascii);
}

struct MyStringView : std::string_view
{
  using std::string_view::string_view;
};

template<>
struct std::formatter<MyStringView, char>
  : std::formatter<std::string_view, char>
{};

int main()
{
  test_fill(10);
  test_fill(std::string_view("test"));
  // Test with type stored by handle
  test_fill(MyStringView("test"));
}
