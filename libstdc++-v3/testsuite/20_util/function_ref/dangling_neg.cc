// { dg-do compile { target c++26 } }

#include <functional>

template<typename F>
constexpr std::function_ref<int(int, int) const>
create(F f)
{
  std::function_ref<int(int, int) const> fr(f);
  return fr;
}

constexpr auto vLambda = create([](int x, int y) { return x + y; }); // { dg-error "is not a constant expression" }

struct Empty
{
  int
  operator()(int x, int y) const
  { return x + y; }
};

constexpr auto vEmpty = create(Empty{}); // { dg-error "is not a constant expression" }

struct NonEmpty {
  int v;

  int
  operator()(int x, int y) const
  { return x + y + v; }
};

constexpr auto vNonEmpty = create(NonEmpty{3}); // { dg-error "is not a constant expression" }

struct InstanceWins {
  int
  operator()(int x, int y) const
  { return x + y; }

  static int
  operator()(float x, float y)
  { return x + y; }
};

constexpr auto vInstanceWins = create(InstanceWins{}); // { dg-error "is not a constant expression" }

struct EmptyET
{
  int
  operator()(this EmptyET, int x, int y)
  { return x + y; }
};

constexpr auto vEmptyET = create(EmptyET{}); // { dg-error "is not a constant expression" }

struct NonEmptyET {
  int v;

  int
  operator()(this NonEmptyET s, int x, int y)
  { return x + y + s.v; }
};

constexpr auto vNonEmptyET = create(NonEmptyET{3}); // { dg-error "is not a constant expression" }

struct InstanceWinsET {
  int
  operator()(this InstanceWinsET, int x, int y)
  { return x + y; }

  static int
  operator()(float x, float y)
  { return x + y; }
};

constexpr auto vInstanceWinsET = create(InstanceWinsET{}); // { dg-error "is not a constant expression" }

