// PR c++/121597
// { dg-do compile { target c++20 } }

template<int, class> struct Tmpl {};
template<typename T> struct Wrap {};

template<typename = void>
void foo()
{
  using Type = decltype([]<typename = void>{
    return Tmpl<0, char>{};
  }());

  []<typename = void>{
    using X = decltype([]<auto N, class T>(Tmpl<N, T>&&)
      {}.template operator()<0>(Type{}));
    return Wrap<X>{};
  }();
};

template void foo<>();
