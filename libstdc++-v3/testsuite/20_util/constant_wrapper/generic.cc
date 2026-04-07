// { dg-do run { target c++26 } }
#include <functional>
#include <utility>
#include <string_view>
#include <stdexcept>

#include <testsuite_hooks.h>

constexpr void
check_same(auto actual, auto expected)
{
  VERIFY(actual == expected);
  static_assert(std::same_as<decltype(actual), decltype(expected)>);
};


constexpr void
test_c_arrays()
{
  constexpr double x[] = {1.1, 2.2, 3.3};
  auto cx = std::cw<x>;
  auto access = [](auto x, size_t i)
  { return x[i]; };

  check_same(std::cw<x>[0], x[0]);
  check_same(std::cw<x>[1], x[1]);
  check_same(std::cw<x>[2], x[2]);

  check_same(cx[std::cw<0>], std::cw<x[0]>);
  check_same(cx[std::cw<1>], std::cw<x[1]>);
  check_same(cx[std::cw<2>], std::cw<x[2]>);
}

constexpr size_t
deduce_cstr_size(auto str)
{
  size_t sz = 0;
  while(str[sz++] != '\0') { }
  return sz;
}

constexpr void
test_string_literals()
{
  auto foo = std::cw<"foo">;
  constexpr const typename decltype(foo)::value_type & cstr = foo;
  constexpr size_t N = std::size(cstr);
  constexpr auto foo_view = std::string_view(cstr, N-1);

  constexpr const char (&cstr1)[deduce_cstr_size(foo)] = foo;
  constexpr size_t N1 = std::size(cstr);

  static_assert(static_cast<char const*>(cstr) ==
		static_cast<char const*>(cstr1));
  static_assert(N1 == N);

  static_assert(foo[0] == 'f');
  static_assert(foo[1] == 'o');
  static_assert(foo[2] == 'o');
  static_assert(foo[3] == '\0');
  static_assert(static_cast<char const *>(foo) == foo_view);
}

constexpr bool
convert_constexpr(auto c)
{
  if constexpr (int(c) > 0)
    return true;
  return false;
}

constexpr void
test_converted_constexpr()
{
  VERIFY(!convert_constexpr(std::cw<-1>));
  VERIFY(convert_constexpr(std::cw<1>));
}

constexpr void
test_ints()
{
  std::constant_wrapper<2> two;
  std::constant_wrapper<3> three;
  std::constant_wrapper<5> five;

  VERIFY(two + 3 == 5);
  static_assert(std::same_as<decltype(two + 3), int>);

  VERIFY(two + three == 5);
  VERIFY(two + three == five);
  static_assert(std::same_as<decltype(two + three), std::constant_wrapper<5>>);

  VERIFY(two == std::cw<2>);
  VERIFY(two + 3 == std::cw<5>);
}

constexpr int
add(int i, int j)
{ return i + j; }

template<bool Noexcept>
struct CAdd
{
  constexpr int
  operator()(int i, int j) const noexcept(Noexcept)
  { return i + j; }
};

template<bool Noexcept>
struct RAdd
{
  int
  operator()(int i, int j) const noexcept(Noexcept)
  { return i + j; }
};

struct CMixedAdd
{
  static constexpr int
  operator()(int i, int j)
  { return i + j; }

  template <auto N1, auto N2>
    static constexpr int operator()
    (std::constant_wrapper<N1, int> i, std::constant_wrapper<N2, int> j)
    { return 100 + i + j; }
};

struct RMixedAdd
{
  static int
  operator()(int i, int j)
  { return i + j; }

  template <auto N1, auto N2>
    static int operator()
    (std::constant_wrapper<N1, int> i, std::constant_wrapper<N2, int> j)
    { return 100 + i + j; }
};

template<typename T1, typename T2>
struct AtLeastOneInt
{
  static_assert(std::is_same_v<T1, int> || std::is_same_v<T2, int>);
  using type = int;
};

struct PoisonedAdd
{
  template<typename T1, typename T2>
  constexpr static
  typename AtLeastOneInt<T1, T2>::type 
  operator()(T1 i, T2 j) noexcept
  { return i + j; }
};

struct MoveOnly
{
  constexpr explicit
  MoveOnly(int p) : v(p)
  { }

  MoveOnly(MoveOnly&&) = default;

  int v;
};

struct MoveArgFunc
{
  constexpr int
  operator()(MoveOnly arg) const
  { return arg.v; }
};

struct ThrowFunc
{
  static constexpr int
  operator()(int i, int j)
  {
    if (i < 0 || j < 0)
      throw std::invalid_argument("negative");
    return i + j;
  }
};

