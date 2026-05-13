// { dg-do run { target c++11 } }
// { dg-require-cstdint "" }

#include <random>
#include <testsuite_hooks.h>
#include <testsuite_iterators.h>

using dist = std::piecewise_linear_distribution<>;

template<typename... Args>
bool
is_default(Args&&... args)
{
  std::piecewise_linear_distribution<> u(std::forward<Args>(args)...);

  std::vector<double> ints = u.intervals();
  if (ints.size() != 2 || ints[0] != 0.0 || ints[1] != 1.0)
    return false;

  std::vector<double> dens = u.densities();
  if (dens.size() != 2 || dens[0] != 1.0 || dens[1] != 1.0)
    return false;

  return true;
}

template<template<typename> class Range>
void
test_it_pair()
{
  double wt[3];
  double x[3];

  Range<double> r0(x, x);
  VERIFY( is_default(r0.begin(), r0.end(), wt) );

  x[0] = 4.2;
  Range<double> r1a(x, x+1);
  VERIFY( is_default(r1a.begin(), r1a.end(), wt) );

  x[0] = 0.0;
  Range<double> r1b(x, x+1);
  VERIFY( is_default(r1b.begin(), r1b.end(), wt) );

  x[1] = 1.0;
  wt[0] = wt[1] = 13.0;
  Range<double> r2a(x, x+2);
  VERIFY( is_default(r2a.begin(), r2a.end(), wt) );

  wt[0] = wt[1] = 1.0;
  Range<double> r2b(x, x+2);
  VERIFY( is_default(r2b.begin(), r2b.end(), wt) );

  wt[1] = 2.0;
  Range<double> r2c(x, x+2);
  VERIFY( !is_default(r2c.begin(), r2c.end(), wt) );

  x[1] = 0.5;
  wt[0] = wt[1] = 2.0;
  Range<double> r2d(x, x+2);
  VERIFY( !is_default(r2d.begin(), r2d.end(), wt) );

  x[2] = 1.0;
  wt[2] = wt[0];
  Range<double> r3(x, x+3);
  VERIFY( !is_default(r3.begin(), r3.end(), wt) );
}

void test_init_list()
{
  auto c1 = [](double x) { return 1.0; };
  VERIFY(  is_default(std::initializer_list<double>{}, c1) );
  VERIFY(  is_default(std::initializer_list<double>{4.2}, c1) );
  VERIFY(  is_default(std::initializer_list<double>{0.0, 1.0}, c1) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5}, c1) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5, 1.0}, c1) );

  auto c2 = [](double x) { return 4.2; };
  VERIFY(  is_default(std::initializer_list<double>{}, c2) );
  VERIFY(  is_default(std::initializer_list<double>{4.2}, c2) );
  VERIFY(  is_default(std::initializer_list<double>{0.0, 1.0}, c2) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5}, c2) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5, 1.0}, c2) );

  auto id = [](double x) { return x; };
  VERIFY(  is_default(std::initializer_list<double>{}, id) );
  VERIFY(  is_default(std::initializer_list<double>{4.2}, id) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 1.0}, id) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5}, id) );
  VERIFY( !is_default(std::initializer_list<double>{0.0, 0.5, 1.0}, id) );
}

void test_xbound()
{
  auto c1 = [](double x) { return 1.0; };
  VERIFY(  is_default(0, 0.0, 1.0, c1) );
  VERIFY( !is_default(0, 0.0, 0.5, c1) );
  VERIFY(  is_default(1, 0.0, 1.0, c1) );
  VERIFY( !is_default(1, 0.0, 0.5, c1) );
  VERIFY( !is_default(2, 0.0, 1.0, c1) );

  auto c2 = [](double x) { return 4.2; };
  VERIFY(  is_default(0, 0.0, 1.0, c2) );
  VERIFY( !is_default(0, 0.0, 0.5, c2) );
  VERIFY(  is_default(1, 0.0, 1.0, c2) );
  VERIFY( !is_default(1, 0.0, 0.5, c2) );
  VERIFY( !is_default(2, 0.0, 1.0, c2) );

  auto id = [](double x) { return x; };
  VERIFY( !is_default(0, 0.0, 1.0, id) );
  VERIFY( !is_default(0, 0.0, 0.5, id) );
  VERIFY( !is_default(1, 0.0, 1.0, id) );
  VERIFY( !is_default(1, 0.0, 0.5, id) );
  VERIFY( !is_default(2, 0.0, 1.0, id) );
}

int main()
{
  using namespace __gnu_test;
  test_it_pair<input_container>();
  test_it_pair<forward_container>();
  test_it_pair<random_access_container>();

  test_init_list();
  test_xbound();
  return 0;
}
