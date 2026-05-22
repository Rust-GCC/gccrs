// PR c++/125135
// { dg-do compile { target c++11 } }

template <typename T>
struct A
{
  template <typename... Ts> // variadic causea ICE.
  A(T&, const Ts&...){} // T& arg causes ICE.

  A(const A&) = delete; // causes ICE.
};

struct RE
{
  RE(): a(*this) {}

  A<RE> a;
};

RE re;