template<bool Constexpr, auto functor>
  constexpr void
  check_invoke()
  {
    auto ci = std::cw<2>;
    auto cj = std::cw<3>;
    auto cfo = std::cw<functor>;

    if constexpr (Constexpr)
      check_same(cfo(ci, cj), std::cw<5>);
    else
      check_same(cfo(ci, cj), 5);

    check_same(cfo(2, cj), 5);
    check_same(cfo(2, 3), 5);

    constexpr bool Noexcept = noexcept(functor(2, 3));
    static_assert(noexcept(cfo(ci, cj)) == Constexpr || Noexcept);
    static_assert(noexcept(cfo(2, cj)) == Noexcept);
    static_assert(noexcept(cfo(2, 3)) == Noexcept);
  }

constexpr void
test_function_object()
{
  check_invoke<true, CAdd<true>{}>();
  check_invoke<true, CAdd<false>{}>();
  check_invoke<true, [](int i, int j) { return i + j; }>();
  check_invoke<true, [](auto i, auto j) noexcept { return i + j; }>();
  if !consteval {
    check_invoke<false, RAdd<true>{}>();
    check_invoke<false, RAdd<false>{}>();
  }

  // Check if constant_wrappers are not passed to value,
  // if they can be unwrapped.
  check_invoke<true, PoisonedAdd{}>();

  // Prefer unwrapping constant_wrappers, so calls (int, int)
  check_same(CMixedAdd{}(2, 3), 5);
  check_same(CMixedAdd{}(std::cw<2>, std::cw<3>), 105);
  check_same(std::cw<CMixedAdd{}>(std::cw<2>, std::cw<3>), std::cw<5>);
  check_invoke<true, CMixedAdd{}>();
  if !consteval {
    // Cannot return value wrapped in constant_wrapper because operator
    // is not constexpr, fallbacks to runtime call, that selects
    // (constant_wrapper, constant_wrapper) overload
    check_same(RMixedAdd{}(2, 3), 5);
    check_same(RMixedAdd{}(std::cw<2>, std::cw<3>), 105);
    check_same(std::cw<RMixedAdd{}>(std::cw<2>, std::cw<3>), 105);
    check_same(std::cw<RMixedAdd{}>(std::cw<2>, 3), 5);
    check_same(std::cw<RMixedAdd{}>(2, 3), 5);
  }

  // Test if arguments are fowarded
  std::cw<MoveArgFunc{}>(MoveOnly{10});

  // For positive arguments call do not throw, constant_wrapper type is valid.
  check_invoke<true, ThrowFunc{}>();
  // For negative arguments, the call exits via exception, and constant_wrapper 
  // type is invalid, so we fallback to runtime.
  static_assert(std::is_same_v<int, decltype(std::cw<ThrowFunc{}>(std::cw<-1>, std::cw<1>))>);
  static_assert(!noexcept(std::cw<ThrowFunc{}>(std::cw<-1>, std::cw<1>)));
  try {
    std::cw<ThrowFunc{}>(std::cw<-1>, std::cw<1>);
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }
}

constexpr void
test_function_pointer()
{
  check_invoke<true, add>();
}

template<bool Noexcept>
struct CIndex
{
  constexpr int
  operator[](int i, int j) const noexcept(Noexcept)
  { return i*j; }
};

template<bool Noexcept>
struct RIndex
{
  int
  operator[](int i, int j) const noexcept(Noexcept)
  { return i*j; }
};

struct CMixedIndex
{
  static constexpr int
  operator[](int i, int j)
  { return i * j; }

  template <auto N1, auto N2>
    static constexpr int operator[]
    (std::constant_wrapper<N1, int> i, std::constant_wrapper<N2, int> j)
    { return 100 + i * j; }
};

struct RMixedIndex
{
  static int
  operator[](int i, int j)
  { return i * j; }

  template <auto N1, auto N2>
    static int operator[]
    (std::constant_wrapper<N1, int> i, std::constant_wrapper<N2, int> j)
    { return 100 + i * j; }
};

struct PoisonedIndex
{
  template<typename T1, typename T2>
  constexpr static
  typename AtLeastOneInt<T1, T2>::type 
  operator[](T1 i, T2 j) noexcept
  { return i * j; }
};

struct MoveArgIndex
{
  constexpr int
  operator[](MoveOnly arg) const
  { return arg.v; }
};

struct ThrowIndex
{
  static constexpr int
  operator[](int i, int j)
  {
    if (i < 0 || j < 0)
      throw std::invalid_argument("negative");
    return i * j;
  }
};

