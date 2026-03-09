// { dg-do compile { target c++26 } }
// { dg-require-iconv "IBM1047" }
// { dg-additional-options "-fexec-charset=IBM1047" }

struct S {
  char buf[16];
  constexpr const char *data () const { return buf; }
  constexpr decltype (sizeof 0) size () const { for (int i = 0; i < 16; ++i) if (!buf[i]) return i; return 0;  }
};
struct T {
  constexpr const char *data () const { return "bar"; }
  constexpr decltype (sizeof 0) size () const { return 3; }
};
constexpr char str[] = "abcdefg";
struct U {
  constexpr const char *data () const { return &str[2]; }
  constexpr decltype (sizeof 0) size () const { return 4; }
};
struct V {
  constexpr const char *data () const { return &"abcdefghi"[3]; }
  constexpr decltype (sizeof 0) size () const { return 5; }
};
struct W {
  constexpr const char *data () const { return &"abcdefghi"[3] + 2; }
  constexpr decltype (sizeof 0) size () const { return 3; }
};

consteval
{
  S s;
  for (int i = 0; i < 10; ++i)
    s.buf[i] = '0' + i;
  s.buf[10] = '\0';
  __builtin_constexpr_diag (0, "foo", "bar");		// { dg-message "constexpr message: bar \\\[foo\\\]" }
  __builtin_constexpr_diag (1, "foo", "bar");		// { dg-warning "constexpr message: bar \\\[foo\\\]" }
  __builtin_constexpr_diag (2, "foo", "bar");		// { dg-error "constexpr message: bar \\\[foo\\\]" }
  __builtin_constexpr_diag (0, "bar_baz", "bar");	// { dg-message "constexpr message: bar \\\[bar_baz\\\]" }
  __builtin_constexpr_diag (1, "bar_baz", "bar");	// { dg-warning "constexpr message: bar \\\[bar_baz\\\]" }
  __builtin_constexpr_diag (2, "bar_baz", "bar");	// { dg-error "constexpr message: bar \\\[bar_baz\\\]" }
  __builtin_constexpr_diag (0, "baz", s);		// { dg-message "constexpr message: 0123456789 \\\[baz\\\]" }
  __builtin_constexpr_diag (1, "baz", T {});		// { dg-warning "constexpr message: bar \\\[baz\\\]" }
  __builtin_constexpr_diag (2, "baz", U {});		// { dg-error "constexpr message: cdef \\\[baz\\\]" }
  __builtin_constexpr_diag (0, "baz", V {});		// { dg-message "constexpr message: defgh \\\[baz\\\]" }
  __builtin_constexpr_diag (1, "baz", W {});		// { dg-warning "constexpr message: fgh \\\[baz\\\]" }
  __builtin_constexpr_diag (0, "", "bar");		// { dg-message "constexpr message: bar" }
  __builtin_constexpr_diag (1, "", "bar");		// { dg-warning "constexpr message: bar" }
  __builtin_constexpr_diag (2, "", "bar");		// { dg-error "constexpr message: bar" }
}
