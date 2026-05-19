// PR c++/121287
// { dg-do compile { target c++20 } }

template <typename _Tp, _Tp...> struct integer_sequence {};
template <long... _Idx>
using index_sequence = integer_sequence<unsigned long, _Idx...>;
template <typename> struct array {};

template <typename = void>
auto
f ()
{
  using Res = decltype([]<unsigned long Idx>(index_sequence<Idx>) { }(index_sequence<3>{}));
  return []<long...>(index_sequence<>) { array<Res>{}; }({});
}

void
g ()
{
  f ();
}
