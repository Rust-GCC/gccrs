// CWG3119 - for-range-declaration of an expansion-statement as a templated
// entity.
// { dg-do run { target c++26 } }

namespace std {
  using size_t = decltype (sizeof 0);
  template<typename T> struct tuple_size;
  template<size_t, typename> struct tuple_element;
}
struct B { int a; long b; };
struct C { char a; short b; int c; long d; long long e; float f; };
struct E { char a; template <int I> char &get () { return a; } };

template <> struct std::tuple_size <E> { static const int value = 7; };
template <std::size_t I> struct std::tuple_element <I, E> { using type = char; };

struct A
{
  B b;
  C c;
  int d[3];
  E e;
};

template <typename ...T>
int
foo (T... x)
{
  return sizeof... (x);
}

int
main ()
{
  int ret = 0;
  template for (auto [...e] : A ())
    ret += foo (e...);
  if (ret != 18)
    __builtin_abort ();
}
