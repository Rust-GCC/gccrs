// { dg-do run { target c++26 } }

#include <memory>
#include <testsuite_hooks.h>

struct Obj
{
  int i;
  constexpr auto operator<=>(const Obj&) const = default;
};

template<>
struct std::hash<Obj>
{
  static size_t operator()(Obj const& obj)
  { return std::hash<int>{}(obj.i);  }
};

constexpr void
test_relops()
{
  std::indirect<Obj> i1;
  VERIFY( i1 == i1 );
  VERIFY( i1 <= i1 );
  VERIFY( i1 >= i1 );

  std::indirect<Obj> i2 = std::move(i1); // make i1 valueless
  VERIFY( i1 == i1 );
  VERIFY( i2 == i2 );
  VERIFY( i2 != i1 );
  VERIFY( i1 < i2 );
  VERIFY( i2 >= i1 );

  std::indirect<Obj> i3 = std::move(i2); // make i2 valueless
  VERIFY( i2 == i1 );
  VERIFY( i2 >= i1 );
  VERIFY( i2 <= i1 );
  VERIFY( i3 > i2 );
}

constexpr void
test_comp_with_t()
{
  std::indirect<Obj> i1;
  Obj o{2};
  VERIFY( i1 != o );
  VERIFY( i1 < o );

  std::indirect<Obj> i2(Obj{2});
  VERIFY( i2 == o );
  VERIFY( i2 <= o );
  VERIFY( o <= i2 );

  std::indirect<Obj> i3 = std::move(i2); // make i2 valueless
  VERIFY( i2 != o );
  VERIFY( i2 < o );
}

void
test_hash()
{
  Obj o{5};
  std::indirect<Obj> i(o);
  VERIFY( std::hash<std::indirect<Obj>>{}(i)
	  == std::hash<Obj>{}(o) );

  auto(std::move(i)); // make i valueless
  (void)std::hash<std::indirect<Obj>>{}(i);
}

template<bool Noexcept>
struct BoolConv
{
  bool b;

  constexpr BoolConv(bool p) noexcept
  : b(p)
  { }

  constexpr operator bool() const noexcept(Noexcept)
  { return b; }
};

template<bool Noexcept, typename EqRet = bool>
struct Comp
{
  friend constexpr EqRet
  operator==(const Comp&, const Comp&) noexcept(Noexcept)
  { return true; }

  friend constexpr std::strong_ordering
  operator<=>(const Comp&, const Comp&) noexcept(Noexcept)
  { return std::strong_ordering::equal; }
};

template<bool Noexcept, typename T>
void test_noexcept_eq(T t)
{
  std::indirect<T> i(t);
  static_assert(noexcept(i == t) == Noexcept);
  static_assert(noexcept(i != t) == Noexcept);
  static_assert(noexcept(i == i) == Noexcept);
  static_assert(noexcept(i != i) == Noexcept);
}

template<bool Noexcept, typename T>
void test_noexcept_rel(T t)
{
  std::indirect<T> i(t);
  static_assert(noexcept(i < t) == Noexcept);
  static_assert(noexcept(i > t) == Noexcept);
  static_assert(noexcept(i <= t) == Noexcept);
  static_assert(noexcept(i >= t) == Noexcept);
  static_assert(noexcept(i <=> t) == Noexcept);

  static_assert(noexcept(t < i) == Noexcept);
  static_assert(noexcept(t > i) == Noexcept);
  static_assert(noexcept(t <= i) == Noexcept);
  static_assert(noexcept(t >= i) == Noexcept);
  static_assert(noexcept(t <=> i) == Noexcept);

  static_assert(noexcept(i < i) == Noexcept);
  static_assert(noexcept(i > i) == Noexcept);
  static_assert(noexcept(i <= i) == Noexcept);
  static_assert(noexcept(i >= i) == Noexcept);
  static_assert(noexcept(i <=> i) == Noexcept);
}

void test_noexcept()
{
  test_noexcept_eq<true>(Comp<true>());
  test_noexcept_eq<false>(Comp<false>());
  test_noexcept_rel<true>(Comp<true>());
  test_noexcept_rel<false>(Comp<false>());

  test_noexcept_eq<true>(Comp<true, BoolConv<true>>());
  test_noexcept_eq<false>(Comp<true, BoolConv<false>>());
  test_noexcept_eq<false>(Comp<false, BoolConv<true>>());
  test_noexcept_eq<false>(Comp<false, BoolConv<false>>());
}

int main()
{
  test_relops();
  test_comp_with_t();
  test_hash();
  test_noexcept();

  static_assert([] {
    test_relops();
    test_comp_with_t();
    return true;
  }());
}
