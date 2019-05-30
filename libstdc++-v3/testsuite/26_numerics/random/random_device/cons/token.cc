// { dg-do run { target c++11 } }
// { dg-require-cstdint "" }
//
// 2008-11-24  Edward M. Smith-Rowland <3dw4rd@verizon.net>
//
// Copyright (C) 2008-2019 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// C++11 26.5.6 class random_device [rand.device]

#include <random>
#include <stdexcept>
#include <testsuite_hooks.h>

void
test01()
{
  std::random_device x("default");

  VERIFY( x.min() == std::numeric_limits<std::random_device::result_type>::min() );
  VERIFY( x.max() == std::numeric_limits<std::random_device::result_type>::max() );

}

void
test02()
{
#ifdef _GLIBCXX_USE_DEV_RANDOM
  std::random_device x1("/dev/urandom");
  std::random_device x2("/dev/random");
#endif
}

void
test03()
{
  // At least one of these tokens should be valid.
  const std::string tokens[] = {
    "rdseed", "rdrand", "rand_s", "/dev/urandom", "/dev/random", "mt19337"
  };
  int count = 0;
  for (const std::string& token : tokens)
  {
    try
    {
      std::random_device x(token);
      ++count;
    }
    catch (const std::runtime_error&)
    {
    }
  }
  VERIFY( count != 0 );
}

void
test04()
{
  bool can_use_mt19937 = true;
  try
  {
    std::random_device x("mt19937");
  }
  catch (const std::runtime_error&)
  {
    can_use_mt19937 = false;
  }

  // If "mt19337" is a valid token then numeric seeds should be too.
  if (can_use_mt19937)
  {
    std::random_device x1("0");
    std::random_device x2("1234");
    std::random_device x3("0xc0fefe");
  }
}

int main()
{
  test01();
  test02();
  test03();
  test04();
}
