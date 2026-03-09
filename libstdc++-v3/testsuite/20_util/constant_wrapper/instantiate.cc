// { dg-do run { target c++26 } }
#include <type_traits>
#include <utility>

#include <testsuite_hooks.h>

namespace free_ops
{
  template<int OpId>
    struct UnaryOps
    {
      friend constexpr int
      operator+(UnaryOps) noexcept requires (OpId == 0)
      { return OpId; }

      friend constexpr int
      operator-(UnaryOps) noexcept requires (OpId == 1)
      { return OpId; }

      friend constexpr int
      operator~(UnaryOps) noexcept requires (OpId == 2)
      { return OpId; }

      friend constexpr int
      operator!(UnaryOps) noexcept requires (OpId == 3)
      { return OpId; }

      friend constexpr int
      operator&(UnaryOps) noexcept requires (OpId == 4)
      { return OpId; }

      friend constexpr int
      operator*(UnaryOps) noexcept requires (OpId == 5)
      { return OpId; }

      friend constexpr int
      operator++(UnaryOps) noexcept requires (OpId == 6)
      { return OpId; }

      friend constexpr int
      operator++(UnaryOps, int) noexcept requires (OpId == 7)
      { return OpId; }

      friend constexpr int
      operator--(UnaryOps) noexcept requires (OpId == 8)
      { return OpId; }

      friend constexpr int
      operator--(UnaryOps, int) noexcept requires (OpId == 9)
      { return OpId; }
    };
}

namespace member_ops
{
  template<int OpId>
    struct UnaryOps
    {
      constexpr int
      operator+() const noexcept requires (OpId == 0)
      { return OpId; }

      constexpr int
      operator-() const noexcept requires (OpId == 1)
      { return OpId; }

      constexpr int
      operator~() const noexcept requires (OpId == 2)
      { return OpId; }

      constexpr int
      operator!() const noexcept requires (OpId == 3)
      { return OpId; }

      constexpr int
      operator&() const noexcept requires (OpId == 4)
      { return OpId; }

      constexpr int
      operator*() const noexcept requires (OpId == 5)
      { return OpId; }

      constexpr int
      operator++() const noexcept requires (OpId == 6)
      { return OpId; }

      constexpr int
      operator++(int) const noexcept requires (OpId == 7)
      { return OpId; }

      constexpr int
      operator--() const noexcept requires (OpId == 8)
      { return OpId; }

      constexpr int
      operator--(int) const noexcept requires (OpId == 9)
      { return OpId; }
    };
}

namespace mutable_ops
{
  template<int OpId>
    struct UnaryOps
    {
      constexpr int
      operator+() noexcept requires (OpId == 0)
      { return OpId; }

      constexpr int
      operator-() noexcept requires (OpId == 1)
      { return OpId; }

      constexpr int
      operator~() noexcept requires (OpId == 2)
      { return OpId; }

      constexpr int
      operator!() noexcept requires (OpId == 3)
      { return OpId; }

      constexpr int
      operator&() noexcept requires (OpId == 4)
      { return OpId; }

      constexpr int
      operator*() noexcept requires (OpId == 5)
      { return OpId; }

      constexpr int
      operator++() noexcept requires (OpId == 6)
      { return OpId; }

      constexpr int
      operator++(int) noexcept requires (OpId == 7)
      { return OpId; }

      constexpr int
      operator--() noexcept requires (OpId == 8)
      { return OpId; }

      constexpr int
      operator--(int) noexcept requires (OpId == 9)
      { return OpId; }
    };
}

constexpr size_t n_unary_ops = 10;

template<template<int> typename Ops, int EnabledId, int ActiveId = EnabledId>
  constexpr void
  test_unary_operator()
  {
    auto x = std::cw<Ops<EnabledId>{}>;

    auto check = [](auto c)
    {
      VERIFY(c == EnabledId);
      static_assert(std::same_as<decltype(c), std::constant_wrapper<EnabledId>>);
    };

#define CHECK_EXPR(Id, Expr)      \
    if constexpr (ActiveId == Id) \
      check(Expr);                \
    else                          \
      static_assert(!requires { Expr; })

    CHECK_EXPR(0, +x);
    CHECK_EXPR(1, -x);
    CHECK_EXPR(2, ~x);
    CHECK_EXPR(3, !x);
    if constexpr (ActiveId == 4)
      check(&x);
    CHECK_EXPR(5, *x);
    CHECK_EXPR(6, ++x);
    CHECK_EXPR(7, x++);
    CHECK_EXPR(8, --x);
    CHECK_EXPR(9, x--);
#undef CHECK_EXPR

    static_assert(n_unary_ops == 10);
  }

