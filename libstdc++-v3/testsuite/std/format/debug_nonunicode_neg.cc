// { dg-options "-fexec-charset=ISO-8859-1" }
// { dg-do compile { target c++26 } }
// { dg-require-effective-target cxx11_abi }
// { dg-require-iconv "ISO-8859-1" }

#include <format>

constexpr bool
test_format(std::string_view str)
{
  (void)std::format("{:?}", str);
  return true;
}

static_assert(test_format("\x10")); // { dg-error "in 'constexpr' expansion of" }
static_assert(test_format("Åëÿ")); // { dg-error "in 'constexpr' expansion of" }

// { dg-prune-output "for non-Unicode literal encodings, only printable ASCII characters and standard" }

