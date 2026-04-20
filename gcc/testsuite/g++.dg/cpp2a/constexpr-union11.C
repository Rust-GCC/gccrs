// PR c++/124910
// { dg-do compile { target c++20 } }

struct nullopt_t { } nullopt;
using _Stored_type = int;
struct _Empty { };
struct _Optional_payload_base {
  union {
    int _M_value = 1;
    _Empty _M_empty;
  };
  constexpr void _M_reset() {
    _M_value.~_Stored_type();
    _M_empty = {};
  }
};
struct _Optional_payload : _Optional_payload_base {};
struct _Optional_base {
  _Optional_payload _M_payload;
};
struct optional : _Optional_base {
  constexpr void operator=(nullopt_t) { _M_payload._M_reset(); }
};
constexpr auto f() {
  optional result;
  result = nullopt;
  return result;
}
constexpr auto v = f();