constexpr void
test_unary_operators_all()
{
  auto run = []<size_t... Idx>(std::integer_sequence<size_t, Idx...>)
  {
    (test_unary_operator<free_ops::UnaryOps, Idx>(), ...);
    (test_unary_operator<member_ops::UnaryOps, Idx>(), ...);
    (test_unary_operator<mutable_ops::UnaryOps, Idx, -1>(), ...);
  };
  run(std::make_index_sequence<n_unary_ops>());
}

namespace free_ops
{
  template<int OpId>
    struct BinaryOps
    {
      friend constexpr int
      operator+(BinaryOps, BinaryOps) noexcept requires (OpId == 0)
      { return OpId; }

      friend constexpr int
      operator-(BinaryOps, BinaryOps) noexcept requires (OpId == 1)
      { return OpId; }

      friend constexpr int
      operator*(BinaryOps, BinaryOps) noexcept requires (OpId == 2)
      { return OpId; }

      friend constexpr int
      operator/(BinaryOps, BinaryOps) noexcept requires (OpId == 3)
      { return OpId; }

      friend constexpr int
      operator%(BinaryOps, BinaryOps) noexcept requires (OpId == 4)
      { return OpId; }

      friend constexpr int
      operator<<(BinaryOps, BinaryOps) noexcept requires (OpId == 5)
      { return OpId; }

      friend constexpr int
      operator>>(BinaryOps, BinaryOps) noexcept requires (OpId == 6)
      { return OpId; }

      friend constexpr int
      operator&(BinaryOps, BinaryOps) noexcept requires (OpId == 7)
      { return OpId; }

      friend constexpr int
      operator|(BinaryOps, BinaryOps) noexcept requires (OpId == 8)
      { return OpId; }

      friend constexpr int
      operator^(BinaryOps, BinaryOps) noexcept requires (OpId == 9)
      { return OpId; }

      friend constexpr int
      operator&&(BinaryOps, BinaryOps) noexcept requires (OpId == 10)
      { return OpId; }

      friend constexpr int
      operator||(BinaryOps, BinaryOps) noexcept requires (OpId == 11)
      { return OpId; }

      friend constexpr int
      operator<=>(BinaryOps, BinaryOps) noexcept requires (OpId == 12)
      { return OpId; }

      friend constexpr int
      operator<(BinaryOps, BinaryOps) noexcept requires (OpId == 13)
      { return OpId; }

      friend constexpr int
      operator<=(BinaryOps, BinaryOps) noexcept requires (OpId == 14)
      { return OpId; }

      friend constexpr int
      operator==(BinaryOps, BinaryOps) noexcept requires (OpId == 15)
      { return OpId; }

      friend constexpr int
      operator!=(BinaryOps, BinaryOps) noexcept requires (OpId == 16)
      { return OpId; }

      friend constexpr int
      operator>(BinaryOps, BinaryOps) noexcept requires (OpId == 17)
      { return OpId; }

      friend constexpr int
      operator>=(BinaryOps, BinaryOps) noexcept requires (OpId == 18)
      { return OpId; }

      friend constexpr int
      operator+=(BinaryOps, BinaryOps) noexcept requires (OpId == 19)
      { return OpId; }

      friend constexpr int
      operator-=(BinaryOps, BinaryOps) noexcept requires (OpId == 20)
      { return OpId; }

      friend constexpr int
      operator*=(BinaryOps, BinaryOps) noexcept requires (OpId == 21)
      { return OpId; }

      friend constexpr int
      operator/=(BinaryOps, BinaryOps) noexcept requires (OpId == 22)
      { return OpId; }

      friend constexpr int
      operator%=(BinaryOps, BinaryOps) noexcept requires (OpId == 23)
      { return OpId; }

