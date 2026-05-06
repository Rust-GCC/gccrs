// CWG 3123
// { dg-do compile { target c++26 } }

namespace N {
template <class C>
auto begin (C &c) noexcept (noexcept (c.begin ()))
-> decltype (c.begin ());

template <class C>
auto end (C &c) noexcept (noexcept (c.end ()))
-> decltype (c.end ());

struct D { };
}

template <class T>
struct E {
int x;

  struct F {
    static_assert (!__is_same (T, N::D));
  };

  F begin ();
};

int
main ()
{
  template for (auto elem : E <N::D> ())
    ;
}