template<bool Constexpr, auto index>
  constexpr void
  check_subscript()
  {
    auto ci = std::cw<2>;
    auto cj = std::cw<3>;
    auto cio = std::cw<index>;

    if constexpr (Constexpr)
      check_same(cio[ci, cj], std::cw<6>);
    else
      check_same(cio[ci, cj], 6);

    check_same(cio[2, cj], 6);
    check_same(cio[2, 3], 6);

    constexpr bool Noexcept = noexcept(index[2, 3]);
    static_assert(noexcept(cio[ci, cj]) == Constexpr || Noexcept);
    static_assert(noexcept(cio[2, cj]) == Noexcept);
    static_assert(noexcept(cio[2, 3]) == Noexcept);
  }

constexpr void
test_indexable1()
{
  check_subscript<true, CIndex<true>{}>();
  check_subscript<true, CIndex<false>{}>();
  if !consteval {
    check_subscript<false, RIndex<true>{}>();
    check_subscript<false, RIndex<false>{}>();
  }

  // Check if constant_wrappers are not passed to value,
  // if they can be unwrapped.
  check_subscript<true, PoisonedIndex{}>();

  // Prefer unwrapping constant_wrappers, so calls (int, int)
  check_same(CMixedIndex{}[2, 3], 6);
  check_same(CMixedIndex{}[std::cw<2>, std::cw<3>], 106);
  check_same(std::cw<CMixedIndex{}>[std::cw<2>, std::cw<3>], std::cw<6>);
  check_subscript<true, CMixedIndex{}>();
  if !consteval {
    // Cannot return value wrapped in constant_wrapper because operator
    // is not constexpr, fallbacks to runtime call, that selects
    // (constant_wrapper, constant_wrapper) overload
    check_same(RMixedIndex{}[2, 3], 6);
    check_same(RMixedIndex{}[std::cw<2>, std::cw<3>], 106);
    check_same(std::cw<RMixedIndex{}>[std::cw<2>, std::cw<3>], 106);
    check_same(std::cw<RMixedIndex{}>[std::cw<2>, 3], 6);
    check_same(std::cw<RMixedIndex{}>[2, 3], 6);
  }

  // Test if arguments are fowarded
  std::cw<MoveArgIndex{}>[MoveOnly{10}];

  // For positive arguments subscript do not throw, constant_wrapper type is valid.
  check_subscript<true, ThrowIndex{}>();
  // For negative arguments, the subscript exits via exception, and constant_wrapper 
  // type is invalid, so we fallback to runtime.
  static_assert(std::is_same_v<int, decltype(std::cw<ThrowIndex{}>[std::cw<-1>, std::cw<1>])>);
  static_assert(!noexcept(std::cw<ThrowIndex{}>[std::cw<-1>, std::cw<1>]));
  try {
    std::cw<ThrowIndex{}>[std::cw<-1>, std::cw<1>];
    VERIFY(false);
  } catch (const std::invalid_argument&) {
    VERIFY(true);
  }
}

struct Indexable2
{
  template<typename I, typename J>
    constexpr int
    operator[](I i, J j) const noexcept
    { return i*j; }
};

constexpr void
test_indexable2()
{
  auto cind = std::cw<Indexable2{}>;
  auto ci = std::cw<2>;
  auto cj = std::cw<3>;
  check_same(cind[ci, cj], std::cw<6>);
  check_same(cind[ci, 3], 6);
  check_same(cind[2, 3], 6);
}

struct Indexable3
{
  template<typename... Is>
    constexpr int
    operator[](Is... i) const noexcept
    { return (1 * ... * i); }
};

constexpr void
test_indexable3()
{
  auto cind = std::cw<Indexable3{}>;
  auto ci = std::cw<2>;
  auto cj = std::cw<3>;

  check_same(cind[], std::cw<1>);
  check_same(cind[ci], std::cw<2>);
  check_same(cind[ci, cj], std::cw<2*3>);
}

struct Divide
{
  int value;

  constexpr int
  divide(int div) const
  { return value / div; }

};

