// { dg-do compile { target c++26 } }

#include <array>
#include <ranges>
#include <span>
#include <inplace_vector>

template<typename Range0>
void
test_zero(Range0&& r0)
{
  std::inplace_vector<int, 0> f0(std::from_range, r0);
  std::inplace_vector<int, 1> f1(std::from_range, r0);
  std::inplace_vector<int, 5> f5(std::from_range, r0);
  std::inplace_vector<int, 7> f7(std::from_range, r0);
}

template<typename Range1>
void
test_one(Range1&& r1)
{
  std::inplace_vector<int, 0> f0(std::from_range, r1); // { dg-error "required from" }
  std::inplace_vector<int, 1> f1(std::from_range, r1);
  std::inplace_vector<int, 5> f5(std::from_range, r1);
  std::inplace_vector<int, 7> f7(std::from_range, r1);
}

template<typename Range5>
void
test_five(Range5&& r5)
{
  std::inplace_vector<int, 0> f0(std::from_range, r5); // { dg-error "required from" }
  std::inplace_vector<int, 1> f1(std::from_range, r5); // { dg-error "required from" }
  std::inplace_vector<int, 3> f3(std::from_range, r5); // { dg-error "required from" }
  std::inplace_vector<int, 5> f5(std::from_range, r5);
  std::inplace_vector<int, 7> f7(std::from_range, r5);
}

template<typename Range>
constexpr std::ranges::ref_view<Range>
ref_view(Range& rg)
{ return std::ranges::ref_view<Range>(rg); }

void
test_all()
{
  std::array<int, 0> a0;
  std::span<int, 0> s0(a0);
  std::array<int, 1> a1;
  std::span<int, 1> s1(a1);
  std::array<int, 5> a5;
  std::span<int, 5> s5(a5);
  std::array<int, 7> a7;
  std::span<int, 7> s7(a7);

  test_zero(a0);
  test_zero(s0);
  test_zero(ref_view(a0));
  test_zero(std::views::empty<int>);
  test_zero(s5 | std::views::adjacent<7> | std::views::elements<0>);
  test_zero(a5 | std::views::adjacent<7> | std::views::elements<0>);

  test_one(a1); // { dg-error "from here" }
  test_one(s1); // { dg-error "from here" }
  // ref_view is not statically sized due pointer dereference
  test_one(ref_view(a1));
  test_one(a5 | std::views::adjacent<7> | std::views::elements<0>);
  test_one(s5 | std::views::adjacent<5> | std::views::elements<0>); // { dg-error "from here" }

  test_five(a5); // { dg-error "from here" }
  test_five(s5); // { dg-error "from here" }
  // ref_view is not statically sized due pointer dereference
  test_five(ref_view(a5));
  test_five(a7 | std::views::adjacent<3> | std::views::elements<0>);
  test_five(s7 | std::views::adjacent<3> | std::views::elements<0>); // { dg-error "from here" }
}

// { dg-error "static assertion failed" "" { target *-*-* } 0 }
