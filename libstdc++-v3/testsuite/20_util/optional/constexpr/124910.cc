// { dg-do compile { target c++20 } }

// PR124910 - bogus 'std::optional{...}' is not a constant expression error
// after resetting it via '= nullopt'

#include <optional>

struct A
{
  constexpr A(int m) : m(m) { }
  int m;
};

struct B
{
  constexpr B(int m) : m(m) { }
  constexpr ~B() { }
  int m;
};

static_assert(   std::is_trivially_destructible_v<int> );
static_assert(   std::is_trivially_destructible_v<A> );
static_assert( ! std::is_trivially_destructible_v<B> );

template<class T>
void
do_test()
{
  constexpr std::optional<T> x1 = [] {
    std::optional<T> o = 1;
    o = std::nullopt;
    return o;
  }();

  constexpr std::optional<T> x2 = [] {
    std::optional<T> o = 1;
    o.reset();
    return o;
  }();

  constexpr std::optional<T> x3 = [] {
    std::optional<T> o1 = 1;
    std::optional<long> o2;
    o1 = o2;
    return o1;
  }();

  constexpr std::optional<T> x4 = [] {
    std::optional<T> o1 = 1;
    std::optional<long> o2;
    o1 = std::move(o2);
    return o1;
  }();

  constexpr std::optional<T> x5 = [] {
    std::optional<T> o1 = 1;
    std::optional<T> o2;
    std::swap(o1, o2);
    return o1;
  }();

  struct C : std::optional<T> {
    constexpr C() : std::optional<T>(1) { this->reset(); }
  };
  constexpr C x6;
}

int
main()
{
  do_test<int>();
  do_test<A>();
  do_test<B>();
}