      friend constexpr int
      operator&=(BinaryOps, BinaryOps) noexcept requires (OpId == 24)
      { return OpId; }

      friend constexpr int
      operator|=(BinaryOps, BinaryOps) noexcept requires (OpId == 25)
      { return OpId; }
      friend constexpr int

      operator^=(BinaryOps, BinaryOps) noexcept requires (OpId == 26)
      { return OpId; }

      friend constexpr int
      operator<<=(BinaryOps, BinaryOps) noexcept requires (OpId == 27)
      { return OpId; }

      friend constexpr int
      operator>>=(BinaryOps, BinaryOps) noexcept requires (OpId == 28)
      { return OpId; }
    };
}

namespace member_ops
{
  template<int OpId>
    struct BinaryOps
    {
      constexpr int
      operator+(BinaryOps) const noexcept requires (OpId == 0)
      { return OpId; }

      constexpr int
      operator-(BinaryOps) const noexcept requires (OpId == 1)
      { return OpId; }

      constexpr int
      operator*(BinaryOps) const noexcept requires (OpId == 2)
      { return OpId; }

      constexpr int
      operator/(BinaryOps) const noexcept requires (OpId == 3)
      { return OpId; }

      constexpr int
      operator%(BinaryOps) const noexcept requires (OpId == 4)
      { return OpId; }

      constexpr int
      operator<<(BinaryOps) const noexcept requires (OpId == 5)
      { return OpId; }

      constexpr int
      operator>>(BinaryOps) const noexcept requires (OpId == 6)
      { return OpId; }

      constexpr int
      operator&(BinaryOps) const noexcept requires (OpId == 7)
      { return OpId; }

      constexpr int
      operator|(BinaryOps) const noexcept requires (OpId == 8)
      { return OpId; }

      constexpr int
      operator^(BinaryOps) const noexcept requires (OpId == 9)
      { return OpId; }

      constexpr int
      operator&&(BinaryOps) const noexcept requires (OpId == 10)
      { return OpId; }

      constexpr int
      operator||(BinaryOps) const noexcept requires (OpId == 11)
      { return OpId; }

      constexpr int
      operator<=>(BinaryOps) const noexcept requires (OpId == 12)
      { return OpId; }

      constexpr int
      operator<(BinaryOps) const noexcept requires (OpId == 13)
      { return OpId; }

      constexpr int
      operator<=(BinaryOps) const noexcept requires (OpId == 14)
      { return OpId; }

      constexpr int
      operator==(BinaryOps) const noexcept requires (OpId == 15)
      { return OpId; }

      constexpr int
      operator!=(BinaryOps) const noexcept requires (OpId == 16)
      { return OpId; }

      constexpr int
      operator>(BinaryOps) const noexcept requires (OpId == 17)
      { return OpId; }

      constexpr int
      operator>=(BinaryOps) const noexcept requires (OpId == 18)
      { return OpId; }

      constexpr int
      operator+=(BinaryOps) const noexcept requires (OpId == 19)
      { return OpId; }

      constexpr int
      operator-=(BinaryOps) const noexcept requires (OpId == 20)
      { return OpId; }

      constexpr int
      operator*=(BinaryOps) const noexcept requires (OpId == 21)
      { return OpId; }

      constexpr int
      operator/=(BinaryOps) const noexcept requires (OpId == 22)
      { return OpId; }

      constexpr int
      operator%=(BinaryOps) const noexcept requires (OpId == 23)
      { return OpId; }

      constexpr int
      operator&=(BinaryOps) const noexcept requires (OpId == 24)
      { return OpId; }

      constexpr int
      operator|=(BinaryOps) const noexcept requires (OpId == 25)
      { return OpId; }

      constexpr int
      operator^=(BinaryOps) const noexcept requires (OpId == 26)
      { return OpId; }

      constexpr int
      operator<<=(BinaryOps) const noexcept requires (OpId == 27)
      { return OpId; }

      constexpr int
      operator>>=(BinaryOps) const noexcept requires (OpId == 28)
      { return OpId; }
    };
}

namespace mutable_ops
{
  template<int OpId>
    struct BinaryOps
    {
      constexpr int
      operator+(BinaryOps) noexcept requires (OpId == 0)
      { return OpId; }

