// { dg-do compile { target c++11 } }

template <typename _Tp> struct remove_cv {
  using type = _Tp;
};
struct basic_string {
  ~basic_string();
};
template <typename T> using remove_cvref_t = remove_cv<T>;
template <typename> struct basic_string_view {};
template <typename> struct basic_format_string;
auto format(...) -> basic_string {} // { dg-warning "no return statement" }
template <int N>
auto compile_string_to_view(const char (&)[N]) -> basic_string_view<char>;

void MallocError() {
  format([] {
    using char_type = remove_cvref_t<decltype(0)>;
    struct FMT_COMPILE_STRING {
      operator char_type() {} // { dg-warning "no return statement" }
    };
    return FMT_COMPILE_STRING();
  });
}
