// PR c++/124689
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }
// Test error recovery from bogus <meta> replacement.

namespace std {
  using size_t = decltype (sizeof 0);
  struct string_view {};
  namespace meta {
    using info = decltype(^^::);
    consteval int is_enumerator (info);
    consteval void type_of (info);
    consteval char size_of (info);
    consteval int offset_of (info);
    consteval float operator_of (info);
    consteval int *source_location_of (info);
    consteval bool identifier_of (info);
    consteval void u8identifier_of (info);
    consteval int type_order (info, info);
    consteval double enumerators_of (info);
    struct access_context {
      static consteval short current ();
    };
    consteval bool is_bit_field (int);
    consteval bool is_void_type (double);
    consteval info tuple_element (long int, info);
    consteval size_t extent (info, info);
    consteval string_view symbol_of (int);
    consteval info members_of (info, int);
    consteval info data_member_spec (info, float);
  }
}

int a;
constexpr auto b = is_enumerator (^^::);		// { dg-error "incorrect 'int' return type, expected 'bool'" }
consteval {
  type_of (^^a);					// { dg-error "incorrect 'void' return type, expected 'std::meta::info'" }
}
struct S { int a; S &operator [] (const S &); };
constexpr auto c = size_of (^^a);			// { dg-error "incorrect 'char' return type, expected 'std::size_t'" }
constexpr auto d = offset_of (^^S::a);			// { dg-error "incorrect 'int' return type, expected 'std::meta::member_offset'" }
constexpr auto e = operator_of (^^S::operator []);	// { dg-error "incorrect 'float' return type, expected 'std::meta::operators'" }
constexpr auto f = source_location_of (^^a);		// { dg-error "incorrect 'int\\\*' return type, expected 'std::source_location'" }
constexpr auto g = identifier_of (^^a);			// { dg-error "incorrect 'bool' return type, expected 'std::string_view'" }
consteval {
  u8identifier_of (^^a);				// { dg-error "incorrect 'void' return type, expected 'std::u8string_view'" }
}
constexpr auto i = type_order (^^int, ^^int);		// { dg-error "incorrect 'int' return type, expected 'std::strong_ordering'" }
enum E { E1 };
constexpr auto j = enumerators_of (^^E);		// { dg-error "incorrect 'double' return type, expected 'std::vector<std::meta::info>'" }
constexpr auto k = std::meta::access_context::current (); // { dg-error "incorrect 'short int' return type, expected 'std::meta::access_context'" }
constexpr auto l = std::meta::is_bit_field (0);		// { dg-error "incorrect 'int' type of argument 1, expected 'std::meta::info'" }
constexpr auto m = std::meta::is_void_type (0.0);	// { dg-error "incorrect 'double' type of argument 1, expected 'std::meta::info'" }
constexpr auto n = std::meta::tuple_element (0, ^^::);	// { dg-error "incorrect 'long int' type of argument 1, expected 'std::size_t'" }
constexpr auto o = extent (^^int, ^^int);		// { dg-error "incorrect 'std::meta::info' type of argument 2, expected 'unsigned int'" }
constexpr auto p = std::meta::symbol_of (0);		// { dg-error "incorrect 'int' type of argument 1, expected 'std::meta::operators'" }
constexpr auto q = members_of (^^S, 0);			// { dg-error "incorrect 'std::meta::info' return type, expected 'std::vector<std::meta::info>'" }
constexpr auto r = data_member_spec (^^S, 0.0f);	// { dg-error "incorrect 'float' type of argument 2, expected 'std::meta::data_member_options'" }