      constexpr int
      operator-(BinaryOps) noexcept requires (OpId == 1)
      { return OpId; }

      constexpr int
      operator*(BinaryOps) noexcept requires (OpId == 2)
      { return OpId; }

      constexpr int
      operator/(BinaryOps) noexcept requires (OpId == 3)
      { return OpId; }

      constexpr int
      operator%(BinaryOps) noexcept requires (OpId == 4)
      { return OpId; }

      constexpr int
      operator<<(BinaryOps) noexcept requires (OpId == 5)
      { return OpId; }

      constexpr int
      operator>>(BinaryOps) noexcept requires (OpId == 6)
      { return OpId; }

      constexpr int
      operator&(BinaryOps) noexcept requires (OpId == 7)
      { return OpId; }

      constexpr int
      operator|(BinaryOps) noexcept requires (OpId == 8)
      { return OpId; }

      constexpr int
      operator^(BinaryOps) noexcept requires (OpId == 9)
      { return OpId; }

      constexpr int
      operator&&(BinaryOps) noexcept requires (OpId == 10)
      { return OpId; }

      constexpr int
      operator||(BinaryOps) noexcept requires (OpId == 11)
      { return OpId; }

      constexpr int
      operator<=>(BinaryOps) noexcept requires (OpId == 12)
      { return OpId; }

      constexpr int
      operator<(BinaryOps) noexcept requires (OpId == 13)
      { return OpId; }

      constexpr int
      operator<=(BinaryOps) noexcept requires (OpId == 14)
      { return OpId; }

      constexpr int
      operator==(BinaryOps) noexcept requires (OpId == 15)
      { return OpId; }

      constexpr int
      operator!=(BinaryOps) noexcept requires (OpId == 16)
      { return OpId; }

      constexpr int
      operator>(BinaryOps) noexcept requires (OpId == 17)
      { return OpId; }

      constexpr int
      operator>=(BinaryOps) noexcept requires (OpId == 18)
      { return OpId; }

      constexpr int
      operator+=(BinaryOps) noexcept requires (OpId == 19)
      { return OpId; }

      constexpr int
      operator-=(BinaryOps) noexcept requires (OpId == 20)
      { return OpId; }

      constexpr int
      operator*=(BinaryOps) noexcept requires (OpId == 21)
      { return OpId; }

      constexpr int
      operator/=(BinaryOps) noexcept requires (OpId == 22)
      { return OpId; }

      constexpr int
      operator%=(BinaryOps) noexcept requires (OpId == 23)
      { return OpId; }

      constexpr int
      operator&=(BinaryOps) noexcept requires (OpId == 24)
      { return OpId; }

      constexpr int
      operator|=(BinaryOps) noexcept requires (OpId == 25)
      { return OpId; }

      constexpr int
      operator^=(BinaryOps) noexcept requires (OpId == 26)
      { return OpId; }

      constexpr int
      operator<<=(BinaryOps) noexcept requires (OpId == 27)
      { return OpId; }

      constexpr int
      operator>>=(BinaryOps) noexcept requires (OpId == 28)
      { return OpId; }
    };
}

constexpr size_t n_binary_ops = 29;

