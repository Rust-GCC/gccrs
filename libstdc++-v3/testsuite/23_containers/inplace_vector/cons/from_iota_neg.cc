// { dg-do compile { target c++26 } }
// { dg-require-effective-target int128 }

#include <array>
#include <ranges>
#include <span>
#include <inplace_vector>

template<typename Int, Int min, Int max = std::numeric_limits<Int>::max()>
struct StaticIota : std::ranges::iota_view<Int, Int>
{
  using Base = std::ranges::iota_view<Int, Int>;
  using size_type = std::ranges::range_size_t<Base>;

  constexpr StaticIota() noexcept : Base(min, max) {}

  constexpr static size_type
  size()
  { return size_type(max) - size_type(min); }
};

template<typename Range>
constexpr std::ranges::ref_view<Range>
ref_view(Range& rg)
{ return std::ranges::ref_view<Range>(rg); }

void
test_all()
{
  StaticIota<__int128, 0, 12> m12;

  std::inplace_vector<int, 15> tm1(std::from_range, m12);
  std::inplace_vector<int, 15> tr1(std::from_range, ref_view(m12));

  std::inplace_vector<int, 10> tm2(std::from_range, m12); // { dg-error "(from here|expansion of)" }
  std::inplace_vector<int, 10> tr2(std::from_range, ref_view(m12)); // { dg-error "(from here|expansion of)" }

  StaticIota<__int128, 0> mm;

  std::inplace_vector<int, 10> tm3(std::from_range, mm); // { dg-error "(from here|expansion of)" }
  std::inplace_vector<int, 10> tr3(std::from_range, ref_view(mm)); // { dg-error "(from here|expansion of)" }
}

// { dg-error "static assertion failed" "" { target *-*-* } 0 }
