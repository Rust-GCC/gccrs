// { dg-do compile { target c++26 } }

#include <functional>

template<typename F>
constexpr std::function_ref<int(int, int) const>
create(F f)
{
  std::function_ref<int(int, int) const> fr(f);
  return fr;
}

constexpr auto vLambda = create([](int x, int y) static { return x + y; });

constexpr auto vTgreater = create(std::greater<int>{});
constexpr auto vDgreater = create(std::greater<>{});
constexpr auto vRgreater = create(std::ranges::greater{});

constexpr auto vTplus = create(std::plus<int>{});
constexpr auto vDplus = create(std::plus<>{});

struct Empty
{
  static int
  operator()(int x, int y)
  { return x + y; }
};

constexpr auto vEmpty = create(Empty{});

struct NonEmpty {
  int v;

  static int
  operator()(int x, int y)
  { return x + y; }
};

constexpr auto vNonEmpty = create(NonEmpty{3});

struct NonStatic {
  int v;

  int
  operator()(int x, int y) const
  { return x + y + v; }
};

constexpr auto vNonType = create(std::nontype<NonStatic{3}>);

struct StaticWins {
  static int
  operator()(int x, int y)
  { return x + y; }

  int
  operator()(float x, float y) const
  { return x + y; }
};

constexpr auto vStaticWins = create(StaticWins{});

struct StaticWinsET {
  static int
  operator()(int x, int y)
  { return x + y; }

  int
  operator()(this StaticWinsET, float x, float y)
  { return x + y; }
};

constexpr auto vStaticWinsET = create(StaticWinsET{});