constexpr void
test_member_pointer()
{
  constexpr int nom = 42;
  constexpr int denom = 3;

  auto cvalue = std::cw<&Divide::value>;
  auto cdiv = std::cw<&Divide::divide>;
  auto co = std::cw<Divide{nom}>;

  check_same((&co)->*cvalue, std::cw<nom>);
  check_same((&co)->*(&Divide::value), nom);
  check_same(&(co.value)->*cvalue, nom);

  check_same(cvalue(co), std::cw<nom>);
  check_same(cvalue(co.value), nom);
  check_same(cvalue(&co.value), nom);
  check_same(cvalue(std::ref(co.value)), nom);

  auto cresult = std::cw<nom / denom>;
  check_same(((&co)->*(&Divide::divide))(denom), cresult.value);
  check_same((&(co.value)->*cdiv)(denom), cresult.value);
  check_same(((&decltype(co)::value)->*cdiv)(denom), cresult.value);

  check_same(cdiv(co, std::cw<denom>), cresult); 
  check_same(cdiv(co.value, std::cw<denom>), cresult.value); 
  check_same(cdiv(co.value, denom), cresult.value); 
  check_same(cdiv(&co.value, denom), cresult.value); 
  check_same(cdiv(std::ref(co.value), denom), cresult.value); 
}

struct Truthy
{
  constexpr operator bool() const
  { return true; }
};

template<typename Lhs, typename Rhs>
  concept has_op_and = requires (Lhs lhs, Rhs rhs)
  {
    lhs && rhs;
  };

constexpr void
test_logic()
{
  auto ctrue = std::cw<true>;
  auto cfalse = std::cw<false>;
  auto truthy = Truthy{};

  auto check_and = [](auto lhs, auto rhs)
  {
    static_assert(lhs && rhs);
    static_assert(std::same_as<decltype(lhs && rhs), bool>);
  };

  auto check_or = [](auto lhs, auto rhs)
  {
    static_assert(lhs || rhs);
    static_assert(std::same_as<decltype(lhs || rhs), bool>);
  };

  check_and(ctrue, ctrue);
  check_or(ctrue, cfalse);
  check_and((std::cw<0> < std::cw<1>), (std::cw<1> < std::cw<5>));
  check_or((std::cw<0> < std::cw<1>), (std::cw<1> < std::cw<5>));

  auto ctruthy = std::cw<Truthy{}>;
  static_assert(has_op_and<decltype(truthy), bool>);
  static_assert(!has_op_and<decltype(ctruthy), decltype(ctrue)>);
  static_assert(!has_op_and<decltype(ctruthy), bool>);
}

struct ThreeWayComp
{
  friend
  constexpr std::strong_ordering
  operator<=>(ThreeWayComp lhs, ThreeWayComp rhs)
  { return lhs.value <=> rhs.value; }

  int value;
};

constexpr void
test_three_way()
{
  auto ctrue = std::cw<true>;
  auto cfalse = std::cw<false>;

  check_same(std::cw<ThreeWayComp{0}> < std::cw<ThreeWayComp{1}>, ctrue);
  check_same(std::cw<ThreeWayComp{2}> > std::cw<ThreeWayComp{1}>, ctrue);
  check_same(std::cw<ThreeWayComp{2}> >= std::cw<ThreeWayComp{1}>, ctrue);
  check_same(std::cw<ThreeWayComp{0}> <= std::cw<ThreeWayComp{1}>, ctrue);
  check_same(std::cw<ThreeWayComp{0}> >= std::cw<ThreeWayComp{1}>, cfalse);

  check_same(std::cw<ThreeWayComp{0}> < ThreeWayComp{1}, true);
  check_same(ThreeWayComp{2} > std::cw<ThreeWayComp{1}>, true);
}

struct EqualityComp
{
  friend
  constexpr bool
  operator==(EqualityComp lhs, EqualityComp rhs)
  { return lhs.value == rhs.value; }

  int value;
};

constexpr void
test_equality()
{
  auto ctrue = std::cw<true>;
  check_same(std::cw<EqualityComp{1}> == std::cw<EqualityComp{1}>, ctrue);
  check_same(std::cw<EqualityComp{0}> != std::cw<EqualityComp{1}>, ctrue);

  check_same(std::cw<EqualityComp{1}> == EqualityComp{1}, true);
  check_same(EqualityComp{0} != std::cw<EqualityComp{1}>, true);
}

struct ConstAssignable
{
  int value;

  constexpr ConstAssignable
  operator=(int rhs) const
  { return ConstAssignable{rhs}; }

  friend constexpr bool
  operator==(const ConstAssignable& lhs, const ConstAssignable& rhs)
  { return lhs.value == rhs.value; }
};

constexpr void
test_assignment()
{
  check_same(std::cw<ConstAssignable{3}> = std::cw<2>,
	     std::cw<ConstAssignable{2}>);
}


constexpr bool
test_all()
{
  test_c_arrays();
  test_ints();
  test_function_object();
  test_function_pointer();
  test_indexable1();
  test_indexable2();
  test_indexable3();
  test_member_pointer();
  test_logic();
  test_three_way();
  test_equality();
  return true;
}

int
main()
{
  test_all();
  static_assert(test_all());
  return 0;
}
