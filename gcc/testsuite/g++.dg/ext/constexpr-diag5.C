// { dg-do compile { target c++26 } }
// { dg-require-iconv "IBM1047" }
// { dg-additional-options "-fexec-charset=IBM1047" }

#include <string_view>

namespace std
{
#if __has_builtin(__builtin_constexpr_diag)
  struct _S_constexpr_tag_str {
  private:
    string_view _M_str;
  public:
    template <class _Tp>
      requires convertible_to<const _Tp&, string_view>
      consteval _S_constexpr_tag_str(const _Tp& __s) : _M_str(__s) {}
    friend constexpr void constexpr_print_str(_S_constexpr_tag_str __tag,
					      string_view) noexcept;
    friend constexpr void constexpr_print_str(_S_constexpr_tag_str __tag,
					      u8string_view) noexcept;
    friend constexpr void constexpr_warning_str(_S_constexpr_tag_str,
						string_view) noexcept;
    friend constexpr void constexpr_warning_str(_S_constexpr_tag_str,
						u8string_view) noexcept;
    friend constexpr void constexpr_error_str(_S_constexpr_tag_str,
					      string_view) noexcept;
    friend constexpr void constexpr_error_str(_S_constexpr_tag_str,
					      u8string_view) noexcept;
  };
  constexpr void constexpr_print_str(string_view __msg) noexcept
  { return __builtin_constexpr_diag(16, "", __msg); }
  constexpr void constexpr_print_str(u8string_view __msg) noexcept
  { return __builtin_constexpr_diag(16, "", __msg); }
  constexpr void constexpr_print_str(_S_constexpr_tag_str __tag,
				     string_view __msg) noexcept
  { return __builtin_constexpr_diag(16, __tag._M_str, __msg); }
  constexpr void constexpr_print_str(_S_constexpr_tag_str __tag,
				     u8string_view __msg) noexcept
  { return __builtin_constexpr_diag(16, __tag._M_str, __msg); }
  constexpr void constexpr_warning_str(_S_constexpr_tag_str __tag,
				       string_view __msg) noexcept
  { return __builtin_constexpr_diag(17, __tag._M_str, __msg); }
  constexpr void constexpr_warning_str(_S_constexpr_tag_str __tag,
				       u8string_view __msg) noexcept
  { return __builtin_constexpr_diag(17, __tag._M_str, __msg); }
  constexpr void constexpr_error_str(_S_constexpr_tag_str __tag,
				     string_view __msg) noexcept
  { return __builtin_constexpr_diag(18, __tag._M_str, __msg); }
  constexpr void constexpr_error_str(_S_constexpr_tag_str __tag,
				     u8string_view __msg) noexcept
  { return __builtin_constexpr_diag(18, __tag._M_str, __msg); }
#endif
}

consteval
{
  std::constexpr_print_str("foo");			// { dg-message "constexpr message: foo" }
  std::constexpr_print_str(u8"bar");			// { dg-message "constexpr message: bar" }
  std::constexpr_print_str("uninitialized", "foo");	// { dg-message "constexpr message: foo \\\[uninitialized\\\]" }
  std::constexpr_print_str("uninitialized", u8"bar");	// { dg-message "constexpr message: bar \\\[uninitialized\\\]" }
  std::constexpr_warning_str("uninitialized", "foo");	// { dg-warning "constexpr message: foo \\\[uninitialized\\\]" }
  std::constexpr_warning_str("uninitialized", u8"bar");	// { dg-warning "constexpr message: bar \\\[uninitialized\\\]" }
  std::constexpr_error_str("uninitialized", "foo");	// { dg-error "constexpr message: foo \\\[uninitialized\\\]" }
  std::constexpr_error_str("uninitialized", u8"bar");	// { dg-error "constexpr message: bar \\\[uninitialized\\\]" }
}