template<template<int> typename Ops, int EnabledId, int ActiveId = EnabledId>
  constexpr void
  test_binary_operator()
  {
    auto cx = std::cw<Ops<EnabledId>{}>;
    auto cy = std::cw<Ops<EnabledId>{}>;

    auto check = []<typename ResultT>(auto c, ResultT)
    {
      VERIFY(c == ResultT::value);
      static_assert(std::same_as<decltype(c), ResultT>);
    };

#define CHECK_OP(Id, Op)            \
    if constexpr (ActiveId == Id)   \
      check(cx Op cy, std::cw<Id>); \
    else                            \
      static_assert(!requires { cx Op cy; })

#define CHECK_OP_F(Id, Op, Fb)             \
    if constexpr (ActiveId == Fb)          \
      check(cx Op cy, std::cw<(Fb Op 0)>); \
    else CHECK_OP(Id, Op)

    CHECK_OP( 0, +);
    CHECK_OP( 1, -);
    CHECK_OP( 2, *);
    CHECK_OP( 3, /);
    CHECK_OP( 4, %);
    CHECK_OP( 5, <<);
    CHECK_OP( 6, >>);
    CHECK_OP( 7, &);
    CHECK_OP( 8, |);
    CHECK_OP( 9, ^);
    CHECK_OP(10, &&);
    CHECK_OP(11, ||);
    CHECK_OP(12, <=>);
    CHECK_OP_F(13, <,  12);
    CHECK_OP_F(14, <=, 12);
    CHECK_OP_F(17, >,  12);
    CHECK_OP_F(18, >=, 12);
    CHECK_OP(15, ==);
    CHECK_OP(16, !=);
    CHECK_OP(19, +=);
    CHECK_OP(20, -=);
    CHECK_OP(21, *=);
    CHECK_OP(22, /=);
    CHECK_OP(23, %=);
    CHECK_OP(24, &=);
    CHECK_OP(25, |=);
    CHECK_OP(26, ^=);
    CHECK_OP(27, <<=);
    CHECK_OP(28, >>=);
#undef CHECK_OP_F
#undef CHECK_OP

    static_assert(n_binary_ops == 29);
  }

template<template<int> typename Ops, int OpId>
  constexpr void
  test_mixed_binary_operators()
  {
    constexpr auto x = Ops<OpId>{};
    auto cx = std::cw<x>;
    constexpr auto y = Ops<OpId>{};
    auto cy = std::cw<y>;

    auto check = []<typename ResT>(auto vc, auto cv, ResT res)
    {
      VERIFY(vc == res);
      static_assert(std::same_as<decltype(vc), ResT>);

      VERIFY(cv == res);
      static_assert(std::same_as<decltype(cv), ResT>);
    };

#define CHECK_OP(Id, Op)           \
    if constexpr (OpId == Id)      \
      check(x Op cy, cx Op y, Id); \
    else                           \
      static_assert(!requires { x Op cy; } && !requires { cx Op y; })

#define CHECK_OP_F(Id, Op, Fb)          \
    if constexpr (OpId == Fb)           \
      check(x Op cy, cx Op y, Id Op 0); \
    else CHECK_OP(Id, Op)

    CHECK_OP( 0, +);
    CHECK_OP( 1, -);
    CHECK_OP( 2, *);
    CHECK_OP( 3, /);
    CHECK_OP( 4, %);
    CHECK_OP( 5, <<);
    CHECK_OP( 6, >>);
    CHECK_OP( 7, &);
    CHECK_OP( 8, |);
    CHECK_OP( 9, ^);
    CHECK_OP(10, &&);
    CHECK_OP(11, ||);
    CHECK_OP(12, <=>);
    CHECK_OP_F(13, <,  12);
    CHECK_OP_F(14, <=, 12);
    CHECK_OP_F(17, >,  12);
    CHECK_OP_F(18, >=, 12);
    CHECK_OP(15, ==);
    CHECK_OP(16, !=);
    CHECK_OP(19, +=);
    CHECK_OP(20, -=);
    CHECK_OP(21, *=);
    CHECK_OP(22, /=);
    CHECK_OP(23, %=);
    CHECK_OP(24, &=);
    CHECK_OP(25, |=);
    CHECK_OP(26, ^=);
    CHECK_OP(27, <<=);
    CHECK_OP(28, >>=);
#undef CHECK_OP_F
#undef CHECK_OP

    static_assert(n_binary_ops == 29);
  }

constexpr void
test_binary_operators_all()
{
  auto run = []<size_t... Idx>(std::integer_sequence<size_t, Idx...>)
  {
    (test_binary_operator<free_ops::BinaryOps, Idx>(), ...);
    (test_mixed_binary_operators<free_ops::BinaryOps, Idx>(), ...);
    (test_binary_operator<member_ops::BinaryOps, Idx>(), ...);
    (test_binary_operator<mutable_ops::BinaryOps, Idx, -1>(), ...);
  };
  run(std::make_index_sequence<n_binary_ops>());
}

constexpr bool
test_all()
{
  test_unary_operators_all();
  test_binary_operators_all();
  return true;
}

int
main()
{
  test_all();
  return 0;
}
