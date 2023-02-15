// { dg-do compile { target c++20 } }

namespace std {
  struct source_location {
    struct __impl {
      const char *_M_file_name, *_M_function_name;
      int __foo, _M_line, _M_column;
    };
  };
}

auto x = __builtin_source_location ();	// { dg-error "'std::source_location::__impl' does not contain only non-static data members '_M_file_name', '_M_function_name', '_M_line' and '_M_column'" }
// { dg-message "using '__builtin_source_location'" "" { target *-*-* } .-1 }
